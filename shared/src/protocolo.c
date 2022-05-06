#include "../include/protocolo.h"


static void* serializar_NO_OP(uint32_t parametro1) {
   void* stream = malloc(sizeof(op_code_instrucciones) + sizeof(uint32_t));

    op_code_instrucciones cop = NO_OP;
    memcpy(stream, &cop, sizeof(op_code_instrucciones));
    memcpy(stream+sizeof(op_code_instrucciones), &parametro1, sizeof(uint32_t));

    printf("El cop en serializar NO_OP es: %d\n",cop);
    printf("El parametro 1 es: %d\n",parametro1);
    printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));
    return stream;
}


static void deserializar_NO_OP_2(void* stream, uint32_t* parametro1) {
	size_t tam_param;

    memcpy(parametro1, stream ,sizeof(uint32_t));

}

bool send_NO_OP(int fd, uint32_t parametro1) {
	printf("Entre en send_NO_OP \n");
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

    printf("El cop en serializar IO es: %d\n",cop);
    printf("El parametro 1 es: %d\n",parametro1);
    printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));
    return stream;
}


static void deserializar_IO(void* stream, uint32_t* parametro1) {
	size_t tam_param;

    memcpy(parametro1, stream ,sizeof(uint32_t));

}

bool send_IO(int fd, uint32_t parametro1) {
	printf("Entre en send_IO \n");
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

    printf("El cop en serializar READ es: %d\n",cop);
    printf("El parametro 1 es: %d\n",parametro1);
    //printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));
    return stream;
}


static void deserializar_READ(void* stream, uint32_t* parametro1) {
	size_t tam_param;

    memcpy(parametro1, stream ,sizeof(uint32_t));

}

bool send_READ(int fd, uint32_t parametro1) {
	printf("Entre en send_READ \n");
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

    printf("El cop en serializar WRITE es: %d\n",cop);
    printf("El parametro 1 es: %d\n",parametro1);
    printf("El parametro 2 es: %d\n",parametro2);
   // printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));

    return stream;
}

static void deserializar_WRITE(void* stream, uint32_t* parametro1, uint32_t* parametro2) {
    memcpy(parametro1, stream, sizeof(uint32_t));
    memcpy(parametro2, stream+sizeof(uint32_t), sizeof(uint32_t));
}

bool send_WRITE(int fd, uint32_t parametro1, uint32_t parametro2) {
	printf("Entre en send_WRITE \n");
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

    printf("El cop en serializar WRITE es: %d\n",cop);
    printf("El parametro 1 es: %d\n",parametro1);
    printf("El parametro 2 es: %d\n",parametro2);
  //  printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));

    return stream;
}

static void deserializar_COPY(void* stream, uint32_t* parametro1, uint32_t* parametro2) {
    memcpy(parametro1, stream, sizeof(uint32_t));
    memcpy(parametro2, stream+sizeof(uint32_t), sizeof(uint32_t));
}

bool send_COPY(int fd, uint32_t parametro1, uint32_t parametro2) {
	printf("Entre en send_COPY \n");
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


    printf("El cop en serializar EXIT es: %d\n",cop);

  //  printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));
    return stream;
}


static void deserializar_EXIT(void* stream) {
	size_t tam_param;

    // memcpy(parametro1, stream ,sizeof(uint32_t));

}

bool send_EXIT(int fd) {
	printf("Entre en send_EXIT \n");
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
	printf("Entre en send_cant instrucciones \n");
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
    printf("El tam a enviar es: %d\n",parametro1);
   // printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));
    return stream;
}


void deserializar_TAM(void* stream, uint32_t* parametro1) {

    memcpy(parametro1, stream ,sizeof(uint32_t));
//  printf("El tam en deserializar tam es: %d \n", parametro1);

}

