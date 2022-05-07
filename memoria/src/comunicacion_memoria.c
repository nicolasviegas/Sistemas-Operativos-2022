#include "../include/comunicacion_memoria.h"
//#include "../include/cpu.h"
//#include "../include/funciones_cpu.h"


static void procesar_conexion_memoria(void* void_args) {

    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* log_kernel = args->log;
    int cliente_socket = args->fd;
    char* server_name = args->server_name;
    free(args);




    while (cliente_socket != -1) {

    	//log_trace(log_memoria,"Estoy dentro del while cliente socket");


    	// log_trace(log_memoria,"antes de enviar el inice a kernel");


    	     // send_indice_a_kernel(fd_kernel,indice_tabla);


    	    //  log_trace(log_memoria,"envie el inice a kernel %d", indice_tabla);

    	 // break; //borrar despues esto es solo para que no me itere ahora
    }

    log_warning(log_memoria, "El cliente se desconecto de %s server", server_name);


    return;
}



int server_escuchar_memoria(t_log* logger, char* server_name, int server_socket) {
    int cliente_socket = esperar_cliente(logger, server_name, server_socket);

    if (cliente_socket != -1) {
        pthread_t hilo;
        t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
        args->log = logger;
        args->fd = cliente_socket;
        args->server_name = server_name;
       // log_error(log_kernel,"Estoy en sv escuchar antes de procesar conexion");
        pthread_create(&hilo, NULL, (void*) procesar_conexion_memoria, (void*) args);
        pthread_detach(hilo);
       // log_error(log_kernel,"Estoy en sv escuchar despues de procesar conexion");
        return 1;
    }
    return 0;
}
