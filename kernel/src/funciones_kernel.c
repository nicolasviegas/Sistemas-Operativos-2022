#include "../include/funciones_kernel.h"
#include "../include/kernel.h"

void cargar_instruccion(int id, char* nombre, uint32_t parametro1, uint32_t parametro2){
	instrucciones* estructura_instrucciones = malloc(sizeof(instrucciones));

	estructura_instrucciones->id = id;
	estructura_instrucciones->nombre = nombre;
	estructura_instrucciones->parametro1 = parametro1;
	estructura_instrucciones->parametro2 = parametro2;

	list_add(lista_instrucciones_kernel,estructura_instrucciones);
}

//TODO: revisar funcionamiento de esta funcion

int estimar_rafaga(rafaga_t rafaga, double alfa){
	int* nueva_rafaga;
    nueva_rafaga = (rafaga.duracion_total * alfa) + (1 - alfa) * rafaga.rafaga_anterior;

	(*nueva_rafaga) = rafaga.rafaga_actual;
	(*rafaga.rafaga_anterior) = rafaga.rafaga_actual;
	(*rafaga.duracion_total) = rafaga.duracion_total + nueva_rafaga;
	
	return rafaga;
}