bool send_TAM(int fd, uint32_t parametro1) {
	printf("Entre en send_TAM \n");
   size_t size = sizeof(uint32_t);

    void* stream = serializar_TAM(parametro1);


    if (send(fd, stream, size, 0) != size) {
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

    deserializar_TAM(stream, parametro1);

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
    printf("El tam a enviar es: %d\n",parametro1);
   // printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));
    return stream;
}


void deserializar_pid(void* stream, uint32_t* parametro1) {

    memcpy(parametro1, stream ,sizeof(uint32_t));
//  printf("El tam en deserializar tam es: %d \n", parametro1);

}

bool send_pid_to_cpu(int fd,uint32_t parametro1){
	printf("Entre en send_PID \n");
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
static void* serializar_instrucciones(t_list* parametro1) {
   void* stream = malloc(sizeof(t_list));
   // op_code_instrucciones cop = READ;
    //memcpy(stream, &cop, sizeof(op_code_instrucciones));
    memcpy(stream, &parametro1, sizeof(t_list));

   // printf("El cop en serializar READ es: %d\n",cop);
    printf("El tam a enviar es: %d\n",parametro1);
   // printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));
    return stream;
}

///////////////////
/*
static void* serializar_lista_instrucciones(t_list* lista_instrucciones_kernel, int fd){
    int i;
    int cantidad_instrucciones = list_size(lista_instrucciones_kernel);
    int tamanio_lista = sizeof(int);
    int tamanio_instruccion = sizeof(int) + sizeof(char) + sizeof(int) + sizeof(int);
    int tamanio_total = tamanio_lista + (tamanio_instruccion * cantidad_instrucciones);
    void* buffer = malloc(tamanio_total);
    int offset = 0;
    memcpy(buffer + offset, &cantidad_instrucciones, sizeof(int));
    offset += sizeof(int);
    for(i = 0; i < cantidad_instrucciones; i++){
           instrucciones* instruccion = list_get(lista_instrucciones_kernel, i);
           memcpy(buffer + offset, &instruccion->id, sizeof(int));
           offset += sizeof(int);
           memcpy(buffer + offset, &instruccion->nombre, sizeof(char));
           offset += sizeof(char);
           memcpy(buffer + offset, &instruccion->parametro1, sizeof(int));
           offset += sizeof(int);
           memcpy(buffer + offset, &instruccion->parametro2, sizeof(int));
           offset += sizeof(int);
       }
    send(fd, buffer, tamanio_total, 0);
      free(buffer);
  }

//////////////////////




void deserializar_lista_instrucciones(int fd, t_list* lista_instrucciones_kernel,int tam){

    int i;
    for(i=0;i<tam;i++){
        instrucciones* instruccion = malloc(sizeof(instrucciones));
        if (!recv_NO_OP_2(fd, &instruccion)) {
                      log_error(log_kernel, "Fallo recibiendo ID");
                      return;
        }
        if (!recv_IO()(fd, &instruccion->nombre)) {
                      log_error(log_kernel, "Fallo recibiendo NOMBRE");
                      return;
        }
        if (!recv_PARAMETRO1(fd, &instruccion->parametro1)) {
                      log_error(log_kernel, "Fallo recibiendo PARAMETRO1");
                      return;
        }
        if (!recv_PARAMETRO2(fd, &instruccion->parametro2)) {
                      log_error(log_kernel, "Fallo recibiendo PARAMETRO2");
                      return;
        }
        list_add(lista_instrucciones_kernel,instruccion);
        free(instruccion);
    }
    return;
}

*/

static void* serializar_t_list_instrucciones(size_t* size, t_list* lista) {
	*size = sizeof(instrucciones)*list_size(lista); // 2 uint8_t por cada elemento
    void* stream = malloc(*size);

    // serializo los elementos
    t_list_iterator* list_it = list_iterator_create(lista);
    for (uint8_t i=0; list_iterator_has_next(list_it); i+=2) {
        instrucciones* instrucc = list_iterator_next(list_it);
        memcpy(stream, &instrucc->id, sizeof(uint32_t));
        memcpy(stream+sizeof(uint32_t), &instrucc->nombre, sizeof(char));
        memcpy(stream+sizeof(uint32_t)+sizeof(char), &instrucc->parametro1, sizeof(uint32_t));
        memcpy(stream+sizeof(uint32_t)+sizeof(char)+sizeof(uint32_t), &instrucc->parametro2, sizeof(uint32_t));
        //memcpy(stream+(i+1)*sizeof(uint8_t), &instrucc->y, sizeof(uint8_t));
    }
    list_iterator_destroy(list_it);
    return stream;
}

static t_list* deserializar_t_list_instrucciones(void* stream, int n_elements) {
    t_list* lista = list_create();

    // De-serializo y los meto en la lista
    for (int i=0; n_elements>0; n_elements--) {
        instrucciones* instrucc = malloc(sizeof(instrucciones));
        memcpy(&instrucc->id, stream,  sizeof(uint32_t));
        memcpy(&instrucc->nombre,stream+sizeof(uint32_t), sizeof(char));
        memcpy(&instrucc->parametro1, stream+sizeof(uint32_t)+sizeof(char), sizeof(uint32_t));
        memcpy(&instrucc->parametro2, stream+sizeof(uint32_t)+sizeof(char)+sizeof(uint32_t),  sizeof(uint32_t));


        list_add(lista, instrucc);
    }
    return lista;
}

bool send_instrucciones_to_cpu(int fd,t_list* parametro1){
	printf("Entre en send_instrucciones \n");
	   size_t size;// = sizeof(t_list);

	    void* stream = serializar_t_list_instrucciones(size,&parametro1);


	    if (send(fd, stream, size, 0) != size) {
	        free(stream);
	        return false;
	    }

	    free(stream);
	    return true;
}


bool recv_instrucciones_to_cpu(int fd, t_list** parametro1,int tam) {
    size_t size = sizeof(t_list);
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_t_list_instrucciones(stream,6);//HARDCODEADO EL 6 HAY QUE DARLE LA VUELTA

    free(stream);
    return true;
}
