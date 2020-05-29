#include <stdint.h>
#include <stdio.h>

#include "multitarefas.h"

/*
 * Prototipos das tarefas
 */
<<<<<<< Updated upstream
void tarefa_1(void);
void tarefa_2(void);
=======
void thread0(void);
void thread1(void);
void thread2(void);
void thread3(void);
void thread4(void);
void thread5(void);

// Será usado semaforos para utilizar count
semaforo_t count_flag = 1; // variavel q sinaliza disponibilidade da variavel count
int count;                 // O uso dessa variavel eh considerada uma região crítica.
>>>>>>> Stashed changes

/*
 * Configuracao dos tamanhos das pilhas
 */
#define TAM_PILHA_1		(TAM_MINIMO_PILHA + 24)
#define TAM_PILHA_2		(TAM_MINIMO_PILHA + 24)
<<<<<<< Updated upstream
#define TAM_PILHA_OCIOSA	(TAM_MINIMO_PILHA + 24)

=======
#define TAM_PILHA_3		(TAM_MINIMO_PILHA + 24)

#define TAM_PILHA_4		(TAM_MINIMO_PILHA + 24)
#define TAM_PILHA_5		(TAM_MINIMO_PILHA + 24)
#define TAM_PILHA_6		(TAM_MINIMO_PILHA + 24)

#define TAM_PILHA_OCIOSA	(TAM_MINIMO_PILHA + 24)

#define N_ELEMENTOS 7

>>>>>>> Stashed changes
/*
 * Declaracao das pilhas das tarefas
 */
uint32_t PILHA_TAREFA_1[TAM_PILHA_1];
uint32_t PILHA_TAREFA_2[TAM_PILHA_2];
<<<<<<< Updated upstream
=======
uint32_t PILHA_TAREFA_3[TAM_PILHA_3];

uint32_t PILHA_TAREFA_4[TAM_PILHA_4];
uint32_t PILHA_TAREFA_5[TAM_PILHA_5];
uint32_t PILHA_TAREFA_6[TAM_PILHA_6];

>>>>>>> Stashed changes
uint32_t PILHA_TAREFA_OCIOSA[TAM_PILHA_OCIOSA];

/*
 * Funcao principal de entrada do sistema
 */
int main(void)
{
	
	/* Criacao das tarefas */
	/* Parametros: ponteiro, nome, ponteiro da pilha, tamanho da pilha, prioridade da tarefa */
	
<<<<<<< Updated upstream
	CriaTarefa(tarefa_1, "Tarefa 1", PILHA_TAREFA_1, TAM_PILHA_1, 1);
	
	CriaTarefa(tarefa_2, "Tarefa 2", PILHA_TAREFA_2, TAM_PILHA_2, 2);
=======
	CriaTarefa(thread0, "Tarefa 1", PILHA_TAREFA_1, TAM_PILHA_1, 6);
	
	CriaTarefa(thread1, "Tarefa 2", PILHA_TAREFA_2, TAM_PILHA_2, 5);
        
        CriaTarefa(thread2, "Tarefa 3", PILHA_TAREFA_3, TAM_PILHA_3, 4);
       
        CriaTarefa(thread3, "Tarefa 4", PILHA_TAREFA_4, TAM_PILHA_4, 3);
	
	CriaTarefa(thread4, "Tarefa 5", PILHA_TAREFA_5, TAM_PILHA_5, 2);
        
        CriaTarefa(thread5, "Tarefa 6", PILHA_TAREFA_6, TAM_PILHA_6, 7);
>>>>>>> Stashed changes
	
	/* Cria tarefa ociosa do sistema */
	CriaTarefa(tarefa_ociosa,"Tarefa ociosa", PILHA_TAREFA_OCIOSA, TAM_PILHA_OCIOSA, 1);
	
	/* Configura marca de tempo */
	ConfiguraMarcaTempo();   
	
        /*Inicializa estado das tarefas*/
        TCB[1].estado = ESPERA;
        TCB[2].estado = ESPERA;
        TCB[3].estado = ESPERA;
        TCB[4].estado = ESPERA;
        
	/* Inicia sistema multitarefas */
	IniciaMultitarefas();
	
	/* Nunca chega aqui */
	while (1)
	{
	}
}

<<<<<<< Updated upstream

/* Tarefas de exemplo que usam funcoes para suspender/continuar as tarefas */
void tarefa_1(void)
{
	volatile uint16_t a = 0;
	for(;;)
	{
		a++;
		TarefaContinua(2);
	
	}
}

void tarefa_2(void)
{
	volatile uint16_t b = 0;
	for(;;)
	{
		b++;
		TarefaSuspende(2);	
	}
}
=======
void thread5(){
  int a;
  while(1){
    SemaforoAguarda(&count_flag);
    a = count;
    SemaforoLibera(&count_flag);
    TarefaEspera(3);
  }
  //TarefaEspera(30000);
  //TarefaSuspende(6);
}

// Tarefa de menor prioridade
void thread4(){
  //estado_tarefa_t debbug;
  while(1){
    //debbug = TCB[6].estado;
    SemaforoAguarda(&count_flag);
    count++;
    SemaforoLibera(&count_flag);
    //debbug = TCB[6].estado;
    TarefaContinua(4);
  }
}

void thread3(){
  //estado_tarefa_t debbug;
  while(1){
    //debbug = TCB[6].estado;
    SemaforoAguarda(&count_flag);
    count++;
    SemaforoLibera(&count_flag);
    //debbug = TCB[6].estado;
    TarefaContinua(3);
    TarefaSuspende(4);
  }
}

void thread2(){
  //estado_tarefa_t debbug;
  while(1){
    //debbug = TCB[6].estado;
    SemaforoAguarda(&count_flag);
    count++;
    SemaforoLibera(&count_flag);
    //debbug = TCB[6].estado;
    TarefaContinua(2);
    TarefaSuspende(3);
  }
}

void thread1(){
  //estado_tarefa_t debbug;
  while(1){
    //debbug = TCB[6].estado;
    SemaforoAguarda(&count_flag);
    count++;
    SemaforoLibera(&count_flag);
    //debbug = TCB[6].estado;
    TarefaContinua(1);
    TarefaSuspende(2);
  }
}

void thread0(){
  //estado_tarefa_t debbug;
  while(1){
    //debbug = TCB[6].estado;
    SemaforoAguarda(&count_flag);
    count++;
    SemaforoLibera(&count_flag);
    //debbug = TCB[6].estado;
    TarefaSuspende(1);
  }
}
>>>>>>> Stashed changes
