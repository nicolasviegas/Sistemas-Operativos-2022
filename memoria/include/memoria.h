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
int algoritmo_config;
uint32_t marcos_por_proceso;
char* path_swap;

t_log* log_memoria;


typedef struct{
	uint32_t nro_pagina;
	uint32_t frame; // si hago frame * tamanio_paginas obtengo el offset donde arranca esta pagina si esta en memoria
	uint32_t bit_presencia;
	uint32_t bit_uso;
	uint32_t bit_modificado;
	uint32_t tiempo_uso;
}pagina;

typedef struct{
	uint32_t nro_tabla_2do_nivel;
}entrada_1er_nivel;








void* memoria_principal;

t_list* lista_tablas_1er_nivel; // lista de tablas de 1er nivel (una sola, adentro tiene una tabla por proceso)

t_list* lista_tablas_2do_nivel; // lista de tablas de 2do nivel






uint32_t indice_tabla; // ya no hace falta por que lo obtengo con la funcion nueva todo
//t_paquete* paquete_consola_kernel;

//agregas colas_new, colas_ready, etc

int server_escuchar_memoria(t_log* logger, char* server_name, int server_socket);

void inicializar_config();

uint32_t asignar_tabla_1er_nivel_a_proceso();

uint32_t obtener_nro_tabla_2do_nivel(uint32_t numero_tabla_1er_nivel,uint32_t entrada_primer_nivel);

pagina* buscar_pagina_en_tabla_2do_nivel(uint32_t nro_tabla_2do_nivel,uint32_t nro_pagina);


void cerrar_programa(t_log* logger);

#endif /*CPUH*/
