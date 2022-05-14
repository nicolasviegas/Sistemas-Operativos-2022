#ifndef KERNELH
#define KERNELH

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
#include "../include/comunicacion_kernel.h"
#include "../include/funciones_kernel.h"
#include "../include/planificacion.h"

typedef enum{ //tipos de identificadores a parsear
    NEW = 25, //0
    READY = 26, //1
    EXEC = 27, //2
    BLOCKED = 28,//3 LO AGREGO YO
    SUSPENDED_BLOCKED = 29, //4
    SUSPENDED_READY = 30,
	FINISH = 31, //5
}op_estados;


pthread_t hiloNewReady;

char* ip;
char* puerto_escucha;
char* ip_cpu;
char* ip_memoria;
char* puerto_memoria;
char* puerto_cpu_dispatch;
char* puerto_cpu_interrupt;
int algoritmo_config;
int estimacion_inicial;
float alfa;
uint32_t grado_multiprogramacion;
uint32_t tiempo_max_bloqueado;




t_list* lista_instrucciones_kernel;

uint32_t tam_del_proceso_por_archivo;

t_config* config_kernel;

t_log* log_kernel;

t_paquete* paquete_consola_kernel;

//agregas colas_new, colas_ready, etc
void inicializar_config();
void inicializar_listas();
void inicializar_semaforos();
void inicializar_planificacion();

int obtener_algoritmo(char* algoritmo_char);

int server_escuchar_kernel(t_log* logger, char* server_name, int server_socket);

void cerrar_programa_kernel(t_log* logger);

bool generar_conexiones_cpu(t_log* logger, char* ip, char* puerto, int* fd_mod2);


#endif /*KERNELH*/
