/*
 * multitarefas.h
 *
 */ 


#ifndef MULTITAREFAS_H_
#define MULTITAREFAS_H_

#include "stdint.h"
#include "cpu-port.h"

/******************************************************************/
/* macros de configuracao */

/* numero de tarefas */
#define NUMERO_DE_TAREFAS	3

/* número de prioridades/tarefas */
#define PRIORIDADE_MAXIMA   4

/* frequencia de clock da CPU */
#define cfg_CPU_CLOCK_HZ 	48000000

/* frequencia da marca de tempo do sistema multitarefas */
#define cfg_MARCA_TEMPO_HZ  1000

typedef  void (*tarefa_t)(void);
typedef enum {PRONTA, ESPERA} estado_tarefa_t;
typedef uint8_t	  prioridade_t;
typedef uint16_t  tick_t;

/**
* \struct tcb_t
* Estrutura de controle de tarefas
*/

typedef struct
{
	const char		*nome;
	stackptr_t 	stack_pointer;
	estado_tarefa_t estado;
	prioridade_t 	prioridade;
	uint16_t		tempo_espera;
}tcb_t;

extern  uint8_t		tarefa_atual;
extern  uint8_t		proxima_tarefa;
extern  tcb_t		TCB[NUMERO_DE_TAREFAS+1];
extern  stackptr_t	ponteiro_de_pilha;
extern  prioridade_t Prioridades[PRIORIDADE_MAXIMA+1];

/**
* \struct semaforo_t
* Estrutura de controle do semaforo
*/

typedef struct 
{
	uint8_t     contador;            ///< Contador do semaforo
	uint8_t 	tarefaEsperando;        ///< Tarefa esperando
} semaforo_t;


void tarefa_ociosa(void);
uint8_t escalonador(void);

void TrocaContextoDasTarefas(void);
uint32_t * CriaContexto(tarefa_t endereco_tarefa, uint32_t* ptr_pilha);
void CriaTarefa(tarefa_t p, const char * nome, stackptr_t pilha, uint16_t tamanho, prioridade_t prioridade);
void IniciaMultitarefas(void);
void ConfiguraMarcaTempo(void);
void ExecutaMarcaDeTempo(void);

void TarefaSuspende(uint8_t id_tarefa);
void TarefaContinua(uint8_t id_tarefa);
void TarefaEspera(tick_t qtas_marcas);		

void SemaforoAguarda(semaforo_t* sem);
void SemaforoLibera(semaforo_t* sem);
#endif /* MULTITAREFAS_H_ */