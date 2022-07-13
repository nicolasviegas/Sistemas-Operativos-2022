#include "../cpu/include/cpu.h"


int obtener_algoritmo(char* algoritmo_char){

	 t_algoritmo_reemplazo_tlb switcher;

	    if (strcmp(algoritmo_char,"FIFO") == 0)
	    {
	        switcher = FIFO;
	       // log_info(log_cpu, "El algoritmo de reemplazo elegido es FIFO.");
	    }


	    if (strcmp(algoritmo_char,"LRU") == 0)
	    {
	        switcher = LRU;
	       // log_info(log_cpu, "El algoritmo de reemplazo elegido es LRU.");
	    }
	    return switcher;
}

void inicializar_config(){
	  ip = config_get_string_value(config_cpu,"IP_MEMORIA");// esto no se si va
	  puerto_escucha_dispatch = config_get_string_value(config_cpu,"PUERTO_ESCUCHA_DISPATCH");
	  puerto_interrupt = config_get_string_value(config_cpu,"PUERTO_ESCUCHA_INTERRUPT");
	  ip_memoria = config_get_string_value(config_cpu,"IP_MEMORIA");
	  puerto_memoria = config_get_string_value(config_cpu,"PUERTO_MEMORIA");

	  entradas_tlb = config_get_int_value(config_cpu,"ENTRADAS_TLB");
	  log_info(log_cpu,"La cantidad de entradas de la TLB son: %d \n",entradas_tlb);
	  reemplazo_tlb = config_get_string_value(config_cpu,"REEMPLAZO_TLB");
	  log_info(log_cpu,"El algoritmo d reemplazo de la TLB es: %s \n",reemplazo_tlb);
	  //char* reemplazo_tlb = config_get_string_value(config_cpu,"REEMPLAZO_TLB");
	  algoritmo_config = obtener_algoritmo(reemplazo_tlb);
	  retardo_noop = config_get_int_value(config_cpu,"RETARDO_NOOP");
	  log_info(log_cpu,"El retardo NO_OP es: %d \n",retardo_noop);

}



void cerrar_programa3(t_log* logger) {
    log_destroy(logger);
    config_destroy(config_cpu);
    list_destroy_and_destroy_elements(lista_instrucciones_cpu,free);
    list_destroy_and_destroy_elements(lista_pcb_cpu,free);
    list_destroy_and_destroy_elements(lista_tlb,free);


    close(fd_cpu);
}


void sighandler(int x){
	cerrar_programa3(log_cpu);
	exit(EXIT_SUCCESS);
}


int main() {
    signal(SIGINT, sighandler);



    log_cpu = log_create("cpu.log","cpu",1,LOG_LEVEL_TRACE);

    config_cpu = config_create("cpu.config");


    inicializar_config();

    lista_instrucciones_cpu = list_create();
    lista_pcb_cpu = list_create();
    lista_tlb = list_create();

    ultimo_proceso_cpu = 1 ;

    //fd_kernel = iniciar_servidor(log_cpu,"CPU",ip,puerto_escucha);
    fd_cpu = iniciar_servidor(log_cpu,"CPU",ip,puerto_escucha_dispatch);

    fd_cpu_interrupt = iniciar_servidor(log_cpu,"CPU INTERRUPT",ip,puerto_interrupt);



    //log_trace(log_cpu,"El socket en cpu.c es : %d",fd_cpu);



    fd_memoria=0;
    if (!generar_conexion_cpu_a_memoria(log_cpu, ip_memoria, puerto_memoria, &fd_memoria)) {
       			cerrar_programa3(log_cpu);
       			return EXIT_FAILURE;
       		}


//	log_trace(log_cpu,"El tamaño de las paginas es %d", tam_paginas);
 //  log_trace(log_cpu,"El fd_memoria despues de grar conexiones es: %d",fd_memoria);


    //conexion entre cpu (Servidor) y kernel(cliente)

    //  while(server_escuchar_cpu(log_cpu,"CPU",fd_kernel));
    while(server_escuchar_cpu(log_cpu,"CPU",fd_cpu,fd_cpu_interrupt));

  //  log_warning(log_cpu,"Despues de server escuchar");

    cerrar_programa3(log_cpu); //hay que agregar listas a destruir, el config y otras cosas

    return EXIT_SUCCESS;
}
