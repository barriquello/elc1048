/*
 * cpu_port.h
 *
 *  Author: Carlos H. Barriquello
 */ 


#ifndef CPU_PORT_H_
#define CPU_PORT_H_

#include "stdint.h"

/* configurar conforme processador*/
/* Ex. processador ARM 16 registradores de 4 bytes cada */
#define TAM_MINIMO_PILHA  (16)

/* tipo do ponteiro de pilha */
typedef uint32_t* stackptr_t;


/* registradores da cpu ARM Cortex-M*/
#define NVIC_INT_CTRL_B         ( ( volatile unsigned long *) 0xe000ed04 )
#define NVIC_SYSPRI3			( ( volatile unsigned long *) 0xe000ed20 )
#define NVIC_SYSTICK_CTRL       ( ( volatile unsigned long *) 0xe000e010 )
#define NVIC_SYSTICK_LOAD       ( ( volatile unsigned long *) 0xe000e014 )

#define NVIC_PENDSVSET      			0x10000000         			// Dispara exceção PendSV
#define NVIC_PENDSVCLR      			0x08000000         			// Limpa a flag PendSV
#define NVIC_SYSTICK_CLK        		0x00000004
#define NVIC_SYSTICK_INT        		0x00000002
#define NVIC_SYSTICK_ENABLE     		0x00000001
#define PRIO_BITS       		        4        					// 15 níveis de prioridade
#define LOWEST_INTERRUPT_PRIORITY		0xF
#define KERNEL_INTERRUPT_PRIORITY 		(LOWEST_INTERRUPT_PRIORITY << (8 - PRIO_BITS) )
#define NVIC_PENDSV_PRI					( ( ( unsigned long ) KERNEL_INTERRUPT_PRIORITY ) << 16 )
#define NVIC_SYSTICK_PRI				( ( ( unsigned long ) KERNEL_INTERRUPT_PRIORITY ) << 24 )


/* macros dependentes de hardware, instruções em assembly */
#define REG_ATOMICA_INICIO()  	  __asm(" CPSID I");
#define REG_ATOMICA_FIM()  		  __asm(" CPSIE I");

#define TROCA_CONTEXTO()		*(NVIC_INT_CTRL_B) = NVIC_PENDSVSET; __asm(" CPSIE I");
#define TrocaContexto()		    TROCA_CONTEXTO()
#define Clear_PendSV(void)		*(NVIC_INT_CTRL_B) = NVIC_PENDSVCLR

#define GERA_INTERRUPCAO_SW()      __asm(  /* Call SVC to start the first task. */		\
										"cpsie i				\n"					\
										"svc 0					\n"					\
									);												\


#define RESTAURA_SP(SP)		__asm(	"LDR	 R1, =SP	    \n"		\
									"LDR     R0, [R1]		\n"		\
							);

#define SALVA_SP(SP)		__asm(	"LDR	R1,	=SP	    \n"		\
									"STR    R0, [R1]	\n"		\
							);

#define SALVA_CONTEXTO()   __asm(								\
								"MRS     R0,PSP			\n"		\
								"SUB     R0, R0, #0x10	\n"		\
								"STM     R0,{R4-R7}		\n"		\
								"MOV     R4,R8          \n"		\
								"MOV     R5,R9          \n"		\
								"MOV     R6,R10         \n"		\
								"MOV     R7,R11         \n"		\
								"SUB     R0, R0, #0x20	\n"		\
								"STM     R0,{R4-R7}		\n"		\
								"SUB     R0, R0, #0x10	\n"		\
							);

#define RESTAURA_CONTEXTO()    __asm volatile(											  \
									/* Restore r4-11 from new process stack */			  \
									"LDM     R0!,{R4-R7}		\n"						  \
									"MOV     R8,R4              \n"						  \
									"MOV     R9,R5              \n"						  \
									"MOV     R10,R6             \n"					  	  \
									"MOV     R11,R7             \n"					  	  \
									"LDM     R0!,{R4-R7}		\n"						  \
									/* Load MSP with new process SP */					  \
									"MSR     PSP, R0			\n"						  \
									/* Ensures that int return use the task stack  */	  \
									"LDR     R1,=0xFFFFFFFD     \n"						  \
									/* Exception return will restore remaining context */ \
									"CPSIE   I					\n"						  \
									"BX      R1               	\n"						  \
								)

#define SALVA_ISR()			// em branco para este processador

#define RESTAURA_ISR()		__asm(							  \
								"LDR     R1,=0xFFFFFFFD     \n"						  \
								/* Exception return will restore remaining context */ \
								"CPSIE   I					\n"						  \
								"BX      R1               	\n"						  \
							)


#endif /* CPU_PORT_H_ */