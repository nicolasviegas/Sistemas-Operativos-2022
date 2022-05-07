#include "../include/funciones_kernel.h"


void cargar_instruccion(int id, char* nombre, uint32_t parametro1, uint32_t parametro2){
	instrucciones* estructura_instrucciones = malloc(sizeof(instrucciones));

	estructura_instrucciones->id = id;
	estructura_instrucciones->nombre = nombre;
	estructura_instrucciones->parametro1 = parametro1;
	estructura_instrucciones->parametro2 = parametro2;

	list_add(lista_instrucciones_kernel,estructura_instrucciones);
	//free(estructura_instrucciones);
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

		//return fd_memoria;
}







