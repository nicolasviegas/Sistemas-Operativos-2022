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
#include "../../shared/include/utils.h"

typedef struct{
	uint32_t carpinchoPID;
	clock_t horaDeIngresoAReady;
	int duracionCpu;
	bool suspendido;
	t_list* semaforosRetenidos;
	//t_semaforo* semaforoEsperado;

	int socketConsola;
	int socketMemoria;
	int socketCpu;
	t_log* logger;
	char* nombreServidor;

}pcb_kernel;


#endif /* KERNEL_INCLUDE_PLANIFICACION_H_ */
