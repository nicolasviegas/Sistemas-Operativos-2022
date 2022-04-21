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
#include "../../shared/include/utils.h"


typedef struct{
	char* id;
	int parametro1;
	int parametro2;
}instrucciones;

#define NULL_PROPIO 33

typedef enum{ //tipos de identificadores a parsear
	NO_OP, //0
	IO, //1
	READ, //2
	COPY,//3 LO AGREGO YO
	WRITE, //4
	EXIT, //5
}op_code_tipo_identificador;


t_list* lista_instrucciones;

t_config* config_consola;

t_log* log_consola;

void obtener_instrucciones(char* path);

void paquete_instrucciones(t_list* lista_instrucciones, int socket);

bool recibir_confirmacion(int conexion);

void terminar_consola(t_log* log, t_list* lista, int conexion, t_config* config);

void mostrar_lista_instrucciones(t_list* lista);

void mostrar_lista_instrucciones2(t_queue* queue,char * nombre_cola);


#endif /*CONSOLA_H_*/
