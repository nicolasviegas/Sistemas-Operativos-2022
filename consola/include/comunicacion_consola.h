#ifndef COMUNICACION_CONSOLA_H_

#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <inttypes.h>
#include <commons/log.h>
#include "../include/consola.h"
//#include "../../shared/include/socket.h"
//#include "../../shared/include/protocolo.h"
#include "socket.h"
#include "protocolo.h"


//int server_escuchar(t_log* logger, char* server_name, int server_socket);

//bool generar_conexiones(t_log* logger, char* ip, char* puerto, int* fd_mod2);

void send_instrucciones(t_list* lista_instrucciones, int fd_mod2);

#endif
