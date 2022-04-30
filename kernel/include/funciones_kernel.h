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

//TODO: chequear los tipos de datos
typedef struct{
	char* id;
	uint32_t tamanio;
	instrucciones* list_instr;
	int prog_counter;
    pagina tab_pag;
    rafaga_t rafaga; 
}PCB_t;

//TODO: chequear los tipos de datos
typedef struct{
	int* rafaga_actual;
	int* duracion_total;
	int* rafaga_anterior;
}rafaga_t;

typedef struct{}pagina;


void cargar_instruccion(int id, char* nombre, uint32_t parametro1, uint32_t parametro2);

int estimar_rafaga(rafaga_t rafaga, double alfa);

#endif /* KERNEL_INCLUDE_FUNCIONES_KERNEL_H_ */
