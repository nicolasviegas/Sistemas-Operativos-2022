#include "../include/planificacion.h"



void cargar_PCB_kernel(int contador_cliente, uint32_t tam, t_list* lista_nueva_kernel, t_list* tabla_paginas, int estimacion_inicial, int alfa){
	pcb_t* pcb_proceso = malloc(sizeof(pcb_t));

	pcb_proceso->PID = contador_cliente;
	pcb_proceso->tamanio = tam;
	pcb_proceso->instrucciones = lista_nueva_kernel;
	pcb_proceso->tabla_paginas = tabla_paginas;
	pcb_proceso->estimacionRafaga = estimacion_inicial;
	pcb_proceso->alpha = alfa;

	list_add(lista_pcb,pcb_proceso);

	free(pcb_proceso);
}



