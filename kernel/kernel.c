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

void cerrar_programa2(t_log* logger) {
    log_destroy(logger);
}

int main() {

    char* ip;
    char* puerto_escucha;
    contador_cliente = 0;

    //signal(SIGINT, sighandler);

    log_kernel = log_create("kernel.log","cpu",1,LOG_LEVEL_TRACE);

    config_kernel = config_create("kernel.config");

    ip = config_get_string_value(config_kernel,"IP_MEMORIA");
    puerto_escucha = config_get_string_value(config_kernel,"PUERTO_ESCUCHA");

    char* ip_cpu = config_get_string_value(config_kernel,"IP_CPU");
    char* puerto_cpu_dispatch = config_get_string_value(config_kernel,"PUERTO_CPU_DISPATCH");



    lista_instrucciones_kernel = list_create();

    int fd_consola = iniciar_servidor(log_kernel,"KERNEL",ip,puerto_escucha);

    log_trace(log_kernel,"El socket : %d",fd_consola);

    fd_cpu = 0;
    if (!generar_conexiones_cpu(log_kernel, ip_cpu, puerto_cpu_dispatch, &fd_cpu)) {
    		cerrar_programa2(log_kernel);
    		return EXIT_FAILURE;
    }
    log_trace(log_kernel,"El fd_cpu despues de grar conexiones es: %d",fd_cpu);


    //conexion entre Kernel (Servidor) y consola(cliente)
    while(server_escuchar_kernel(log_kernel,"KERNEL",fd_consola));

/*
    instrucciones* a = malloc(sizeof(instrucciones));
    a = list_get(lista_instrucciones_kernel,0);
    log_error(log_kernel,"ID de la primer operacion: %d",a->id);
    log_error(log_kernel,"nombre de la primer operacion: %s",a->nombre);
    log_error(log_kernel,"PARAMETRO 1 de la primer operacion: %d",a->parametro1);
    log_error(log_kernel,"PARAMETRO 2  de la primer operacion: %d",a->parametro2);
    free(a);
*/
    log_warning(log_kernel,"Despues de server escuchar");

    //cerrar_programa_kernel(log_kernel);

    return EXIT_SUCCESS;
}
