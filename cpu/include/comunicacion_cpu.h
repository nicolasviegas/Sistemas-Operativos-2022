#ifndef CPU_INCLUDE_COMUNICACION_CPU_H_
#define CPU_INCLUDE_COMUNICACION_CPU_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/config.h>
//#include "../../shared/include/utils.h"
//#include "../../shared/include/socket.h"
//#include "../../shared/include/protocolo.h"
//#include "../../shared/include/consola-kernel.h"
#include "utils.h"
#include "socket.h"
#include "protocolo.h"
#include "consola-kernel.h"
/*
 * void deserializar_sem_init(void* stream, mate_sem_name* nombre, unsigned int* valor);
 * void deserializar_sem_wait(void* stream, mate_sem_name* nombre);
 * void deserializar_sem_post(void* stream, mate_sem_name* nombre);
 * void deserializar_sem_destroy(void* stream, mate_sem_name* nombre);
 * void deserializar_call_io(void* stream, mate_io_resource* io, char** msg);

*/

int contador_instrucciones;


#endif /* KERNEL_INCLUDE_COMUNICACION_KERNEL_H_ */


