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
#include "../../shared/include/utils.h"

typedef struct{
	uint32_t pid;
	uint32_t nro_frame;
	union uso{ //union guarda dos tipos en la misma locacion de memoria
		uint32_t bit_U;    // bit de uso
		uint32_t TUR;       // tiempo de ultima referencia
	};
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

	//t_semaforo* semaforoEsperado;
	//comentario prueba 2

//	int socketConsola;
//	int socketMemoria;
//	int socketCpu;
//	t_log* logger;
//	char* nombreServidor;

}pcb_t;

typedef enum{
	SRT,
	FIFO
}t_algoritmo_planificacion;

t_list* lista_pcb_en_memoria;
t_list* lista_pcb_totales;

void cargar_PCB_kernel(int contador_cliente, uint32_t tam, t_list* lista_nueva_kernel, t_list* tabla_paginas, int estimacion_inicial, int alfa);

//void cargar_lista_nueva(t_list* lista_nueva_kernel,int fd_cpu);

void send_instrucciones_kernel_a_cpu(int fd_cpu,t_log* logger,pcb_t* lista_instrucciones);

#endif /* KERNEL_INCLUDE_PLANIFICACION_H_ */
