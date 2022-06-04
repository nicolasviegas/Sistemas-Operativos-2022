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
//#include "../../shared/include/utils.h"
//#include "../../shared/include/protocolo.h"
#include "utils.h"
#include "protocolo.h"
#include "../include/comunicacion_cpu.h"

typedef struct{
	uint32_t PID;
	uint32_t tamanio;
	t_list* instrucciones;
	uint32_t PC;
	uint32_t indice_tabla_paginas;

}pcb_cpu;

bool interrupcion;

t_list* lista_instrucciones_cpu;

t_list* lista_pcb_cpu;

//uint32_t tam_del_proceso_por_archivo;

t_config* config_cpu;

t_log* log_cpu;

uint32_t tiempo_bloqueante;

//t_paquete* paquete_consola_kernel;

//agregas colas_new, colas_ready, etc

//int server_escuchar_cpu(t_log* logger, char* server_name, int server_socket);
int server_escuchar_cpu(t_log* logger, char* server_name, int server_socket,int server_socket_1);

void cerrar_programa(t_log* logger);

#endif /*CPUH*/
