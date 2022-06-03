#include "../memoria/include/memoria.h"





void cerrar_programa5(t_log* logger) {
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

    log_memoria = log_create("memoria.log","memoria",1,LOG_LEVEL_TRACE);

    config_memoria = config_create("memoria.config");

    ip = "127.0.0.1"; //config_get_string_value(config_memoria,"IP_MEMORIA"); /////////////////////////BORRAR
    puerto_escucha = config_get_string_value(config_memoria,"PUERTO_ESCUCHA");


    fd_memoria = iniciar_servidor(log_memoria,"MEMORIA",ip,puerto_escucha);


    //log_trace(log_memoria,"El socket de memoria es : %d",fd_memoria);

    //ACA HAY QUE RESERVAR MEMORIA Y OTRAS YERBAS?
    //O SE HACE CUANDO SE CONECTA KERNEL?
    ///
    indice_tabla = 412; //ES UN EJEMPLO PARA PROBAR LAS CONEXIONES


    //

    //conexion entre cpu (Servidor) y kernel(cliente)
    while(server_escuchar_memoria(log_memoria,"MEMORIA",fd_memoria));


  //  log_warning(log_cpu,"Despues de server escuchar");

    cerrar_programa5(log_memoria);

    return 0;
}
