#ifndef CPUH
#define CPUH

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
#include "../include/comunicacion_cpu.h"



//t_list* lista_instrucciones_kernel;

//uint32_t tam_del_proceso_por_archivo;

t_config* config_cpu;

t_log* log_cpu;

//t_paquete* paquete_consola_kernel;

//agregas colas_new, colas_ready, etc

int server_escuchar_cpu(t_log* logger, char* server_name, int server_socket);

void cerrar_programa(t_log* logger);

#endif /*CPUH*/
