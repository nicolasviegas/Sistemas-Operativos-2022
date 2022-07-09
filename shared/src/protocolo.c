#include "protocolo.h"


static void* serializar_NO_OP(uint32_t parametro1) {
   void* stream = malloc(sizeof(op_code_instrucciones) + sizeof(uint32_t));

    op_code_instrucciones cop = NO_OP;
    memcpy(stream, &cop, sizeof(op_code_instrucciones));
    memcpy(stream+sizeof(op_code_instrucciones), &parametro1, sizeof(uint32_t));

    //printf("El cop en serializar NO_OP es: %d\n",cop);
   // printf("El parametro 1 es: %d\n",parametro1);
    //printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));
    return stream;
}


static void deserializar_NO_OP_2(void* stream, uint32_t* parametro1) {
	size_t tam_param;

    memcpy(parametro1, stream ,sizeof(uint32_t));

}

bool send_NO_OP(int fd, uint32_t parametro1) {
	//printf("Entre en send_NO_OP \n");
   size_t size = sizeof(op_code_instrucciones) + sizeof(uint32_t);

    void* stream = serializar_NO_OP(parametro1);


    if (send(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    free(stream);
    return true;
}


bool recv_NO_OP_2(int fd, uint32_t* parametro1) {
    size_t size = sizeof(uint32_t);
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_NO_OP_2(stream, parametro1);

    free(stream);
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void* serializar_IO(uint32_t parametro1) {
   void* stream = malloc(sizeof(op_code_instrucciones) + sizeof(uint32_t));

    op_code_instrucciones cop = IO;
    memcpy(stream, &cop, sizeof(op_code_instrucciones));
    memcpy(stream+sizeof(op_code_instrucciones), &parametro1, sizeof(uint32_t));

   // printf("El cop en serializar IO es: %d\n",cop);
   // printf("El parametro 1 es: %d\n",parametro1);
   // printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));
    return stream;
}


static void deserializar_IO(void* stream, uint32_t* parametro1) {
	size_t tam_param;

    memcpy(parametro1, stream ,sizeof(uint32_t));

}

bool send_IO(int fd, uint32_t parametro1) {
	//printf("Entre en send_IO \n");
   size_t size = sizeof(op_code_instrucciones) + sizeof(uint32_t);

    void* stream = serializar_IO(parametro1);


    if (send(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    free(stream);
    return true;
}


bool recv_IO(int fd, uint32_t* parametro1) {
    size_t size = sizeof(uint32_t);
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_IO(stream, parametro1);

    free(stream);
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void* serializar_READ(uint32_t parametro1) {
   void* stream = malloc(sizeof(op_code_instrucciones) + sizeof(uint32_t));

    op_code_instrucciones cop = READ;
    memcpy(stream, &cop, sizeof(op_code_instrucciones));
    memcpy(stream+sizeof(op_code_instrucciones), &parametro1, sizeof(uint32_t));

  //  printf("El cop en serializar READ es: %d\n",cop);
   // printf("El parametro 1 es: %d\n",parametro1);
    //printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));
    return stream;
}


static void deserializar_READ(void* stream, uint32_t* parametro1) {
	size_t tam_param;

    memcpy(parametro1, stream ,sizeof(uint32_t));

}

bool send_READ(int fd, uint32_t parametro1) {
	//printf("Entre en send_READ \n");
   size_t size = sizeof(op_code_instrucciones) + sizeof(uint32_t);

    void* stream = serializar_READ(parametro1);


    if (send(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    free(stream);
    return true;
}


bool recv_READ(int fd, uint32_t* parametro1) {
    size_t size = sizeof(uint32_t);
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_READ(stream, parametro1);

    free(stream);
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

static void* serializar_WRITE(uint32_t parametro1, uint32_t parametro2) {
    void* stream = malloc(sizeof(op_code_instrucciones) + sizeof(uint32_t) * 2);

    op_code_instrucciones cop = WRITE;
    memcpy(stream, &cop, sizeof(op_code_instrucciones));
    memcpy(stream+sizeof(op_code_instrucciones), &parametro1, sizeof(uint32_t));
    memcpy(stream+sizeof(op_code_instrucciones)+sizeof(uint32_t), &parametro2, sizeof(uint32_t));

   // printf("El cop en serializar WRITE es: %d\n",cop);
   // printf("El parametro 1 es: %d\n",parametro1);
  //  printf("El parametro 2 es: %d\n",parametro2);
   // printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));

    return stream;
}

static void deserializar_WRITE(void* stream, uint32_t* parametro1, uint32_t* parametro2) {
    memcpy(parametro1, stream, sizeof(uint32_t));
    memcpy(parametro2, stream+sizeof(uint32_t), sizeof(uint32_t));
}

bool send_WRITE(int fd, uint32_t parametro1, uint32_t parametro2) {
	//printf("Entre en send_WRITE \n");
    size_t size = sizeof(op_code_instrucciones) + sizeof(uint32_t) * 2;
    void* stream = serializar_WRITE(parametro1, parametro2);
    if (send(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }
    free(stream);
    return true;
}

bool recv_WRITE(int fd, uint32_t* parametro1, uint32_t* parametro2) {
    size_t size = sizeof(uint32_t) * 2;
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_WRITE(stream, parametro1, parametro2);

    free(stream);
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void* serializar_COPY(uint32_t parametro1, uint32_t parametro2) {
    void* stream = malloc(sizeof(op_code_instrucciones) + sizeof(uint32_t) * 2);

    op_code_instrucciones cop = COPY;
    memcpy(stream, &cop, sizeof(op_code_instrucciones));
    memcpy(stream+sizeof(op_code_instrucciones), &parametro1, sizeof(uint32_t));
    memcpy(stream+sizeof(op_code_instrucciones)+sizeof(uint32_t), &parametro2, sizeof(uint32_t));

//    printf("El cop en serializar COPY es: %d\n",cop);
//    printf("El parametro 1 es: %d\n",parametro1);
//    printf("El parametro 2 es: %d\n",parametro2);
  //  printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));

    return stream;
}

static void deserializar_COPY(void* stream, uint32_t* parametro1, uint32_t* parametro2) {
    memcpy(parametro1, stream, sizeof(uint32_t));
    memcpy(parametro2, stream+sizeof(uint32_t), sizeof(uint32_t));
}

bool send_COPY(int fd, uint32_t parametro1, uint32_t parametro2) {
	//printf("Entre en send_COPY \n");
    size_t size = sizeof(op_code_instrucciones) + sizeof(uint32_t) * 2;
    void* stream = serializar_COPY(parametro1, parametro2);
    if (send(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }
    free(stream);
    return true;
}

bool recv_COPY(int fd, uint32_t* parametro1, uint32_t* parametro2) {
    size_t size = sizeof(uint32_t) * 2;
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_COPY(stream, parametro1, parametro2);

    free(stream);
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void* serializar_EXIT() {
   void* stream = malloc(sizeof(op_code_instrucciones));

    op_code_instrucciones cop = EXIT;
    memcpy(stream, &cop, sizeof(op_code_instrucciones));


  //  printf("El cop en serializar EXIT es: %d\n",cop);

  //  printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));
    return stream;
}


static void deserializar_EXIT(void* stream) {
	size_t tam_param;

    // memcpy(parametro1, stream ,sizeof(uint32_t));

}

bool send_EXIT(int fd) {
	//printf("Entre en send_EXIT \n");
   size_t size = sizeof(op_code_instrucciones);

    void* stream = serializar_EXIT();


    if (send(fd, stream, size, 0) != size) { //ACA EN EL SEND PUSE EL EXIT, ANTES HABIA UN STREAM
      //  free(stream);
        return false;
    }

   // free(stream);
    return true;
}


bool recv_EXIT(int fd) {
    size_t size = sizeof(op_code_instrucciones);
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

   // deserializar_EXIT(stream);

    free(stream);
    return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
static void* serializar_cant_instrucciones(uint32_t cantidad){
	void* stream = malloc(sizeof(uint32_t));


	    memcpy(stream, &cantidad, sizeof(uint32_t));


	    //printf("El tam a enviar es: %d\n",parametro1);

	    return stream;
}

void deserializar_cant_instrucciones(void* stream, uint32_t* cantidad) {

    memcpy(cantidad, stream ,sizeof(uint32_t));
//  printf("El tam en deserializar tam es: %d \n", parametro1);

}


bool send_cant_instrucciones(int fd,uint32_t cantidad){
	//printf("Entre en send_cant instrucciones \n");
	   size_t size = sizeof(uint32_t);

	    void* stream = serializar_cant_instrucciones(cantidad);


	    if (send(fd, stream, size, 0) != size) {
	        free(stream);
	        return false;
	    }

	    free(stream);
	    return true;
}

bool recv_cant_instrucciones(int fd, uint32_t* cantidad) {
    size_t size = sizeof(uint32_t);
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_cant_instrucciones(stream, cantidad);

    free(stream);
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////


static void* serializar_TAM(uint32_t parametro1) {
   void* stream = malloc(sizeof(uint32_t));
   // op_code_instrucciones cop = READ;
    //memcpy(stream, &cop, sizeof(op_code_instrucciones));
    memcpy(stream, &parametro1, sizeof(uint32_t));

   // printf("El cop en serializar READ es: %d\n",cop);
  //  printf("El tam a enviar es: %d\n",parametro1);
   // printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));
    return stream;
}


void deserializar_TAM(void* stream, uint32_t* parametro1) {

    memcpy(parametro1, stream ,sizeof(uint32_t));
//  printf("El tam en deserializar tam es: %d \n", parametro1);

}

bool send_TAM(int fd, uint32_t parametro1) {
	//printf("Entre en send_TAM \n");
   size_t size = sizeof(uint32_t);

    void* stream = serializar_TAM(parametro1);


    if (send(fd, stream, size, MSG_WAITALL) != size) {
        free(stream);
        return false;
    }

    free(stream);
    return true;
}


bool recv_TAM(int fd, uint32_t* parametro1) {
    size_t size = sizeof(uint32_t);
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_indice_tabla_paginas_a_cpu(stream, parametro1);

    free(stream);
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//\

static void* serializar_pid(uint32_t parametro1) {
   void* stream = malloc(sizeof(uint32_t));
   // op_code_instrucciones cop = READ;
    //memcpy(stream, &cop, sizeof(op_code_instrucciones));
    memcpy(stream, &parametro1, sizeof(uint32_t));

   // printf("El cop en serializar READ es: %d\n",cop);
   // printf("El tam a enviar es: %d\n",parametro1);
   // printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));
    return stream;
}


void deserializar_pid(void* stream, uint32_t* parametro1) {

    memcpy(parametro1, stream ,sizeof(uint32_t));
//  printf("El tam en deserializar tam es: %d \n", parametro1);

}

bool send_pid_to_cpu(int fd,uint32_t parametro1){
	//printf("Entre en send_PID \n");
	   size_t size = sizeof(uint32_t);

	    void* stream = serializar_pid(parametro1);


	    if (send(fd, stream, size, 0) != size) {
	        free(stream);
	        return false;
	    }

	    free(stream);
	    return true;
}

bool recv_pid_to_cpu(int fd, uint32_t* parametro1) {
    size_t size = sizeof(uint32_t);
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_pid(stream, parametro1);

    free(stream);
    return true;
}
////////////////////////////////////////////////////////////////////
static void* serializar_indice_a_kernel(uint32_t parametro1) {
   void* stream = malloc(sizeof(uint32_t));

    memcpy(stream, &parametro1, sizeof(uint32_t));

    return stream;
}


void deserializar_indice_a_kernel(void* stream, uint32_t* parametro1) {

    memcpy(parametro1, stream ,sizeof(uint32_t));
 // printf("El indice en deserializar tam es: %d \n", parametro1);

}


bool send_indice_a_kernel(int fd_memoria,uint32_t parametro1){
	 size_t size = sizeof(uint32_t);

		    void* stream = serializar_indice_a_kernel(parametro1);


		    if (send(fd_memoria, stream, size, 0) != size) {
		        free(stream);
		        return false;
		    }

		    free(stream);
		    return true;
}

bool recv_indice_a_kernel(int fd, uint32_t* parametro1) {
    size_t size = sizeof(uint32_t);
    void* stream = malloc(size);

    if (recv(fd, stream, size, MSG_WAITALL) != size) {
        free(stream);
        return false;
    }

    deserializar_indice_a_kernel(stream, parametro1);

    free(stream);
    return true;
}
//////////////////////////

static void* serializar_indice_tabla_paginas_a_cpu(uint32_t parametro1) {
   void* stream = malloc(sizeof(uint32_t));
   // op_code_instrucciones cop = READ;
    //memcpy(stream, &cop, sizeof(op_code_instrucciones));
    memcpy(stream, &parametro1, sizeof(uint32_t));

   // printf("El cop en serializar READ es: %d\n",cop);
  //  printf("El tam a enviar es: %d\n",parametro1);
   // printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));
    return stream;
}


void deserializar_indice_tabla_paginas_a_cpu(void* stream, uint32_t* parametro1) {

    memcpy(parametro1, stream ,sizeof(uint32_t));
//  printf("El tam en deserializar tam es: %d \n", parametro1);

}

bool send_indice_tabla_paginas_a_cpu(int fd, uint32_t parametro1) {
	//printf("Entre en send_TAM \n");
   size_t size = sizeof(uint32_t);

    void* stream = serializar_indice_tabla_paginas_a_cpu(parametro1);


    if (send(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    free(stream);
    return true;
}


bool recv_indice_tabla_paginas_a_cpu(int fd, uint32_t* parametro1) {
    size_t size = sizeof(uint32_t);
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_TAM(stream, parametro1);

    free(stream);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
static void* serializar_PC(uint32_t parametro1) {
   void* stream = malloc(sizeof(uint32_t));
   // op_code_instrucciones cop = READ;
    //memcpy(stream, &cop, sizeof(op_code_instrucciones));
    memcpy(stream, &parametro1, sizeof(uint32_t));

   // printf("El cop en serializar READ es: %d\n",cop);
   // printf("El tam a enviar es: %d\n",parametro1);
   // printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));
    return stream;
}


void deserializar_PC(void* stream, uint32_t* parametro1) {

    memcpy(parametro1, stream ,sizeof(uint32_t));
//  printf("El PC en deserializar PC es: %d \n", parametro1);

}

bool send_PC(int fd, uint32_t parametro1) {
	//printf("Entre en send_PC \n");
   size_t size = sizeof(uint32_t);

    void* stream = serializar_PC(parametro1);


    if (send(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    free(stream);
    return true;
}


bool recv_PC(int fd, uint32_t* parametro1) {
    size_t size = sizeof(uint32_t);
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_PC(stream, parametro1);

    free(stream);
    return true;
}



//////////////////////////////////////////////////////////////////////////////////
static void* serializar_tiempo_bloqueante(uint32_t parametro1) {
   void* stream = malloc(sizeof(uint32_t));
   // op_code_instrucciones cop = READ;
    //memcpy(stream, &cop, sizeof(op_code_instrucciones));
    memcpy(stream, &parametro1, sizeof(uint32_t));

   // printf("El cop en serializar READ es: %d\n",cop);
   // printf("El tam a enviar es: %d\n",parametro1);
   // printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));
    return stream;
}


void deserializar_tiempo_bloqueante(void* stream, uint32_t* parametro1) {

    memcpy(parametro1, stream ,sizeof(uint32_t));
//  printf("El PC en deserializar PC es: %d \n", parametro1);

}

bool send_tiempo_bloqueante(int fd,uint32_t tiempo_bloqueante) {
	//printf("Entre en send_PC \n");
   size_t size = sizeof(uint32_t);

    void* stream = serializar_tiempo_bloqueante(tiempo_bloqueante);


    if (send(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    free(stream);
    return true;
}


bool recv_tiempo_bloqueante(int fd, uint32_t* parametro1) {
    size_t size = sizeof(uint32_t);
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_tiempo_bloqueante(stream, parametro1);

    free(stream);
    return true;
}


///////////////////////////////////////////////////
static void* serializar_interrupcion(uint32_t parametro1) {
   void* stream = malloc(sizeof(uint32_t));
   // op_code_instrucciones cop = READ;
    //memcpy(stream, &cop, sizeof(op_code_instrucciones));
    memcpy(stream, &parametro1, sizeof(uint32_t));

   // printf("El cop en serializar READ es: %d\n",cop);
   // printf("El tam a enviar es: %d\n",parametro1);
   // printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));
    return stream;
}


void deserializar_interrupcion(void* stream, uint32_t* parametro1) {

    memcpy(parametro1, stream ,sizeof(uint32_t));
//  printf("El PC en deserializar PC es: %d \n", parametro1);

}

bool send_interrupcion(int fd,uint32_t tiempo_bloqueante) {
	//printf("Entre en send_PC \n");
   size_t size = sizeof(uint32_t);

    void* stream = serializar_interrupcion(tiempo_bloqueante);


    if (send(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    free(stream);
    return true;
}


bool recv_interrupcion(int fd, uint32_t* parametro1) {
    size_t size = sizeof(uint32_t);
    void* stream = malloc(size);

    if (recv(fd, stream, size, MSG_DONTWAIT) != size) {
        free(stream);
        return false;
    }

    deserializar_interrupcion(stream, parametro1);

    free(stream);
    return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void* serializar_tam_paginas(uint32_t cantidad){
	void* stream = malloc(sizeof(uint32_t));


	    memcpy(stream, &cantidad, sizeof(uint32_t));


	    //printf("El tam a enviar es: %d\n",parametro1);

	    return stream;
}

void deserializar_tam_paginas(void* stream, uint32_t* cantidad) {

    memcpy(cantidad, stream ,sizeof(uint32_t));
//  printf("El tam en deserializar tam es: %d \n", parametro1);

}


bool send_tam_paginas(int fd,uint32_t cantidad){
	//printf("Entre en send_cant instrucciones \n");
	   size_t size = sizeof(uint32_t);

	    void* stream = serializar_tam_paginas(cantidad);


	    if (send(fd, stream, size, 0) != size) {
	        free(stream);
	        return false;
	    }

	    free(stream);
	    return true;
}

bool recv_tam_paginas(int fd, uint32_t* cantidad) {
    size_t size = sizeof(uint32_t);
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_tam_paginas(stream, cantidad);

    free(stream);
    return true;
}

bool send_numero_pagina(int fd,uint32_t numero_pagina){
	//printf("Entre en send_cant instrucciones \n");
	   size_t size = sizeof(uint32_t);

	    void* stream = serializar_tam_paginas(numero_pagina);


	    if (send(fd, stream, size, 0) != size) {
	        free(stream);
	        return false;
	    }

	    free(stream);
	    return true;
}

bool recv_numero_pagina(int fd, uint32_t* numero_pagina) {
    size_t size = sizeof(uint32_t);
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_tam_paginas(stream, numero_pagina);

    free(stream);
    return true;
}

bool send_entrada_1er_nivel(int fd,uint32_t entrada_1er_nivel){
//	printf("Entre en send_cant instrucciones \n");
	   size_t size = sizeof(uint32_t);

	    void* stream = serializar_tam_paginas(entrada_1er_nivel);


	    if (send(fd, stream, size, 0) != size) {
	        free(stream);
	        return false;
	    }

	    free(stream);
	    return true;
}

bool recv_entrada_1er_nivel(int fd, uint32_t* entrada_1er_nivel) {
    size_t size = sizeof(uint32_t);
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_tam_paginas(stream, entrada_1er_nivel);

    free(stream);
    return true;
}


bool send_entrada_2do_nivel(int fd,uint32_t entrada_2do_nivel){
	//printf("Entre en send_cant instrucciones \n");
	   size_t size = sizeof(uint32_t);

	    void* stream = serializar_tam_paginas(entrada_2do_nivel);


	    if (send(fd, stream, size, 0) != size) {
	        free(stream);
	        return false;
	    }

	    free(stream);
	    return true;
}

bool recv_entrada_2do_nivel(int fd, uint32_t* entrada_2do_nivel) {
    size_t size = sizeof(uint32_t);
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_tam_paginas(stream, entrada_2do_nivel);

    free(stream);
    return true;
}


bool send_desplazamiento(int fd,uint32_t desplazamiento){
	//printf("Entre en send_cant instrucciones \n");
	   size_t size = sizeof(uint32_t);

	    void* stream = serializar_tam_paginas(desplazamiento);


	    if (send(fd, stream, size, 0) != size) {
	        free(stream);
	        return false;
	    }

	    free(stream);
	    return true;
}

bool recv_desplazamiento(int fd, uint32_t* desplazamiento) {
    size_t size = sizeof(uint32_t);
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_tam_paginas(stream, desplazamiento);

    free(stream);
    return true;
}

bool send_valor(int fd,uint32_t valor){
	//printf("Entre en send_cant instrucciones \n");
	   size_t size = sizeof(uint32_t);

	    void* stream = serializar_tam_paginas(valor);


	    if (send(fd, stream, size, 0) != size) {
	        free(stream);
	        return false;
	    }

	    free(stream);
	    return true;
}

bool recv_valor(int fd, uint32_t* valor) {
    size_t size = sizeof(uint32_t);
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_tam_paginas(stream, valor);

    free(stream);
    return true;
}
