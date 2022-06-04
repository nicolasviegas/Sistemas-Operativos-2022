#ifndef KERNEL_INCLUDE_FUNCIONES_KERNEL_H_
#define KERNEL_INCLUDE_FUNCIONES_KERNEL_H_

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
#include "../include/kernel.h"
#include "../include/planificacion.h"
#include "kernel.h"

/*typedef struct{
	int id;
	char* nombre;
	uint32_t parametro1;
	uint32_t parametro2;
}instrucciones;*/

void cargar_instruccion(int id, char* nombre, uint32_t parametro1, uint32_t parametro2);

void cargar_instruccion2(int id, char* nombre, uint32_t parametro1, uint32_t parametro2,t_list* lista);

void conexion_kernel_a_memoria();

void pedir_tabla_a_memoria();



#endif /* KERNEL_INCLUDE_FUNCIONES_KERNEL_H_ */
