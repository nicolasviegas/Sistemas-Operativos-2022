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
//#include "../../shared/include/utils.h"
//#include "../../shared/include/protocolo.h"
//#include "../../shared/include/socket.h"
#include "utils.h"
#include "protocolo.h"
#include "socket.h"



typedef enum{
	CLOCK,
	CLOCK_M
}t_algoritmo_memoria;


t_config* config_memoria;

char* puerto_escucha;
uint32_t tamanio_memoria;
uint32_t tamanio_paginas;
uint32_t entradas_por_tabla;
uint32_t retardo_memoria;
uint32_t retardo_swap;
char* algoritmo_config;
uint32_t marcos_por_proceso;
char* path_swap;

t_log* log_memoria;


uint32_t indice_tabla;
//t_paquete* paquete_consola_kernel;

//agregas colas_new, colas_ready, etc

int server_escuchar_memoria(t_log* logger, char* server_name, int server_socket);

void inicializar_config();

void cerrar_programa(t_log* logger);

#endif /*CPUH*/
