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

   // lista_instrucciones_kernel = list_create();

    int fd_kernel = iniciar_servidor(log_cpu,"CPU",ip,puerto_escucha);

    log_trace(log_cpu,"El socket de kernel en cpu.c es : %d",fd_kernel);

    //conexion entre cpu (Servidor) y kernel(cliente)
    while(server_escuchar_cpu(log_cpu,"CPU",fd_kernel));


  //  log_warning(log_cpu,"Despues de server escuchar");

    cerrar_programa3(log_cpu);

    return 0;
}
