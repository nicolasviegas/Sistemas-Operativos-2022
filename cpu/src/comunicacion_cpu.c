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
    t_log* log_kernel = args->log;
    int cliente_socket = args->fd;
    char* server_name = args->server_name;
    free(args);


	log_warning(log_cpu,"Entre en procesar conexion cpu");


	 uint32_t tam;

	     if (recv(cliente_socket, &tam, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
	 	log_info(log_cpu, "DISCONNECT!");


	 	return;
	     }
	     log_warning(log_cpu,"El tam despues del recv es: %d",tam);

	 while (cliente_socket != -1) {

		    	//log_warning(log_kernel,"El codigo de operacion es: %d",cop);

//		        if (recv(cliente_socket, &tam, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
//
//		        	log_info(log_cpu, "DISCONNECT!");
//
//		        	log_trace(log_cpu,"El socket de cpu despues de grar conexiones es: %d",fd_cpu);
//
//
//		           return;
//		        	//break;
//		        }

		      //  log_warning(log_kernel,"El codigo de operacion despues del recv es: %d",cop);

//		        switch (cop) {
//		            case NO_OP:{
//
//		            	 uint32_t parametro1;
//
//		            	 if (!recv_NO_OP_2(cliente_socket, &parametro1)) {
//		            	          log_error(log_kernel, "Fallo recibiendo NO_OP");
//		            	         break;
//		            	      }
//
//		           log_warning(log_kernel, "Deserialice NO_OP el parametro es: %d",parametro1);
//		            	 cargar_instruccion(NO_OP,"NO_OP",parametro1,NULL);
//
//
//		               // log_info(log_kernel, "entre al case NO_OP");
//		                break;
//		            }
//		            case IO:
//		            {
//		            	uint32_t parametro1;
//
//		            	if (!recv_IO(cliente_socket, &parametro1)) {
//		            	     log_error(log_kernel, "Fallo recibiendo IO");
//		            	     break;
//		            	}
//		            	cargar_instruccion(IO,"I\O",parametro1,NULL);
//		            	log_warning(log_kernel, "Deserialice IO el parametro es: %d",parametro1);
//		            	//log_info(log_kernel, "entre a IO");
//
//
//		            	break;
//		            }
//		            case READ:
//		            {
//		            	uint32_t parametro1;
//
//		            	if (!recv_READ(cliente_socket, &parametro1)) {
//		        	     log_error(log_kernel, "Fallo recibiendo READ");
//		        	     break;
//		        	}
//		            	cargar_instruccion(READ,"READ",parametro1,NULL);
//		        	log_warning(log_kernel, "Deserialice READ el parametro es: %d",parametro1);
//		        	//log_info(log_kernel, "entre a IO");
//		        	break;
//					}
//		            case COPY:
//		            {
//		            	uint32_t parametro1, parametro2;
//
//						if (!recv_COPY(cliente_socket, &parametro1, &parametro2)) {
//						   log_error(logger, "Fallo recibiendo COPY");
//						   break;
//						}
//
//						 cargar_instruccion(COPY,"COPY",parametro1,parametro2);
//						log_warning(log_kernel, "Deserialice COPY el parametro1 es: %d",parametro1);
//						log_warning(log_kernel, "Deserialice COPY el parametro2 es: %d",parametro2);
//						break;
//		           	}
//		            case WRITE:
//		            {
//		            	uint32_t parametro1, parametro2;
//
//		            	if (!recv_WRITE(cliente_socket, &parametro1, &parametro2)) {
//		            	   log_error(logger, "Fallo recibiendo WRITE");
//		            	   break;
//		            	}
//
//		            	cargar_instruccion(READ,"READ",parametro1,parametro2);
//		            	log_warning(log_kernel, "Deserialice WRITE el parametro1 es: %d",parametro1);
//		            	log_warning(log_kernel, "Deserialice WRITE el parametro2 es: %d",parametro2);
//		            	break;
//					}
//		            case EXIT:
//		            {
//		            	cargar_instruccion(EXIT,"EXIT",NULL,NULL);
//		            	log_warning(log_kernel, "Entre en EXIT");
//
//						break;
//					}
//
//		            // Errores
//		            case -1:
//		                log_error(log_kernel, "Cliente desconectado de %s...", server_name);
//		                return;
//		               // break;
//		            default:
//		                log_error(log_kernel, "Algo anduvo mal en el server de %s", server_name);
//		                log_info(log_kernel, "Cop: %d", cop);
//		                return;
//		        }

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
