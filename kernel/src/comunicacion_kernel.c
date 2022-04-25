#include "../include/comunicacion_kernel.h"


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

    op_code_instrucciones cop;
    while (cliente_socket != -1) {

        if (recv(cliente_socket, &cop, sizeof(op_code_instrucciones), 0) != sizeof(op_code_instrucciones)) {

            printf("El tam de op_code_instrucciones es %d\n", sizeof(op_code_instrucciones));
        	log_error(log_kernel,"Elde recv vuelve %d",recv(cliente_socket, &cop, sizeof(op_code_instrucciones), 0));
        	log_info(log_kernel, "DISCONNECT!");
            return;
        }

        switch (cop) {
            case NO_OP:
                log_info(log_kernel, "entre a NO_OP");
                break;

            case IO:
            {
            	log_info(log_kernel, "entre a IO");
            	break;
            }
            case READ:
            {
				log_info(log_kernel, "entre a READ");
				break;
			}
            case COPY:
            {
        	   	 log_info(log_kernel, "entre a COPY");
           		break;
           	}
            case WRITE:
            {
            	log_info(log_kernel, "entre a WRITE");
            	break;
			}
            case EXIT:
            {
            	log_info(log_kernel, "entre a EXIT");
				break;
			}



            // Errores
            case -1:
                log_error(log_kernel, "Cliente desconectado de %s...", server_name);
                return;
            default:
                log_error(log_kernel, "Algo anduvo mal en el server de %s", server_name);
                log_info(log_kernel, "Cop: %d", cop);
                return;
        }
    }

    log_warning(log_kernel, "El cliente se desconecto de %s server", server_name);
    return;
}

int server_escuchar(t_log* logger, char* server_name, int server_socket) {
    int cliente_socket = esperar_cliente(logger, server_name, server_socket);

    if (cliente_socket != -1) {
        pthread_t hilo;
        t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
        args->log = logger;
        args->fd = cliente_socket;
        args->server_name = server_name;
        pthread_create(&hilo, NULL, (void*) procesar_conexion, (void*) args);
        pthread_detach(hilo);
        return 1;
    }
    return 0;
}
