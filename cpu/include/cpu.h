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

typedef struct{
	uint32_t numero_pag;
	uint32_t frame;
	uint32_t tiempo_uso;
}tlb;


typedef enum{
	FIFO,
	LRU
}t_algoritmo_tlb;

bool interrupcion;

t_list* lista_instrucciones_cpu;

t_list* lista_pcb_cpu;

//uint32_t tam_del_proceso_por_archivo;

t_config* config_cpu;

t_log* log_cpu;

t_list* lista_tlb;


char* ip;
uint32_t entradas_tlb;
char* reemplazo_tlb;
uint32_t retardo_noop;
char* puerto_escucha_dispatch;
char* puerto_interrupt;
char* ip_memoria;
char* puerto_memoria;
int algoritmo_config;
uint32_t tam_paginas; // falta recibirlo
uint32_t cant_entradas_por_tabla; // falta recibirlo

uint32_t tiempo_bloqueante;

uint32_t ultimo_proceso_cpu;
//t_paquete* paquete_consola_kernel;

//agregas colas_new, colas_ready, etc

//int server_escuchar_cpu(t_log* logger, char* server_name, int server_socket);
void inicializar_config();
int server_escuchar_cpu(t_log* logger, char* server_name, int server_socket,int server_socket_1);

uint32_t obtener_numero_pagina(uint32_t direccion_logica);
uint32_t obtener_entrada_1er_nivel(uint32_t numero_pagina);
uint32_t obtener_entrada_2do_nivel(uint32_t numero_pagina);
uint32_t obtener_desplazamiento(uint32_t direccion_logica,uint32_t numero_pagina);
bool existe_entrada(void* elem);
void correr_tlb_read(uint32_t numero_pagina,uint32_t parametro1,uint32_t tabla_1er_nivel,uint32_t tamanio);
void correr_tlb_copy(uint32_t numero_pagina_origen,uint32_t numero_pagina_destino,uint32_t parametro1,uint32_t parametro2,uint32_t tabla_1er_nivel,uint32_t tamanio);
void correr_tlb_write(uint32_t numero_pagina,uint32_t parametro1,uint32_t parametro2,uint32_t tabla_1er_nivel,uint32_t tamanio);

void cerrar_programa(t_log* logger);

#endif /*CPUH*/
