#include "../cpu/include/cpu.h"





void cerrar_programa3(t_log* logger) {
    log_destroy(logger);
}
//void sighandler(int s) {
//    cerrar_programa(logger);
//    exit(0);
//}




int main() {

    char* ip;
    char* puerto_escucha;

    //signal(SIGINT, sighandler);

    log_cpu = log_create("cpu.log","cpu",1,LOG_LEVEL_TRACE);

    config_cpu = config_create("cpu.config");

    ip = config_get_string_value(config_cpu,"IP_MEMORIA");
    puerto_escucha = config_get_string_value(config_cpu,"PUERTO_ESCUCHA_DISPATCH");
    char* puerto_interrupt =  config_get_string_value(config_cpu,"PUERTO_ESCUCHA_INTERRUPT");


    lista_instrucciones_cpu = list_create();
    lista_pcb_cpu = list_create();

    //fd_kernel = iniciar_servidor(log_cpu,"CPU",ip,puerto_escucha);
    fd_cpu = iniciar_servidor(log_cpu,"CPU",ip,puerto_escucha);

    fd_cpu_interrupt = iniciar_servidor(log_cpu,"CPU",ip,puerto_interrupt);



    //log_trace(log_cpu,"El socket en cpu.c es : %d",fd_cpu);


    char* ip_memoria = config_get_string_value(config_cpu,"IP_MEMORIA");
    char* puerto_memoria = config_get_string_value(config_cpu,"PUERTO_MEMORIA");

    fd_memoria=0;
    if (!generar_conexion_cpu_a_memoria(log_cpu, ip_memoria, puerto_memoria, &fd_memoria)) {
       			cerrar_programa3(log_cpu);
       			return EXIT_FAILURE;
       		}
   //log_trace(log_cpu,"El fd_memoria despues de grar conexiones es: %d",fd_memoria);


    //conexion entre cpu (Servidor) y kernel(cliente)

    //  while(server_escuchar_cpu(log_cpu,"CPU",fd_kernel));
    while(server_escuchar_cpu(log_cpu,"CPU",fd_cpu,fd_cpu_interrupt));

  //  log_warning(log_cpu,"Despues de server escuchar");

    cerrar_programa3(log_cpu); //hay que agregar listas a destruir, el config y otras cosas

    return 0;
}
