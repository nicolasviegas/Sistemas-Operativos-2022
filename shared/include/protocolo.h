#ifndef PROTOCOLO_H_
#define PROTOCOLO_H_

#include <inttypes.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef enum{ //tipos de identificadores a parsear
	NO_OP = 13, //0
	IO = 14, //1
	READ = 15, //2
	COPY = 16,//3 LO AGREGO YO
	WRITE = 17, //4
	EXIT = 18, //5
}op_code_instrucciones;

#define BACKLOG 20

bool send_NO_OP(int fd, uint32_t parametro1);


//bool recv_NO_OP(int fd, uint8_t* parametro1);
bool recv_NO_OP_2(int fd, uint32_t* parametro1);

bool send_IO(int fd, uint32_t  parametro1);
bool recv_IO(int fd, uint32_t* parametro1);

bool send_READ(int fd,uint32_t parametro1);
bool recv_READ(int fd,uint32_t* parametro1);

bool send_COPY(int fd,uint32_t parametro1,uint32_t parametro2);
bool recv_COPY(int fd,uint32_t* parametro1,uint32_t* parametro2);

bool send_WRITE(int fd,uint32_t parametro1,uint32_t parametro2);
bool recv_WRITE(int fd,uint32_t* parametro1,uint32_t* parametro2);

bool send_EXIT(int fd);
bool recv_EXIT(int fd);

bool send_TAM(int fd,uint32_t parametro1);
bool recv_TAM(int fd,uint32_t* parametro1);

void recv_TAM2(int fd,uint32_t* parametro1);

static void* serializar_NO_OP(uint32_t parametro1);

bool send_debug(int fd);

#endif
