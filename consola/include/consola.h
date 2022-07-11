#ifndef CONSOLA_H_
#define CONSOLA_H_

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
//#include "../shared/include/utils.h"
//#include "../../shared/include/consola-kernel.h"
#include "utils.h"
#include "consola-kernel.h"
#include "../include/comunicacion_consola.h"

char* path;
uint32_t tam;
char* ip;
char* puerto;




t_list* lista_instrucciones;

t_config* config_consola;

t_log* log_consola;

t_paquete* paquete_consola_kernel;

void empaquetar(int socket);

void obtener_instrucciones(char* path);

void paquete_instrucciones(t_list* lista_instrucciones, int socket);

bool recibir_confirmacion(int conexion);

void terminar_consola(t_log* log, t_list* lista, int conexion, t_config* config);

void mostrar_lista_instrucciones(t_list* lista);

void mostrar_lista_instrucciones2(t_queue* queue,char * nombre_cola);

void cerrar_programa(t_log* logger);

#endif /*CONSOLA_H_*/
