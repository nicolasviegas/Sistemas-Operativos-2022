#include "include/kernel.h"



/*
void cerrar_programa(t_log* logger) {
    log_destroy(logger);
}
void sighandler(int s) {
    cerrar_programa(logger);
    exit(0);
}
*/

void inicializar_semaforos(){

	pthread_mutex_init(&mutexPotencialesRetensores, NULL);
	pthread_mutex_init(&mutexBlockSuspended, NULL);
	pthread_mutex_init(&mutexReadySuspended, NULL);
	pthread_mutex_init(&mutexNew, NULL);
	pthread_mutex_init(&mutexReady, NULL);
	pthread_mutex_init(&mutexBlock, NULL);
	pthread_mutex_init(&mutexExe, NULL);
	pthread_mutex_init(&mutexExit, NULL);
	sem_init(&analizarSuspension, 0, 0);
	sem_init(&suspensionFinalizada, 0, 0);
	sem_init(&contadorNew, 0, 0); // Estado New
	sem_init(&contadorReady, 0, 0); // Estado Ready
	sem_init(&contadorExe, 0, 0); // Estado Exe
	sem_init(&contadorProcesosEnMemoria, 0, 0);    // Memoria IMP HAY QUE VER COMO SE INICIALIZA PORQUE ESTO AFECTA LA DISPONIBILIDAD DE LA COLA READY
	sem_init(&multiprogramacion, 0, grado_multiprogramacion); // Memoria
	pthread_mutex_init(&multiprocesamiento, NULL);
	sem_init(&contadorBlock, 0, 0);
	sem_init(&largoPlazo, 0, 1);
	sem_init(&contadorReadySuspended, 0, 0);
	sem_init(&medianoPlazo, 0, 1);


	sem_init(&hilo_sincro_cpu_kernel, 0, 0);
}

float obtener_float_de_config(t_config* config, char* key){

	double valor;

	if((valor = config_get_double_value(config, key)) < 0){

		perror("No se pudo obtener el valor del archivo de configuraciones!\n");
		abort();
	}

	return (float) valor;
}

void inicializar_config(){
	  ip = config_get_string_value(config_kernel,"IP_MEMORIA");// esto no se si va
	  puerto_escucha = config_get_string_value(config_kernel,"PUERTO_ESCUCHA");

	  ip_memoria = config_get_string_value(config_kernel,"IP_MEMORIA");
	  puerto_memoria = config_get_string_value(config_kernel,"PUERTO_MEMORIA");

	  ip_cpu = config_get_string_value(config_kernel,"IP_CPU");
	  puerto_cpu_dispatch = config_get_string_value(config_kernel,"PUERTO_CPU_DISPATCH");

	  char* algoritmo_char = config_get_string_value(config_kernel,"ALGORITMO_PLANIFICACION");
	  algoritmo_config = obtener_algoritmo(algoritmo_char);



	  estimacion_inicial = config_get_int_value(config_kernel,"ESTIMACION_INICIAL");
	  grado_multiprogramacion = config_get_int_value(config_kernel,"GRADO_MULTIPROGRAMACION");
	  log_warning(log_kernel,"El grado multi prog es: %d" ,grado_multiprogramacion);
	  alfa = obtener_float_de_config(config_kernel,"ALFA");
	  log_error(log_kernel,"El alfa es: %f",alfa);
	   tiempo_max_bloqueado = config_get_int_value(config_kernel,"TIEMPO_MAXIMO_BLOQUEADO");
}

void inicializar_listas(){


	listaPotencialesRetensores = list_create();
	colaNew = queue_create();
	colaReady = list_create();
	listaExe = list_create();
	listaBlock = list_create();
	listaExit = list_create();
	listaBlockSuspended = list_create();
	colaReadySuspended = queue_create();

	lista_instrucciones_kernel = list_create();
	lista_pcb_en_memoria = list_create();

}

void inicializar_planificacion(){

	pthread_create(&hiloQueDesuspende, NULL, (void*)hiloSuspensionAReady, NULL);
	pthread_create(&hiloMedianoPlazo, NULL, (void*)hiloBlockASuspension, NULL);
	pthread_detach(hiloQueDesuspende);
	pthread_detach(hiloMedianoPlazo);

	pthread_create(&hiloNewReady, NULL, (void*)hiloNew_Ready, NULL);
	pthread_create(&hiloReady_Exec, NULL, (void*)hiloReady_Exe, NULL);
	pthread_detach(hiloNewReady);
	pthread_detach(hiloReady_Exec);


}

void cerrar_programa2(t_log* logger) {
    log_destroy(logger);
}

int main() {


    contador_cliente = 0;

    //signal(SIGINT, sighandler);

    log_kernel = log_create("kernel.log","kernel",1,LOG_LEVEL_TRACE);

    config_kernel = config_create("kernel.config");



    ///////////////////


    //HACER TODAS LAS INICIALIZACIONES
    inicializar_config();
    inicializar_listas();
    inicializar_semaforos();
    inicializar_planificacion();


    ////////////////////
	log_trace(log_kernel,"EL ALGORITMO DE PLANIF ES: %d",algoritmo_config);

    ///

   // fd_kernel = iniciar_servidor(log_kernel,"KERNEL",ip,puerto_escucha);
	int fd_kernel_1 = iniciar_servidor(log_kernel,"KERNEL",ip,puerto_escucha);


    log_trace(log_kernel,"El socket : %d",fd_kernel);

    /////////////////////////////////////////////////////////

    fd_memoria=0;
    if (!generar_conexion_kernel_a_memoria(log_kernel, ip_memoria, puerto_memoria, &fd_memoria)) {
    			cerrar_programa2(log_kernel);
    			return EXIT_FAILURE;
    }
    log_trace(log_kernel,"El fd_memoria despues de grar conexiones es: %d",fd_memoria);


    ////////////////////////////////////////////////////////////

    fd_cpu = 0;
    if (!generar_conexiones_cpu(log_kernel, ip_cpu, puerto_cpu_dispatch, &fd_cpu)) {
    		cerrar_programa2(log_kernel);
    		return EXIT_FAILURE;
    }
    log_trace(log_kernel,"El fd_cpu despues de grar conexiones es: %d",fd_cpu);


    //conexion entre Kernel (Servidor) y consola(cliente)
   // while(server_escuchar_kernel(log_kernel,"KERNEL",fd_kernel));
    while(server_escuchar_kernel(log_kernel,"KERNEL",fd_kernel_1));


    log_warning(log_kernel,"Despues de server escuchar");

    cerrar_programa2(log_kernel);

    return EXIT_SUCCESS;
}
