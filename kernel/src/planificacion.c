#include "../include/planificacion.h"



//void cargar_PCB_kernel(int contador_cliente, uint32_t tam, t_list* lista_nueva_kernel, t_list* tabla_paginas, int estimacion_inicial, int alfa){
//	pcb_t* pcb_proceso = malloc(sizeof(pcb_t));
//
//	pcb_proceso->PID = contador_cliente;
//	pcb_proceso->tamanio = tam;
//	pcb_proceso->instrucciones = lista_nueva_kernel;
//	pcb_proceso->tabla_paginas = tabla_paginas;
//	pcb_proceso->estimacionRafaga = estimacion_inicial;
//	pcb_proceso->alpha = alfa;
//
//
//
//	//free(pcb_proceso);
//}
/*

void send_instrucciones_kernel_a_cpu(int fd_cpu,t_log* logger,pcb_t* pcb_proceso){
		instrucciones* a = malloc(sizeof(instrucciones));
		int cant_instrucciones = list_size(pcb_proceso->instrucciones);
		int indice = 0;

		log_error(logger,"El socket dentro de send instrcciones es : %d",fd_cpu);
		//log_error(logger,"El tam recibido por parametro a enviar es: %d",tam);




		while(indice < cant_instrucciones){
			a = list_get(pcb_proceso->instrucciones,indice);

			//log_warning(logger,"El id de las intrucciones en send instrucciones es: %d",a->id);

			if(a->id == NO_OP){
				log_warning(logger,"entre en NO_OP dentro de send_instrucciones");
				send_NO_OP(fd_cpu,a->parametro1);

			}
			else if(a->id == IO){
				log_warning(logger,"entre en I/O dentro de send_instrucciones");
				//printf( "el parametro 1  de IO es: %d\n",a->parametro1);
				send_IO(fd_cpu,a->parametro1);
			}
			else if(a->id == READ){
				log_warning(logger,"entre en READ dentro de send_instrucciones");
				send_READ(fd_cpu,a->parametro1);
			}
			else if(a->id == COPY){
				log_warning(logger,"entre en COPY dentro de send_instrucciones");
				send_COPY(fd_cpu,a->parametro1,a->parametro2);

			}
			else if(a->id == WRITE){
				log_warning(logger,"entre en WRITE dentro de send_instrucciones");
				send_WRITE(fd_cpu,a->parametro1,a->parametro2);
			}
			else if(a->id == EXIT){
				log_warning(logger,"Entre en EXIT dentro de send_instrucciones");
				send_EXIT(fd_cpu);
			}

			indice++;
		}

		free(a);

}


*/
