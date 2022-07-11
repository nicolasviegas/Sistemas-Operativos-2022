#include "../include/funciones_kernel.h"


void cargar_instruccion(int id, char* nombre, uint32_t parametro1, uint32_t parametro2){
	instrucciones* estructura_instrucciones = malloc(sizeof(instrucciones));

	estructura_instrucciones->id = id;
	estructura_instrucciones->nombre = nombre;
	estructura_instrucciones->parametro1 = parametro1;
	estructura_instrucciones->parametro2 = parametro2;

	list_add(lista_instrucciones_kernel,estructura_instrucciones);
}

void cargar_instruccion2(int id, char* nombre, uint32_t parametro1, uint32_t parametro2,t_list* lista){
	instrucciones* estructura_instrucciones = malloc(sizeof(instrucciones));

	estructura_instrucciones->id = id;
	estructura_instrucciones->nombre = nombre;
	estructura_instrucciones->parametro1 = parametro1;
	estructura_instrucciones->parametro2 = parametro2;

	list_add(lista,estructura_instrucciones);
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

}

void pedir_tabla_a_memoria(){
	send_TAM(fd_memoria,666);
}


void enviar_pcb_a_cpu(pcb_t* pcb_proceso){

	//log_trace(log_kernel,"Entre a enviar pcb a cpu");

	uint32_t b = list_size(pcb_proceso->instrucciones);

	send_pid_to_cpu(fd_cpu,pcb_proceso->PID);
	send_TAM(fd_cpu,pcb_proceso->tamanio);
	send_cant_instrucciones(fd_cpu,b);
	send_indice_tabla_paginas_a_cpu(fd_cpu,pcb_proceso->indice_tabla_paginas);
	send_instrucciones_kernel_a_cpu(fd_cpu,log_kernel,pcb_proceso);
	send_PC(fd_cpu,pcb_proceso->PC);

	list_clean(lista_instrucciones_kernel);

	log_trace(log_kernel, "Cantidad de instrucciones restantes del proceso %d: %d",pcb_proceso->PID,(b - pcb_proceso->PC));

	//log_trace(log_kernel,"El PC del proceso %d es: %d",pcb_proceso->PID,pcb_proceso->PC);

	//log_trace(log_kernel,"Sali de enviar pcb a cpu");
}

void send_instrucciones_kernel_a_cpu(int fd_cpu,t_log* logger,pcb_t* pcb_proceso){
		//printf("[SEND INSTRUCCIONES KERNEL A CPU] cantidad de instrucciones: %d \n",list_size(pcb_proceso->instrucciones));
		instrucciones* a;
		int cant_instrucciones = list_size(pcb_proceso->instrucciones);
		int indice = 0;

		//log_error(logger,"El socket dentro de send instrcciones es : %d",fd_cpu);
		//log_error(logger,"El tam recibido por parametro a enviar es: %d",tam);

		while(indice < cant_instrucciones){
			//log_warning(logger,"El id de las intrucciones en send instrucciones es: %d",a->id);
			a = list_get(pcb_proceso->instrucciones,indice);

			//printf("[SEND INSTRUCCIONES KERNEL A CPU] la instruccion es: %s \n",a->nombre);

			if(a->id == NO_OP){
				//log_warning(logger,"entre en NO_OP dentro de send_instrucciones");
				send_NO_OP(fd_cpu,a->parametro1);

			}
			else if(a->id == IO){
				//log_warning(logger,"entre en I/O dentro de send_instrucciones");
				//printf( "el parametro 1  de IO es: %d\n",a->parametro1);
				send_IO(fd_cpu,a->parametro1);
			}
			else if(a->id == READ){
				//log_warning(logger,"entre en READ dentro de send_instrucciones");
				send_READ(fd_cpu,a->parametro1);
			}
			else if(a->id == COPY){
				//log_warning(logger,"entre en COPY dentro de send_instrucciones");
				send_COPY(fd_cpu,a->parametro1,a->parametro2);

			}
			else if(a->id == WRITE){
				//log_warning(logger,"entre en WRITE dentro de send_instrucciones");
				send_WRITE(fd_cpu,a->parametro1,a->parametro2);
			}
			else if(a->id == EXIT || a->nombre == "EXIT" ){
				//log_warning(logger,"Entre en EXIT dentro de send_instrucciones");
				send_EXIT(fd_cpu);
			}

			indice++;
		}

}


