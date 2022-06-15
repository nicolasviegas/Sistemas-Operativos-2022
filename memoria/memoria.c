//#include "../memoria/include/memoria.h"
#include "include/memoria.h"

int obtener_algoritmo(char* algoritmo_char){

	 t_algoritmo_memoria switcher;



	    if (strcmp(algoritmo_char,"CLOCK") == 0)
	    {
	        switcher = CLOCK;
	        log_info(log_memoria, "El algoritmo de reemplazo elegido es CLOCK.");
	    }


	    if (strcmp(algoritmo_char,"CLOCK-M") == 0)
	    {
	        switcher = CLOCK_M;
	        log_info(log_memoria, "El algoritmo de reemplazo elegido es CLOCK-M.");
	    }
	    return switcher;
}


void inicializar_config(){
	  ///ip = config_get_string_value(config_memoria,"IP_MEMORIA");// esto no se si va
	  puerto_escucha = config_get_string_value(config_memoria,"PUERTO_ESCUCHA");

	  tamanio_memoria = config_get_int_value(config_memoria,"TAM_MEMORIA");
	  tamanio_paginas = config_get_int_value(config_memoria,"TAM_PAGINA");
	  cant_entradas_por_tabla = config_get_int_value(config_memoria,"ENTRADAS_POR_TABLA");
	  retardo_memoria = config_get_int_value(config_memoria,"RETARDO_MEMORIA");
	  retardo_swap = config_get_int_value(config_memoria,"RETARDO_SWAP");
	  char* algoritmo_char = config_get_string_value(config_memoria,"ALGORITMO_REEMPLAZO");
	  algoritmo_config = obtener_algoritmo(algoritmo_char);
	  marcos_por_proceso = config_get_int_value(config_memoria,"MARCOS_POR_PROCESO");
	  path_swap = config_get_string_value(config_memoria,"PATH_SWAP");

}

void inicializar_listas(){
	lista_tablas_1er_nivel = list_create();
	lista_tablas_2do_nivel = list_create();
	lista_frames = list_create();
	cargar_lista_frames();

	pthread_mutex_init(&mutexSwap, NULL);

}


void cerrar_programa5(t_log* logger) {
    log_destroy(logger);
    list_clean_and_destroy_elements(lista_tablas_1er_nivel,free);
    list_clean_and_destroy_elements(lista_tablas_2do_nivel,free);
}
//void sighandler(int s) {
//    cerrar_programa(logger);
//    exit(0);
//}




int main() {

    char* ip;



    //signal(SIGINT, sighandler);

    log_memoria = log_create("memoria.log","memoria",1,LOG_LEVEL_TRACE);

    config_memoria = config_create("memoria.config");

    ip = "127.0.0.1"; //config_get_string_value(config_memoria,"IP_MEMORIA"); /////////////////////////BORRAR
    inicializar_config();
    inicializar_listas();


    fd_memoria = iniciar_servidor(log_memoria,"MEMORIA",ip,puerto_escucha);


    //log_trace(log_memoria,"El socket de memoria es : %d",fd_memoria);


    //

    //conexion entre cpu (Servidor) y kernel(cliente)
    while(server_escuchar_memoria(log_memoria,"MEMORIA",fd_memoria));


  //  log_warning(log_cpu,"Despues de server escuchar");

    cerrar_programa5(log_memoria);

    return 0;
}
