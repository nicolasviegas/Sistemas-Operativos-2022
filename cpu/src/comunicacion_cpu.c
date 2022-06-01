#include "../include/comunicacion_cpu.h"
#include "../include/cpu.h"
#include "../include/funciones_cpu.h"


typedef struct {
    t_log* log;
    int fd;
    int fd_interrupt;
    char* server_name;
} t_procesar_conexion_args;

static void procesar_conexion_cpu(void* void_args) {

    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* log_cpu = args->log;
    int cliente_socket = args->fd;
    int cliente_socket_interrupcion = args->fd_interrupt;
    char* server_name = args->server_name;
    free(args);


	 uint32_t pid;
	 uint32_t tam;
	 uint32_t cant_instrucciones = 0;
	 uint32_t indice_tabla;
	 op_code_instrucciones co_op;
	 uint32_t pc;

	 //////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// log_trace(log_cpu,"El socket de kernel en cpu.c es : %d",fd_kernel);
	 printf("El cliente socket en cpu.c procesar conexion es : %d\n",cliente_socket);
	 fd_kernel = cliente_socket;/////////////////////////////////////////////todo HARDCODEADO, PREGUNTAR
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	 while (cliente_socket != -1 && cliente_socket_interrupcion != -1) {

		 if (!recv_pid_to_cpu(cliente_socket, &pid)) {
		 		log_error(log_cpu, "Fallo recibiendo pid");
		 		break;
		 }
		 log_trace(log_cpu,"El pid despues del recv es: %d",pid);


		 if (recv(cliente_socket, &tam, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
		 log_info(log_cpu, "DISCONNECT!");
		 		return;
		 }
		 log_trace(log_cpu,"El tam despues del recv es: %d",tam);

		 if (!recv_cant_instrucciones(cliente_socket, &cant_instrucciones)) {
		 	 	log_error(log_cpu, "Fallo recibiendo cant_instrucciones");

		 	 }
		 log_trace(log_cpu,"cant_instrucciones despues del recv es: %d",cant_instrucciones);

		 if (!recv_indice_tabla_paginas_a_cpu(cliente_socket, &indice_tabla)) {//FALTA HACER LA PARTE DE MEMORIA
				 log_error(log_cpu, "Fallo recibiendo indice tabla paginas");

			}
		 log_trace(log_cpu,"indice tabla pags despues del recv es: %d",indice_tabla);


		for(int i = 0; i<cant_instrucciones;i++){
			if (recv(cliente_socket, &co_op, sizeof(op_code_instrucciones), 0) != sizeof(op_code_instrucciones)) {



					     //log_trace(log_cpu,"El PID ES: %d",contador_cliente);
					     log_info(log_cpu, "DISCONNECT!");

					     log_trace(log_cpu,"El socket de cpu despues de grar conexiones es: %d",fd_cpu);

					    // return;
					    break;
					 }

					       //  log_warning(log_kernel,"El codigo de operacion despues del recv es: %d",cop);

					         switch (co_op) {
					             case NO_OP:{

					             	 uint32_t parametro1;

					             	 if (!recv_NO_OP_2(cliente_socket, &parametro1)) {
					             	          log_error(log_cpu, "Fallo recibiendo NO_OP");
					             	         break;
					             	      }

					            log_warning(log_cpu, "Deserialice NO_OP el parametro es: %d",parametro1);
					             	 cargar_instruccion_cpu(NO_OP,"NO_OP",parametro1,(uint32_t)NULL);


					                // log_info(log_kernel, "entre al case NO_OP");
					                 break;
					             }
					             case IO:
					             {
					             	uint32_t parametro1;

					             	if (!recv_IO(cliente_socket, &parametro1)) {
					             	     log_error(log_cpu, "Fallo recibiendo IO");
					             	     break;
					             	}
					             	cargar_instruccion_cpu(IO,"I\O",parametro1,(uint32_t)NULL);
					             	log_warning(log_cpu, "Deserialice IO el parametro es: %d",parametro1);
					             	//log_info(log_kernel, "entre a IO");


					             	break;
					             }
					             case READ:
					             {
					             	uint32_t parametro1;

					             	if (!recv_READ(cliente_socket, &parametro1)) {
					         	     log_error(log_cpu, "Fallo recibiendo READ");
					         	     break;
					         	}
					             	cargar_instruccion_cpu(READ,"READ",parametro1,(uint32_t)NULL);
					         	log_warning(log_cpu, "Deserialice READ el parametro es: %d",parametro1);
					         	//log_info(log_kernel, "entre a IO");
					         	break;
					 			}
					             case COPY:
					             {
					             	uint32_t parametro1, parametro2;

					 				if (!recv_COPY(cliente_socket, &parametro1, &parametro2)) {
					 				   log_error(log_cpu, "Fallo recibiendo COPY");
					 				   break;
					 				}

					 				cargar_instruccion_cpu(COPY,"COPY",parametro1,parametro2);
					 				log_warning(log_cpu, "Deserialice COPY el parametro1 es: %d",parametro1);
					 				log_warning(log_cpu, "Deserialice COPY el parametro2 es: %d",parametro2);
					 				break;
					            	}
					             case WRITE:
					             {
					             	uint32_t parametro1, parametro2;

					             	if (!recv_WRITE(cliente_socket, &parametro1, &parametro2)) {
					             	   log_error(log_cpu, "Fallo recibiendo WRITE");
					             	   break;
					             	}

					             	cargar_instruccion_cpu(READ,"READ",parametro1,parametro2);
					             	log_warning(log_cpu, "Deserialice WRITE el parametro1 es: %d",parametro1);
					             	log_warning(log_cpu, "Deserialice WRITE el parametro2 es: %d",parametro2);
					             	break;
					 			}
					             case EXIT:
					             {
					            	 cargar_instruccion_cpu(EXIT,"EXIT",(uint32_t)NULL,(uint32_t)NULL);
					             	log_warning(log_cpu, "Entre en EXIT");

					 				break;
					 			}

					             // Errores
					             case -1:
					                 log_error(log_cpu, "Cliente desconectado de %s...", server_name);
					                 //return;
					                 break;
					             default:
					                 log_error(log_cpu, "Algo anduvo mal en el server de %s", server_name);
					                 log_info(log_cpu, "Cop: %d", co_op);
					                 //return;
					                 break;
					         }
		}

		 if (recv(cliente_socket, &pc, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
			 log_info(log_cpu, "DISCONNECT!");
					return;
			 }
		 log_trace(log_cpu,"El PC despues del recv es: %d",pc);



		pcb_cpu* pcb_proceso_cpu = malloc(sizeof(pcb_cpu));


		pcb_proceso_cpu->PID = pid;
		pcb_proceso_cpu->instrucciones = lista_instrucciones_cpu;
		pcb_proceso_cpu->PC = pc;
		pcb_proceso_cpu->indice_tabla_paginas = indice_tabla;
		pcb_proceso_cpu->tamanio = tam;

		list_add(lista_pcb_cpu,pcb_proceso_cpu);

		lista_instrucciones_cpu = list_take_and_remove(lista_instrucciones_cpu,0);

		/*------------------------------------ACA COMIENZA EL CICLO DE EJECUCION----------------------*/
		instrucciones* proxima_a_ejecutar = malloc(sizeof(instrucciones)); //hacer un free al final

		tiempo_bloqueante = 0;

		interrupcion = false;

			while(!interrupcion && pcb_proceso_cpu->PC < list_size(pcb_proceso_cpu->instrucciones) && tiempo_bloqueante == 0){//la interrupcion verla segun el puerto interrupt

			log_trace(log_cpu,"Entre en el while de interrupcion");

			proxima_a_ejecutar = fetch(pcb_proceso_cpu);

			decode_and_execute(pcb_proceso_cpu, proxima_a_ejecutar);

			interrupcion = check_interrupt(cliente_socket_interrupcion);

			log_trace(log_cpu,"El pc despues de ejecutar una instruccion es: %d",pcb_proceso_cpu->PC);

			log_warning(log_cpu,"El tiempo bloqueante en el while de ejecucion es: %d",tiempo_bloqueante);

			}

		//log_warning(log_cpu, "el fd_cpu antes de send pc es: %d",fd_cpu);
		//log_warning(log_cpu, "el fd_kernel antes de send pc es: %d",fd_kernel);

		send_PC(fd_kernel,pcb_proceso_cpu->PC);

		//log_debug(log_cpu,"El tiempo bloqueante antes de mandarlo es: %d ",tiempo_bloqueante);

		send_tiempo_bloqueante(fd_kernel,tiempo_bloqueante);

		//log_trace(log_cpu,"Pase el send pc del PID: %d",pcb_proceso_cpu->PID );


		free(proxima_a_ejecutar);

		log_trace(log_cpu,"Finalizo la ejecucion del proceso: %d ",pcb_proceso_cpu->PID);

		free(pcb_proceso_cpu);

	 }



    log_warning(log_cpu, "El cliente se desconecto de %s server ", server_name);

}


int server_escuchar_cpu(t_log* logger, char* server_name, int server_socket,int server_socket_1) {
    int cliente_socket = esperar_cliente(logger, server_name, server_socket);
    int cliente_socket_1 = esperar_cliente(logger, server_name, server_socket_1);
   // log_warning(logger,"El socket en sv escuchar cpu es: %d",server_socket);
    if (cliente_socket != -1 && cliente_socket_1 != -1) {
    	/////////////////////////////////
        pthread_t hilo;
        t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
        args->log = logger;
        args->fd = cliente_socket;
        args->fd_interrupt = cliente_socket_1;
        args->server_name = server_name;

       // log_error(log_cpu,"Estoy en sv escuchar antes de procesar conexion");
        pthread_create(&hilo, NULL, (void*) procesar_conexion_cpu, (void*) args);
        pthread_detach(hilo);
     //  log_error(log_cpu,"Estoy en sv escuchar despues de procesar conexion");

        return 1;
    }
    return 0;
}
