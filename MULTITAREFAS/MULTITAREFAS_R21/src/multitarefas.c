/*
 * multitarefas.c
 *
 */ 

#include "multitarefas.h"

/* variaveis do sistema multitarefas */
uint8_t 	   tarefa_atual, proxima_tarefa;
tcb_t   	   TCB[NUMERO_DE_TAREFAS+1];
stackptr_t	   ponteiro_de_pilha;
prioridade_t   Prioridades[PRIORIDADE_MAXIMA+1];   /* vetor com as prioridades das tarefas */
uint32_t	   SP;

/* variavel auxiliar para guardar o numero de marcas de tempo */
static tick_t contador_marcas = 0;

static uint8_t numero_tarefas = 0;

/* codigo independente de hardware */
/* funcao para realizar o escalonamento de tarefas por prioridades 
   que retorna a proxima tarefa que sera executada, isto e, aquela que
   tem a maior prioridade e que esta pronta para executar */
   
uint8_t escalonador(void)
{
    
	uint8_t prioridade;
	uint8_t tarefa_selecionada = 0;
    
	/* laço comeca pela maior prioridade ate encontrar 
	uma tarefa em estado de pronta para executar  */	
    for (prioridade=PRIORIDADE_MAXIMA;prioridade>0;prioridade--)
	{ 
      if(Prioridades[prioridade] != 0)
	  {        
        tarefa_selecionada = Prioridades[prioridade];
        if(TCB[tarefa_selecionada].estado == PRONTA)
		{    
		 /* retorna aquela que tem a maior prioridade e que esta pronta para executar */		
          return tarefa_selecionada;    
        }
      }
    } 
    
	/* caso nenhuma esteja pronta para executar, retorna a de menor prioridade, 
	 a qual sempre deve estar pronta para executar */
    if(prioridade == 0) 
	{
    	tarefa_selecionada = Prioridades[prioridade];
    }
	
	return tarefa_selecionada;
}
 


/*********************************************/
void CriaTarefa(tarefa_t p, const char * nome,
stackptr_t pilha, uint16_t tamanho, prioridade_t prioridade)
{
	
	if(tamanho < TAM_MINIMO_PILHA)
	{
		return;
	}
	
	pilha = CriaContexto(p, pilha + tamanho);
	
	/* incrementa o numero de tarefas instaladas */
	numero_tarefas++;

	/* guardar os dados no bloco de controle da tarefa (TCB) */
	TCB[numero_tarefas].nome = nome;
	TCB[numero_tarefas].stack_pointer = (stackptr_t)(pilha);
	TCB[numero_tarefas].estado = PRONTA;
	TCB[numero_tarefas].prioridade = prioridade;
	TCB[numero_tarefas].tempo_espera = 0;
	  
	/* guardar o numero da tarefa (TCB) no vetor de prioridades das tarefas */
	Prioridades[prioridade]=numero_tarefas;

}



/* Serviços do gerenciador de tarefas */
void TarefaSuspende(uint8_t id_tarefa)
{
	REG_ATOMICA_INICIO();
	TCB[id_tarefa].estado = ESPERA; /* tarefa é colocada em espera */
	TrocaContexto(); 		   		/* tarefa atual solicita troca de contexto */
	REG_ATOMICA_FIM();
}

void TarefaContinua(uint8_t id_tarefa)
{
	REG_ATOMICA_INICIO();
	TCB[id_tarefa].estado = PRONTA;			/* tarefa é colocada na fila de prontas */
	TrocaContexto(); 		   				/* tarefa atual solicita troca de contexto */
	REG_ATOMICA_FIM();
}

void TarefaEspera(tick_t qtas_marcas)
{
	if(qtas_marcas > 0)  //** so valores maiores que 0 */
	{
		REG_ATOMICA_INICIO();			/* bloqueia interrupcoes */
		TCB[tarefa_atual].tempo_espera = qtas_marcas;	/* o contador de marcas da tarefa é iniciado com o valor recebido */
		TCB[tarefa_atual].estado = ESPERA;				/* tarefa é colocada na fila de espera */
		TrocaContexto(); 	 /* tarefa atual solicita troca de contexto, so retorna quando ficar pronta novamente */
		REG_ATOMICA_FIM();   /* desbloqueia interrupcoes */
	}
}

/* Exemplo de tarefa ociosa */
void tarefa_ociosa(void)
{
	
	for(;;)
	{		
		#if 0  /* para o uso como sistema cooperativo*/
			REG_ATOMICA_INICIO();
			TrocaContexto();				/* tarefa atual solicita troca de contexto */
			REG_ATOMICA_FIM();
		#endif
	}
}


void IniciaMultitarefas(void)
{
	tarefa_atual = escalonador();
	ponteiro_de_pilha = TCB[tarefa_atual].stack_pointer;
	SP = ponteiro_de_pilha;
	GERA_INTERRUPCAO_SW();
}

void TrocaContextoDasTarefas(void)
{
	
	/* guarda o valor antigo do stack pointer */
	TCB[tarefa_atual].stack_pointer = SP;
		
	/* executa o escalonador */
	proxima_tarefa = escalonador();
		
	/* seleciona a nova tarefa */
	tarefa_atual = proxima_tarefa;
		
	/* coloca um novo valor no stack pointer */
	ponteiro_de_pilha = TCB[tarefa_atual].stack_pointer;
		
	SP = ponteiro_de_pilha;

}
void ExecutaMarcaDeTempo(void)
{
	
	uint8_t tarefa = 0;
		
	++contador_marcas; /* incrementa contador de marcas de tempo */
	
	/* laco para decrementar tempo de espera das tarefas 
	 * e coloca-las na fila de prontas para executar  */	
	for (tarefa=numero_tarefas;tarefa > 0;tarefa--)
	{ 
	  
		if(TCB[tarefa].tempo_espera > 0 ) /* se esta esperando algum tempo */
		{	
			TCB[tarefa].tempo_espera--; /* decrementa tempo de espera */
			
			if(TCB[tarefa].tempo_espera == 0 )
			{
				/* coloca a tarefa na fila de prontas para executar */	
				TCB[tarefa].estado = PRONTA;	        				
			}
		}
	 }
}

/* Servicos de semaforos */
void SemaforoAguarda(semaforo_t* sem)
{
	
	REG_ATOMICA_INICIO();
	
	if(sem->contador > 0)
	{
		sem->contador--;
	}else
	{
		TCB[tarefa_atual].estado = ESPERA;		/* tarefa colocada na fila de espera */
		sem->tarefaEsperando = tarefa_atual;   	/* tarefa colocada na espera do semaforo */
		TROCA_CONTEXTO();						/* solicita troca de contexto */
	}
	
	REG_ATOMICA_FIM();
}


void SemaforoLibera(semaforo_t* sem)
{
	REG_ATOMICA_INICIO();
	
	if(sem->tarefaEsperando > 0)
	{	/* tem alguma tarefa aguardando ? */
		TCB[sem->tarefaEsperando].estado = PRONTA;		/* tarefa colocada na fila de pronta */
		sem->tarefaEsperando = 0;						/* tarefa retirada da espera do semaforo */
	}else
	{
		sem->contador++;
	}
	TROCA_CONTEXTO();
	
	REG_ATOMICA_FIM();
}
