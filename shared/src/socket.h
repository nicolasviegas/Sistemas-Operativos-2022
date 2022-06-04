#ifndef SOCKETS_H_
#define SOCKETS_H_

#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <commons/log.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include<sys/socket.h>
#include<netdb.h>


int fd_cpu;
int fd_memoria;
int fd_kernel;
int fd_cpu_interrupt;

bool interrupcion; //borrar

int iniciar_servidor(t_log* logger, const char* name, char* ip, char* puerto);
int esperar_cliente(t_log* logger, const char* name, int socket_servidor);
int crear_conexion(t_log* logger, const char* server_name, char* ip, char* puerto);
void liberar_conexion(int* socket_cliente);

//int generar_conexion_kernel_cpu(t_log* logger, char* ip, char* puerto,int fd_cpu);
bool generar_conexiones_cpu(t_log* logger, char* ip, char* puerto, int* fd_mod2);

bool generar_conexion_kernel_a_memoria(t_log* logger, char* ip, char* puerto,int* fd_mod3);
bool generar_conexion_cpu_a_memoria(t_log* logger, char* ip, char* puerto,int* fd_mod3);
bool generar_conexiones(t_log* logger, char* ip, char* puerto, int* fd_mod2);

#endif
