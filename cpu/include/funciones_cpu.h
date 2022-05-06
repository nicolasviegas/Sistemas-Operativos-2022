#ifndef KERNEL_INCLUDE_FUNCIONES_KERNEL_H_
#define KERNEL_INCLUDE_FUNCIONES_KERNEL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/config.h>
#include "../../shared/include/utils.h"
#include "../../shared/include/socket.h"
#include "../../shared/include/protocolo.h"
#include "../../shared/include/consola-kernel.h"

/*typedef struct{
	int id;
	char* nombre;
	uint32_t parametro1;
	uint32_t parametro2;
}instrucciones;*/

void cargar_instruccion_cpu(int id, char* nombre, uint32_t parametro1, uint32_t parametro2);

#endif /* CPU_INCLUDE_FUNCIONES_CPU_H_ */