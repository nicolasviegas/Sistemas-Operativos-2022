#ifndef MEMORIAH
#define MEMORIAH

#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include <stdbool.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<commons/collections/queue.h>
#include<commons/string.h>
#include<assert.h>
#include "../../shared/include/utils.h"
#include "../../shared/include/protocolo.h"
#include "../../shared/include/socket.h"


t_config* config_memoria;


t_log* log_memoria;


uint32_t indice_tabla;
//t_paquete* paquete_consola_kernel;

//agregas colas_new, colas_ready, etc

int server_escuchar_memoria(t_log* logger, char* server_name, int server_socket);

void cerrar_programa(t_log* logger);

#endif /*CPUH*/
