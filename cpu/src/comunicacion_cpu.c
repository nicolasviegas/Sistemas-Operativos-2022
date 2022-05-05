#include "../include/comunicacion_cpu.h"
#include "../include/cpu.h"
//#include "../include/kernel.h"
//#include "../include/funciones_kernel.h"


typedef struct {
    t_log* log;
    int fd;
    char* server_name;
} t_procesar_conexion_args;

static void procesar_conexion_cpu(void* void_args) {

    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* log_cpu = args->log;
    int cliente_socket = args->fd;
    char* server_name = args->server_name;
    free(args);


	log_warning(log_cpu,"Entre en procesar conexion cpu");

	 op_code_instrucciones cop;
	 uint32_t pid;


	 while (cliente_socket != -1) {

		 	 if (!recv_pid_to_cpu(cliente_socket, &pid)) {
		 	log_error(log_cpu, "Fallo recibiendo pid");
		 	break;
		 	}
		     log_warning(log_cpu,"El pid despues del recv es: %d",pid);


		     if (recv(cliente_socket, &cop, sizeof(op_code_instrucciones), 0) != sizeof(op_code_instrucciones)) {

		    	 pcb_cpu* pcb_proceso_cpu = malloc(sizeof(pcb_cpu));


		    	 pcb_proceso_cpu->PID = pid;
		    	 pcb_proceso_cpu->instrucciones = lista_instrucciones_cpu;
		    	 pcb_proceso_cpu->PC = contador_instrucciones;
		    	 pcb_proceso_cpu->tabla_paginas; //faltan cargar
		    	 pcb_proceso_cpu->tamanio;//faltan cargar

		    	 list_add(lista_pcb_cpu,pcb_proceso_cpu);
		    	 free(pcb_proceso_cpu);

		    	 log_info(log_cpu, "DISCONNECT!");

		        	log_trace(log_cpu,"El socket de cpu despues de grar conexiones es: %d",fd_cpu);


		           return;
		     }
		        log_warning(log_cpu,"El codigo de operacion despues del recv es: %d",cop);

		        switch (cop) {
		        contador_instrucciones = 0; //esta variable es para ir modificando el PC
		            case NO_OP:{

		            	 uint32_t parametro1;

		            	 if (!recv_NO_OP_2(cliente_socket, &parametro1)) {
		            	          log_error(log_cpu, "Fallo recibiendo NO_OP");
		            	         break;

		            	      }

		           log_warning(log_cpu, "Deserialice NO_OP el parametro es: %d",parametro1);
		           //cargar_instruccion(NO_OP,"NO_OP",parametro1,NULL);


		               // log_info(log_cpu, "entre al case NO_OP");
		                break;
		            }
		            case IO:
		            {
		            	uint32_t parametro1;

		            	if (!recv_IO(cliente_socket, &parametro1)) {
		            	     log_error(log_cpu, "Fallo recibiendo IO");
		            	     break;
		            	}
		            	//cargar_instruccion(IO,"I\O",parametro1,NULL);
		            	log_warning(log_cpu, "Deserialice IO el parametro es: %d",parametro1);
		            	//log_info(log_cpu, "entre a IO");


		            	break;
		            }
		            case READ:
		            {
		            	uint32_t parametro1;

		            	if (!recv_READ(cliente_socket, &parametro1)) {
		        	     log_error(log_cpu, "Fallo recibiendo READ");
		        	     break;
		        	}
		            	//cargar_instruccion(READ,"READ",parametro1,NULL);

		            	log_warning(log_cpu, "Deserialice READ el parametro es: %d",parametro1);
		        	//log_info(log_cpu, "entre a IO");
		        	break;
					}
		            case COPY:
		            {
		            	uint32_t parametro1, parametro2;

						if (!recv_COPY(cliente_socket, &parametro1, &parametro2)) {
						   log_error(logger, "Fallo recibiendo COPY");
						   break;
						}

						 //cargar_instruccion(COPY,"COPY",parametro1,parametro2);
						log_warning(log_cpu, "Deserialice COPY el parametro1 es: %d",parametro1);
						log_warning(log_cpu, "Deserialice COPY el parametro2 es: %d",parametro2);
						break;
		           	}
		            case WRITE:
		            {
		            	uint32_t parametro1, parametro2;

		            	if (!recv_WRITE(cliente_socket, &parametro1, &parametro2)) {
		            	   log_error(logger, "Fallo recibiendo WRITE");
		            	   break;
		            	}

		            	//cargar_instruccion(READ,"READ",parametro1,parametro2);
		            	log_warning(log_cpu, "Deserialice WRITE el parametro1 es: %d",parametro1);
		            	log_warning(log_cpu, "Deserialice WRITE el parametro2 es: %d",parametro2);
		            	break;
					}
		            case EXIT:
		            {
		            	//cargar_instruccion(EXIT,"EXIT",NULL,NULL);
		            	log_warning(log_cpu, "Entre en EXIT");

						break;
					}

		            // Errores
		            case -1:
		                log_error(log_cpu, "Cliente desconectado de %s...", server_name);
		                return;
		               // break;
		            default:
		                log_error(log_cpu, "Algo anduvo mal en el server de %s", server_name);
		                log_info(log_cpu, "Cop: %d", cop);
		                return;
		        }

	 }



    log_warning(log_cpu, "El cliente se desconecto de %s server", server_name);


    return;
}

int server_escuchar_cpu(t_log* logger, char* server_name, int server_socket) {
    int cliente_socket = esperar_cliente(logger, server_name, server_socket);
   // log_warning(logger,"El socket en sv escuchar cpu es: %d",server_socket);
    if (cliente_socket != -1) {
        pthread_t hilo;
        t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
        args->log = logger;
        args->fd = cliente_socket;
        args->server_name = server_name;
       // log_error(log_cpu,"Estoy en sv escuchar antes de procesar conexion");
        pthread_create(&hilo, NULL, (void*) procesar_conexion_cpu, (void*) args);
        pthread_detach(hilo);
     //  log_error(log_cpu,"Estoy en sv escuchar despues de procesar conexion");
        return 1;
    }
    return 0;
}
