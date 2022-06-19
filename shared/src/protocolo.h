#ifndef PROTOCOLOH
#define PROTOCOLOH

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

typedef struct{
	int id;
	char* nombre;
	uint32_t parametro1;
	uint32_t parametro2;
}instrucciones;

typedef enum{
	TLB_CPY = 25,
	TLB_RD = 26,
	TLB_WR = 27
}t_algoritmo_reemplazo_tlb;

typedef enum{
	METER_A_SWAP = 1000,
	SACAR_DE_SWAP = 1001,
	METER_EN_MEM_PRINCIPAL = 1002,
	SACAR_PROCESO_DE_MEMORIA = 1003,
	COPIAR_PAGINAS = 1004
}condicion_if;

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

//bool send_PCB(int fd_cpu,pcb_t* pcb_proceso);

bool send_pid_to_cpu(int fd,uint32_t parametro1);
bool recv_pid_to_cpu(int fd, uint32_t* parametro1);

bool send_indice_a_kernel(int fd_memoria,uint32_t parametro1);
bool recv_indice_a_kernel(int fd_memoria,uint32_t* parametro1);

//bool send_instrucciones_to_cpu(int fd,t_list* parametro1);
//bool recv_instrucciones_to_cpu(int fd, t_list** parametro1,int tam);

bool send_cant_instrucciones(int fd,uint32_t cantidad);
bool recv_cant_instrucciones(int fd, uint32_t* cantidad);

bool send_indice_tabla_paginas_a_cpu(int fd,uint32_t parametro1);
bool recv_indice_tabla_paginas_a_cpu(int fd,uint32_t* parametro1);

bool send_PC(int fd, uint32_t parametro1);
bool recv_PC(int fd, uint32_t* parametro1);

bool send_tiempo_bloqueante(int fd, uint32_t parametro1);
bool recv_tiempo_bloqueante(int fd, uint32_t* parametro1);

bool send_interrupcion(int fd, uint32_t parametro1);
bool recv_interrupcion(int fd, uint32_t* parametro1);

bool send_tam_paginas(int fd,uint32_t cantidad);
bool recv_tam_paginas(int fd,uint32_t* cantidad);

bool send_numero_pagina(int fd,uint32_t numero_pagina);
bool recv_numero_pagina(int fd,uint32_t* numero_pagina);

bool send_entrada_1er_nivel(int fd,uint32_t entrada_1er_nivel);
bool recv_entrada_1er_nivel(int fd,uint32_t* entrada_1er_nivel);

bool send_entrada_2do_nivel(int fd,uint32_t entrada_2do_nivel);
bool recv_entrada_2do_nivel(int fd,uint32_t* entrada_2do_nivel);

bool send_desplazamiento(int fd,uint32_t desplazamiento);
bool recv_desplazamiento(int fd,uint32_t* desplazamiento);

bool send_valor(int fd,uint32_t valor);
bool recv_valor(int fd,uint32_t* valor);

bool send_debug(int fd);

//aaaa


#endif
