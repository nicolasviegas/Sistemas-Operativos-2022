#ifndef KERNEL_H_
#define KERNEL_H_

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
#include "../../shared/include/consola-kernel.h"
#include "../include/comunicacion_kernel.h"
#include "../include/funciones_kernel.h"



t_list* lista_instrucciones_kernel;

uint32_t tam_del_proceso_por_archivo;

t_config* config_kernel;

t_log* log_kernel;

t_paquete* paquete_consola_kernel;

int server_escuchar(t_log* logger, char* server_name, int server_socket);

void cerrar_programa(t_log* logger);

#endif /*KERNEL_H_*/
