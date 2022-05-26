#include "../include/funciones_kernel.h"


void cargar_instruccion(int id, char* nombre, uint32_t parametro1, uint32_t parametro2){
	instrucciones* estructura_instrucciones = malloc(sizeof(instrucciones));

	estructura_instrucciones->id = id;
	estructura_instrucciones->nombre = nombre;
	estructura_instrucciones->parametro1 = parametro1;
	estructura_instrucciones->parametro2 = parametro2;

	list_add(lista_instrucciones_kernel,estructura_instrucciones);
	//free(estructura_instrucciones);
}

void cerrar_programa4(t_log* logger) {
    log_destroy(logger);
}


void conexion_kernel_a_memoria(){

	char* ip_memoria = config_get_string_value(config_kernel,"IP_MEMORIA");
	char* puerto_memoria = config_get_string_value(config_kernel,"PUERTO_MEMORIA");


	fd_memoria=0;
		if (!generar_conexion_kernel_a_memoria(log_kernel, ip_memoria, puerto_memoria, &fd_memoria)) {
			cerrar_programa4(log_kernel);
			//return EXIT_FAILURE;
		}
		log_trace(log_kernel,"El fd_memoria despues de grar conexiones es: %d",fd_memoria);

		//return fd_memoria;
}

void pedir_tabla_a_memoria(){
	send_TAM(fd_memoria,666);
}


void enviar_pcb_a_cpu(void* proceso){

	log_trace(log_kernel,"Entre a enviar pcb a cpu");
	pcb_t* pcb_proceso = (pcb_t *) proceso;

	uint32_t a = list_size(lista_instrucciones_kernel);

	send_pid_to_cpu(fd_cpu,pcb_proceso->PID);
	send_TAM(fd_cpu,pcb_proceso->tamanio);
	send_cant_instrucciones(fd_cpu,a);
	send_indice_tabla_paginas_a_cpu(fd_cpu,pcb_proceso->indice_tabla_paginas);
	send_instrucciones_kernel_a_cpu(fd_cpu,log_kernel,pcb_proceso);
	send_PC(fd_cpu,pcb_proceso->PC);

	lista_instrucciones_kernel = list_take_and_remove(lista_instrucciones_kernel,0);

	log_trace(log_kernel, "[AAAAAAA] la cant de instrucciones en la lista: %d",a);

	log_trace(log_kernel,"El PC del proceso %d es: %d",pcb_proceso->PID,pcb_proceso->PC);

	log_trace(log_kernel,"Sali de enviar pcb a cpu");
}




