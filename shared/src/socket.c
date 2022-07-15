#include "socket.h"

// INICIA SERVER ESCUCHANDO EN IP:PUERTO
int iniciar_servidor(t_log* logger, const char* name, char* ip, char* puerto) {
    int socket_servidor;
    struct addrinfo hints, *servinfo;

    // Inicializando hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Recibe los addrinfo
    getaddrinfo(NULL, puerto, &hints, &servinfo);

    bool conecto = false;

    // Itera por cada addrinfo devuelto
    for (struct addrinfo *p = servinfo; p != NULL; p = p->ai_next) {
        socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (socket_servidor == -1) // fallo de crear socket
            continue;

        if (setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
            error("setsockopt(SO_REUSEADDR) failed");

        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
            // Si entra aca fallo el bind
            close(socket_servidor);
            continue;
        }
        // Ni bien conecta uno nos vamos del for
        conecto = true;
        break;
    }

    if(!conecto) {
        free(servinfo);
        return 0;
    }

    listen(socket_servidor, SOMAXCONN); // Escuchando (hasta SOMAXCONN conexiones simultaneas)

    // Aviso al logger
    log_info(logger, "Escuchando en :%s (%s)\n", puerto, name);

    freeaddrinfo(servinfo);

    return socket_servidor;
}

// ESPERAR CONEXION DE CLIENTE EN UN SERVER ABIERTO
int esperar_cliente(t_log* logger, const char* name, int socket_servidor) {
    struct sockaddr_in dir_cliente;
    socklen_t tam_direccion = sizeof(struct sockaddr_in);

    int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

    log_info(logger, "Cliente conectado a (%s)\n", name);

    return socket_cliente;
}

/////////////////////////////////////////////////////////////

bool generar_conexiones(t_log* logger, char* ip, char* puerto, int* fd_mod2) {
    // No hardcodear, levantar de config


    *fd_mod2 = crear_conexion(logger,"KERNEL", ip, puerto );


    return *fd_mod2 != 0;
}

bool generar_conexiones_cpu(t_log* logger, char* ip, char* puerto, int* fd_mod3) {
    // No hardcodear, levantar de config


    *fd_mod3 = crear_conexion(logger,"CPU", ip, puerto );

   // log_error(logger,"El ip en grar conexiones es: %s",ip);
   // log_error(logger,"El puerto en grar conexiones es: %s",puerto);
   // log_error(logger,"El socket  en generar conexiones cpu es: %d",fd_mod3);

    return *fd_mod3 != 0;
}

///////////////////////////


//int generar_conexion_kernel_cpu(t_log* logger, char* ip, char* puerto,int fd_mod3){
//	//int fd_mod3=0;
//	if (!generar_conexiones_cpu(logger, ip, puerto, &fd_mod3)) {
//		//cerrar_programa(logger);
//		return EXIT_FAILURE;
//	}
//	return EXIT_SUCCESS;
//}

bool generar_conexion_kernel_a_memoria(t_log* logger, char* ip, char* puerto,int* fd_mod3){
	 *fd_mod3 = crear_conexion(logger,"MEMORIA", ip, puerto );

	   // log_error(logger,"El ip en grar conexiones es: %s",ip);
	   // log_error(logger,"El puerto en grar conexiones es: %s",puerto);
	   // log_error(logger,"El socket  en generar conexiones cpu es: %d",fd_mod3);

	    return *fd_mod3 != 0;
}

bool generar_conexion_cpu_a_memoria(t_log* logger, char* ip, char* puerto,int* fd_mod4){
	 *fd_mod4 = crear_conexion(logger,"MEMORIA", ip, puerto );

	   // log_error(logger,"El ip en grar conexiones es: %s",ip);
	   // log_error(logger,"El puerto en grar conexiones es: %s",puerto);
	   // log_error(logger,"El socket  en generar conexiones cpu es: %d",fd_mod3);

	    return *fd_mod4 != 0;
}
