#include "../include/comunicacion_kernel.h"
#include "../include/kernel.h"
#include "protocolo.h"
#include "../include/funciones_kernel.h"



typedef struct {
    t_log* log;
    int fd;
    char* server_name;
} t_procesar_conexion_args;

static void procesar_conexion_kernel(void* void_args) {

    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* log_kernel = args->log;
    int cliente_socket = args->fd;
    char* server_name = args->server_name;
    free(args);


    uint32_t tam;
    if (recv(cliente_socket, &tam, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
	log_info(log_kernel, "DISCONNECT!");


	return;
    }

    tam_proceso = tam;


    op_code_instrucciones cop;
    t_list* lista_intrucciones_1 = list_create();
    while (cliente_socket != -1) {

    	//log_error(log_kernel,"El cliente socket es: %d",cliente_socket);

        if (recv(cliente_socket, &cop, sizeof(op_code_instrucciones), 0) != sizeof(op_code_instrucciones)) {
        	contador_cliente++;



        	//Una vez que la consola nos da todas las instrucciones, aca abajo cargamos estas instrucciones en el pcb y se lo enviamos a cpu



        	pedir_tabla_a_memoria();

        	send_TAM(fd_memoria,tam_proceso);

        	if(mensaje_unico_memoria == 1){
        		uint32_t aux1;
        		       	 if(!recv_interrupcion(fd_memoria,&aux1)){
        		       	 		log_error(log_kernel, "Fallo recibiendo aux");
        		       	 		return;
        		       	 	}

        		       	// 	log_warning(log_cpu,"El tam de pags es %d",tam_paginas);
        		       	uint32_t aux2;
        		       	 	if(!recv_interrupcion(fd_memoria,&aux2)){
        		       	 		 log_error(log_kernel, "Fallo recibiendo aux");
        		       	 		 return;
        		       	 	 }
        		       	 	mensaje_unico_memoria = 0;
        	}




        	uint32_t indice_tabla;
        	if (!recv_indice_a_kernel(fd_memoria, &indice_tabla)) {//puede ser fd_memoria en vez de cliente socket
        	            	     log_error(log_kernel, "Fallo recibiendo indice tabla");
        	            	     break;
        	}
        	log_trace(log_kernel,"El indice tabla de pagina es: %d",indice_tabla);

//////////////////////////////////////////////////////////////////////////////////////////////

        	pcb_t* pcb_proceso = malloc(sizeof(pcb_t));


        	pcb_proceso->PID = contador_cliente;
        	pcb_proceso->tamanio = tam;
        	pcb_proceso->instrucciones = lista_intrucciones_1;
        	pcb_proceso->PC = 0;//contador_instruccion;//arranca desde la instruccion 0
        	pcb_proceso->indice_tabla_paginas = indice_tabla;//esta hardcodeado pero hay que cambiarlo, con una funcion que se lo pida a memoria
        	pcb_proceso->estimacionRafaga = estimacion_inicial;
        	pcb_proceso->alpha = alfa;
        	pcb_proceso->estado = NEW;

        	pcb_proceso->rafagaAnterior;
        	pcb_proceso->estimacionActual;
        	pcb_proceso->estimacionAnterior;
        	pcb_proceso->horaDeIngresoAExe;
        	pcb_proceso-> suspendido = false;
        	pcb_proceso->tiempo_bloqueo = 0;


        	agregarANew(pcb_proceso);
        	//log_warning(log_kernel,"pase el agregar a NEW");



        	//log_trace(log_kernel,"El PID ES: %d",contador_cliente);
        	log_info(log_kernel, "DISCONNECT!");

        	//log_trace(log_kernel,"El socket de cpu despues de grar conexiones es: %d",fd_cpu);


        	break;
        }

      //  log_warning(log_kernel,"El codigo de operacion despues del recv es: %d",cop);

        switch (cop) {
            case NO_OP:{

            	 uint32_t parametro1;

            	 if (!recv_NO_OP_2(cliente_socket, &parametro1)) {
            	          log_error(log_kernel, "Fallo recibiendo NO_OP");
            	         break;
            	      }

         //  log_warning(log_kernel, "Deserialice NO_OP el parametro es: %d",parametro1);
           //cargar_instruccion(NO_OP,"NO_OP",parametro1,NULL);
           cargar_instruccion2(NO_OP,"NO_OP",parametro1,(uint32_t)NULL,lista_intrucciones_1);

               // log_info(log_kernel, "entre al case NO_OP");
                break;
            }
            case IO:
            {
            	uint32_t parametro1;

            	if (!recv_IO(cliente_socket, &parametro1)) {
            	     log_error(log_kernel, "Fallo recibiendo IO");
            	     break;
            	}
            	cargar_instruccion2(IO,"I\O",parametro1,(uint32_t)NULL,lista_intrucciones_1);
            	//log_warning(log_kernel, "Deserialice IO el parametro es: %d",parametro1);
            	//log_info(log_kernel, "entre a IO");


            	break;
            }
            case READ:
            {
            	uint32_t parametro1;
            	if (!recv_READ(cliente_socket, &parametro1)) {
        	     log_error(log_kernel, "Fallo recibiendo READ");
        	     break;
            	}
            	cargar_instruccion2(READ,"READ",parametro1,(uint32_t)NULL,lista_intrucciones_1);
        //	log_warning(log_kernel, "Deserialice READ el parametro es: %d",parametro1);
        	//log_info(log_kernel, "entre a IO");
        	break;
			}
            case COPY:
            {
            	uint32_t parametro1, parametro2;
				if (!recv_COPY(cliente_socket, &parametro1, &parametro2)) {
				   log_error(logger, "Fallo recibiendo COPY");
				   break;
				}
				cargar_instruccion2(COPY,"COPY",parametro1,parametro2,lista_intrucciones_1);
				//log_warning(log_kernel, "Deserialice COPY el parametro1 es: %d",parametro1);
				//log_warning(log_kernel, "Deserialice COPY el parametro2 es: %d",parametro2);
				break;
           	}
            case WRITE:
            {
            	uint32_t parametro1, parametro2;
            	if (!recv_WRITE(cliente_socket, &parametro1, &parametro2)) {
            	   log_error(logger, "Fallo recibiendo WRITE");
            	   break;
            	}
            	cargar_instruccion2(WRITE,"WRITE",parametro1,parametro2,lista_intrucciones_1);
            	//log_warning(log_kernel, "Deserialice WRITE el parametro1 es: %d",parametro1);
            	//log_warning(log_kernel, "Deserialice WRITE el parametro2 es: %d",parametro2);
            	break;
			}
            case EXIT:
            {
            	//cargar_instruccion(EXIT,"EXIT",NULL,NULL);
            	cargar_instruccion2(EXIT,"EXIT",(uint32_t)NULL,(uint32_t)NULL,lista_intrucciones_1);
            	//log_warning(log_kernel, "Entre en EXIT");

				break;
			}

            // Errores
            case -1:
                log_error(log_kernel, "Cliente desconectado de %s...", server_name);
                return;
               // break;
            default:
                log_error(log_kernel, "Algo anduvo mal en el server de %s", server_name);
                log_info(log_kernel, "Cop: %d", cop);
                return;
        }
    }
    log_warning(log_kernel, "El cliente se desconecto de %s server", server_name);


    return;
}


int server_escuchar_kernel(t_log* logger, char* server_name, int server_socket) {
    int cliente_socket = esperar_cliente(logger, server_name, server_socket);

    if (cliente_socket != -1) {
        pthread_t hilo;
        t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
        args->log = logger;
        args->fd = cliente_socket;
        args->server_name = server_name;
       // log_error(log_kernel,"Estoy en sv escuchar antes de procesar conexion");
        pthread_create(&hilo, NULL, (void*) procesar_conexion_kernel, (void*) args);
        pthread_detach(hilo);
       // log_error(log_kernel,"Estoy en sv escuchar despues de procesar conexion");
        return 1;
    }
    return 0;
}
