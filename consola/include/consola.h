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
#include<assert.h>
#include "../../consola/include/sumas.h"
#include "../../shared/include/utils.h"


//#include<utils.h>

typedef struct{
	char* id;
	int parametro1;
	int parametro2;
}instrucciones;


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

void obtener_instrucciones(char*);


void paquete_instrucciones(t_list*, int);

bool recibir_confirmacion(int conexion);

void terminar_consola(t_log* , t_list* , int, t_config*  );

bool sintaxis(char* path);

bool unParametro(char* id);

bool dosParametros(char*id);

bool analizar_sintaxis(char* buffer);


#endif /*CONSOLA_H_*/
