#ifndef KERNEL_INCLUDE_PLANIFICACION_H_
#define KERNEL_INCLUDE_PLANIFICACION_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <pthread.h>
#include <commons/collections/queue.h>
#include <semaphore.h>
#include <time.h>
#include "../include/kernel.h"
#include "utils.h"
typedef struct{
	uint32_t pid;
	uint32_t nro_frame;
	bool bit_P;
}pagina; // VER SI NO HAY QUE MOVER A MEM

typedef struct{
	uint32_t PID;
	uint32_t tamanio;
	t_list* instrucciones;
	uint32_t PC;
	uint32_t indice_tabla_paginas;
	uint32_t estimacionRafaga;
	uint32_t alpha;
	uint32_t estado;

	float rafagaAnterior;
	float estimacionActual;
	float estimacionAnterior;
	float horaDeIngresoAExe;
	bool suspendido;
	uint32_t tiempo_bloqueo;
}pcb_t;

typedef enum{
	SRT,
	FIFO
}t_algoritmo_planificacion;

uint32_t tiempo_bloq_kernel;

t_list* lista_pcb_en_memoria;
t_list* lista_pcb_totales;

void cargar_PCB_kernel(int contador_cliente, uint32_t tam, t_list* lista_nueva_kernel, t_list* tabla_paginas, int estimacion_inicial, int alfa);

void send_instrucciones_kernel_a_cpu(int fd_cpu,t_log* logger,pcb_t* lista_instrucciones);



///////////////////////////////////////////////////////// COLAS /////////////////////////////////////////////////////////

t_queue* colaNew;
t_list* colaReady;
t_list* listaExe;
t_list* listaBlock;
t_list* listaExit;
t_list* listaBlockSuspended;
t_queue* colaReadySuspended;
t_list* listaPotencialesRetensores;

void agregarANew(pcb_t* proceso);
pcb_t* sacarDeNew();
void agregarAReady(pcb_t* proceso);
void agregarABlock(pcb_t* proceso);
void sacarDeBlock(pcb_t* proceso);
void agregarASuspended(pcb_t* proceso);
void sacarDeSuspended(pcb_t* proceso);
void agregarAReadySuspended(pcb_t* proceso);
pcb_t* sacarDeReadySuspended();
void agregarABlockSuspended(pcb_t* pcb);



void hiloNew_Ready();
void hiloReady_Exe();
void hiloExecAExit();
void hiloBlockASuspension();
void hiloSuspensionAReady();
void deteccionYRecuperacion();


pcb_t* obtenerSiguienteDeReady();
pcb_t* obtenerSiguienteSJF();
pcb_t* obtenerSiguienteFIFO();
void actualizarTiemposDeEspera();

void terminarEjecucion(pcb_t* pcb);

bool condiciones_de_suspension();

//
pthread_mutex_t mutexNew;
pthread_mutex_t mutexReady;
pthread_mutex_t mutexBlock;
pthread_mutex_t mutexExe;
pthread_mutex_t mutexExit;
pthread_mutex_t mutexBlockSuspended;
pthread_mutex_t mutexReadySuspended;
pthread_mutex_t mutexPotencialesRetensores;

sem_t contadorNew;
sem_t contadorReady;
sem_t contadorExe;
sem_t contadorProcesosEnMemoria;
sem_t multiprogramacion;
pthread_mutex_t multiprocesamiento;
sem_t contadorBlock;
sem_t analizarSuspension;
sem_t suspensionFinalizada;
sem_t largoPlazo;
sem_t contadorReadySuspended;
sem_t medianoPlazo;

bool hay_alguien_exe;
uint32_t proceso_en_ejecucion;

void enviar_pcb_a_cpu(pcb_t* pcb_proceso);


#endif /* KERNEL_INCLUDE_PLANIFICACION_H_ */
