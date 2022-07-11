#ifndef SHARED_INCLUDE_CONSOLA_KERNEL_H_
#define SHARED_INCLUDE_CONSOLA_KERNEL_H_

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


/*typedef enum{ //tipos de identificadores a parsear
	NO_OP, //0
	IO, //1
	READ, //2
	COPY,//3 LO AGREGO YO
	WRITE, //4
	EXIT, //5
}op_code_tipo_identificador;*/

void* serializar_paquete(t_paquete* paquete, int bytes);
int recibir_operacion(int socket_cliente);
void recibir_mensaje(int socket_cliente,t_log* logger);
void* recibir_buffer(int* size, int socket_cliente);

#endif /* SHARED_INCLUDE_CONSOLA_KERNEL_H_ */
