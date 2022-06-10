#include "../include/comunicacion_memoria.h"
//#include "../include/cpu.h"
//#include "../include/funciones_cpu.h"


static void procesar_conexion_memoria(void* void_args) {

    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* log_kernel = args->log;
    int cliente_socket = args->fd;
    char* server_name = args->server_name;
    free(args);

//    send_TAM(fd_cpu,tamanio_paginas);




    while (cliente_socket != -1) {

    	//log_trace(log_memoria,"Estoy dentro del while cliente socket");

//    		uint32_t condicion;
//    	    if (recv(cliente_socket, &condicion, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
//    		log_info(log_kernel, "DISCONNECT!");
//    		return;
//    	    }
//
//    	     send_indice_a_kernel(cliente_socket,indice_tabla);
//
//    	     log_trace(log_memoria,"envie el inice a kernel %d", indice_tabla);
    		uint32_t numero_pagina;
//    		uint32_t tabla_pagina_1er_nivel;
//    		log_warning(log_kernel,"antes del recv");
//    		if(!recv_numero_pagina(cliente_socket,numero_pagina)){
//				 log_error(log_kernel, "Fallo recibiendo el numero de pag");
//				break;
//           }
//    		recv_tabla_primer_nivel_pcb(fd_cpu,tabla_pagina_1er_nivel);





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
