#include "../include/cpu.h"
#include "../include/funciones_cpu.h"


void cargar_instruccion_cpu(int id, char* nombre, uint32_t parametro1, uint32_t parametro2){
	instrucciones* estructura_instrucciones = malloc(sizeof(instrucciones));

	estructura_instrucciones->id = id;
	estructura_instrucciones->nombre = nombre;
	estructura_instrucciones->parametro1 = parametro1;
	estructura_instrucciones->parametro2 = parametro2;

	list_add(lista_instrucciones_cpu,estructura_instrucciones);
	//free(estructura_instrucciones);
}
