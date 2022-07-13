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
#include <pthread.h>
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
#include "math.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>


typedef enum{
	CLOCK,
	CLOCK_M
}t_algoritmo_memoria;


pthread_mutex_t mutexListaFrame;

pthread_mutex_t mutexEscribirEnMemoria;

pthread_mutex_t mutexLeerEnMemoria;


t_config* config_memoria;

char* puerto_escucha;
uint32_t tamanio_memoria;
uint32_t tamanio_paginas;
uint32_t cant_entradas_por_tabla;
uint32_t retardo_memoria;
uint32_t retardo_swap;
int algoritmo_config;
uint32_t marcos_por_proceso;
char* path_swap;

int punteroClock;

int cant_archivos_swap;

t_log* log_memoria;

typedef struct{
	uint32_t nro_pagina;
	uint32_t frame; // si hago frame * tamanio_paginas obtengo el offset donde arranca esta pagina si esta en memoria
	uint32_t bit_presencia;
	uint32_t bit_uso;
	uint32_t bit_modificado;
	//uint32_t tiempo_uso;
}pagina;


typedef struct{
	bool ocupado;
	uint32_t nro_pagina;
	uint32_t indice_proceso;
//	uint32_t bit_uso;
//	uint32_t bit_modificado;
}frame;


typedef struct{
	uint32_t nro_tabla_2do_nivel;
}entrada_1er_nivel;

void* memoria_principal;

t_list* lista_tablas_1er_nivel; // lista de tablas de 1er nivel (una sola, adentro tiene una tabla por proceso)

t_list* lista_tablas_2do_nivel; // lista de tablas de 2do nivel

t_list* lista_frames;

pthread_mutex_t mutexSwap;


//uint32_t indice_tabla;
int obtener_algoritmo(char* algoritmo_char);

int server_escuchar_memoria(t_log* logger, char* server_name, int server_socket);

void inicializar_config();

uint32_t asignar_tabla_1er_nivel_a_proceso();

uint32_t obtener_nro_tabla_2do_nivel(uint32_t numero_tabla_1er_nivel,uint32_t entrada_primer_nivel);

pagina* buscar_pagina_en_tabla_2do_nivel(uint32_t nro_tabla_2do_nivel,uint32_t nro_pagina);

t_list* dividir_proceso_en_paginas(uint32_t tam_proceso);

t_list* colocar_paginas_en_tabla(t_list* lista_paginas_del_proceso);

void liberar_memoria(uint32_t frame);

void escribir_pagina(uint32_t valor,uint32_t frame, uint32_t desplazamiento);

uint32_t leer_de_memoria(uint32_t frame, uint32_t desp);

bool el_proceso_tiene_almenos_una_pag_en_mem(uint32_t indice_tabla_1er_nivel);

bool al_proceso_le_quedan_frames(uint32_t indice_tabla_1er_nivel);

void traer_proceso_de_swap(uint32_t indice_archivo_swap,uint32_t tamanio, pagina* pagina_buscada,uint32_t frame_a_utilizar);

void pasar_proceso_a_swap(uint32_t indice_tabla);

void crear_archivo(char* path_char);

void escribir_en_swap(uint32_t indice_archivo_swap,pagina* pagina_a_escribir);

t_list* traer_pagina_de_swap(uint32_t indice_archivo_swap,uint32_t nro_pagina,uint32_t tamanio);

char* pasar_a_char(uint32_t num);

uint32_t buscar_frame_libre();

void ejecutar_reemplazo(t_list* lista_valores, pagina* info_pagina,uint32_t indice_pagina_1er_nivel);

void cargar_lista_frames();

void poner_pagina_en_marco(uint32_t marco,pagina* pagina,uint32_t indice_proceso);

void poner_archivo_con_ceros(char* path_char,uint32_t tam_proceso);

void sacar_proceso_de_memoria(uint32_t indice_proceso);

void actualizar_bit_uso_tlb(uint32_t marco_aux);

void actualizar_bit_modif_tlb(uint32_t marco_aux);

void cerrar_programa(t_log* logger);

#endif /*CPUH*/
