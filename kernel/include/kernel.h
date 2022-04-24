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



t_list* lista_instrucciones;

t_config* config_kernel;

t_log* log_kernel;

t_paquete* paquete_consola_kernel;



bool generar_conexiones(t_log* logger, char* ip, char* puerto, int* fd_mod2);



#endif /*KERNEL_H_*/
