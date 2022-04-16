#ifndef CONSOLA_H_
#define CONSOLA_H_


#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<assert.h>

#include<utils.h>

typedef struct{
	char* id;
	int parametro1;
	int parametro2;
}inst;

typedef enum{
	NO_OP,
	IO,
	READ,
	WRITE,
	EXIT,
}op_code;


t_list* obtener_instrucciones(char*);

void paquete_instrucciones(t_list*, int);

void terminar_consola(t_log* , t_list* , int, t_config*  );


#endif /*CONSOLA_H_*/
