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
	NO_OP, //0
	IO, //1
	READ, //2
	COPY,//3 LO AGREGO YO
	WRITE, //4
	EXIT, //5
}op_code_instrucciones;

#define BACKLOG 20

bool send_NO_OP(int fd, uint8_t  nota1, uint8_t  nota2);
bool recv_NO_OP(int fd, uint8_t* nota1, uint8_t* nota2);

bool send_IO(int fd, char*  peli, uint8_t  cant_pochoclos);
bool recv_IO(int fd, char** peli, uint8_t* cant_pochoclos);

bool send_READ(int fd);
bool recv_READ(int fd);

bool send_COPY(int fd);
bool recv_COPY(int fd);

bool send_WRITE(int fd);
bool recv_WRITE(int fd);

bool send_EXIT(int fd);
bool recv_EXIT(int fd);


bool send_debug(int fd);

#endif
