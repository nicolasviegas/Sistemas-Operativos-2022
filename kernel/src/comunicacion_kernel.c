#include "../include/comunicacion_kernel.h"
#include "../include/kernel.h"
#include "../include/funciones_kernel.h"


typedef struct {
    t_log* log;
    int fd;
    char* server_name;
} t_procesar_conexion_args;

static void procesar_conexion(void* void_args) {
    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* log_kernel = args->log;
    int cliente_socket = args->fd;
    char* server_name = args->server_name;
    free(args);

    //log_error(log_kernel,"El cliente es: %s",server_name);

    op_code_instrucciones cop;
    while (cliente_socket != -1) {

    	//log_warning(log_kernel,"El codigo de operacion es: %d",cop);

        if (recv(cliente_socket, &cop, sizeof(op_code_instrucciones), 0) != sizeof(op_code_instrucciones)) {

        	//printf("El socket en procesar conexion es: %d \n",cliente_socket);

        	//printf("El tam de op_code_instrucciones es %d\n", sizeof(op_code_instrucciones));

        	//log_error(log_kernel,"Elde recv vuelve %d",recv(cliente_socket, &cop, sizeof(op_code_instrucciones), 0));

        	log_info(log_kernel, "DISCONNECT!");
            return;
        	//break;
        }

      //  log_warning(log_kernel,"El codigo de operacion despues del recv es: %d",cop);

        switch (cop) {
            case NO_OP:{

            	 uint32_t parametro1;

            	 if (!recv_NO_OP_2(cliente_socket, &parametro1)) {
            	          log_error(log_kernel, "Fallo recibiendo NO_OP");
            	         break;
            	      }

           log_warning(log_kernel, "Deserialice NO_OP el parametro es: %d",parametro1);
            	 cargar_instruccion(NO_OP,"NO_OP",parametro1,NULL);


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
            	cargar_instruccion(IO,"I\O",parametro1,NULL);
            	log_warning(log_kernel, "Deserialice IO el parametro es: %d",parametro1);
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
            	cargar_instruccion(READ,"READ",parametro1,NULL);
        	log_warning(log_kernel, "Deserialice READ el parametro es: %d",parametro1);
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

				 cargar_instruccion(COPY,"COPY",parametro1,parametro2);
				log_warning(log_kernel, "Deserialice COPY el parametro1 es: %d",parametro1);
				//log_info(log_kernel, "Deserialice COPY el parametro2 es: %d",parametro2);
				break;
           	}
            case WRITE:
            {
            	uint32_t parametro1, parametro2;

            	if (!recv_WRITE(cliente_socket, &parametro1, &parametro2)) {
            	   log_error(logger, "Fallo recibiendo WRITE");
            	   break;
            	}

            	cargar_instruccion(READ,"READ",parametro1,parametro2);
            	log_warning(log_kernel, "Deserialice WRITE el parametro1 es: %d",parametro1);
            	//log_info(log_kernel, "Deserialice WRITE el parametro2 es: %d",parametro2);
            	break;
			}
            case EXIT:
            {
            	cargar_instruccion(EXIT,"EXIT",NULL,NULL);
            	log_warning(log_kernel, "Entre en EXIT");

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


    	/*instrucciones* a = malloc(sizeof(instrucciones));
    	a = list_get(lista_instrucciones_kernel,0);
    	log_trace(log_kernel,"ID de la primer operacion: %d",a->id);
    	log_trace(log_kernel,"nombre de la primer operacion: %s",a->nombre);
    	log_trace(log_kernel,"PARAMETRO 1 de la primer operacion: %d",a->parametro1);
    	log_trace(log_kernel,"PARAMETRO 2  de la primer operacion: %d",a->parametro2);
    	free(a);*/

    	//log_error(log_kernel,"------------------------------------------------------");
    }

    log_warning(log_kernel, "El cliente se desconecto de %s server", server_name);
    return;
}

//void recv_TAM2(int fd, uint32_t* tam){
//	if (!recv_NO_OP_2(fd, &tam)) {
//				  log_error(log_kernel, "Fallo recibiendo NO_OP");
//				  return;
//	}
//
//	log_warning(log_kernel,"Deserialice el tam: %d",tam);
//	return;
//}

int server_escuchar(t_log* logger, char* server_name, int server_socket) {
    int cliente_socket = esperar_cliente(logger, server_name, server_socket);

    if (cliente_socket != -1) {
        pthread_t hilo;
        t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
        args->log = logger;
        args->fd = cliente_socket;
        args->server_name = server_name;
        log_error(log_kernel,"Estoy en sv escuchar antes de procesar conexion");
        pthread_create(&hilo, NULL, (void*) procesar_conexion, (void*) args);
        pthread_detach(hilo);
        log_error(log_kernel,"Estoy en sv escuchar despues de procesar conexion");
        return 1;
    }
    return 0;
}
