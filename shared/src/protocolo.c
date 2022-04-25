#include "../include/protocolo.h"

// APROBAR_OPERATIVOS
static void* serializar_NO_OP(int parametro1) {
   void* stream = malloc(sizeof(op_code_instrucciones) + sizeof(uint8_t));

    op_code_instrucciones cop = NO_OP;
    memcpy(stream, &cop, sizeof(op_code_instrucciones));
    memcpy(stream+sizeof(op_code_instrucciones), &parametro1, sizeof(uint8_t));

    return stream;
}

static void* serializar_NO_OP_2(size_t* size,int parametro1) {
	 size_t size_parametro1 = sizeof(int);
	    *size =
	          sizeof(op_code_instrucciones)   // cop
	        + sizeof(size_t)    // total
	        + size_parametro1;
	    size_t size_payload = *size - sizeof(op_code_instrucciones) - sizeof(size_t);

	    void* stream = malloc(*size);

	    op_code_instrucciones cop = NO_OP;
	    memcpy(stream, &cop, sizeof(op_code_instrucciones));
	    memcpy(stream+sizeof(op_code_instrucciones), &size_payload, sizeof(size_t));
	    memcpy(stream+sizeof(op_code_instrucciones)+sizeof(size_t), &size_parametro1, sizeof(size_t));

	    return stream;
}

/*static void deserializar_NO_OP(void* stream, uint8_t* parametro1) {
	size_t tam_param;

    memcpy(parametro1, stream + sizeof(tam_param),sizeof(uint8_t));

}*/

static void deserializar_NO_OP_2(void* stream, int* parametro1) {
	size_t tam_param;

    memcpy(parametro1, stream + sizeof(tam_param),sizeof(uint8_t));

}

/*bool send_NO_OP(int fd, uint8_t parametro1) {
   size_t size = sizeof(op_code_instrucciones) + sizeof(uint8_t);
    void* stream = serializar_NO_OP(parametro1);
    if (send(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }
    free(stream);
    return true;
}*/

bool send_NO_OP_2(int fd, int parametro1) {
   size_t size;
    void* stream = serializar_NO_OP_2(&size,parametro1);
    if (send(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }
    free(stream);
    return true;
}

/*bool recv_NO_OP(int fd, uint8_t* parametro1) {
    size_t size = sizeof(uint8_t);
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_NO_OP(stream, parametro1);

    free(stream);
    return true;
}*/

bool recv_NO_OP_2(int fd, int* parametro1) {
    size_t size = sizeof(uint8_t);
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_NO_OP_2(stream, parametro1);

    free(stream);
    return true;
}

static void* serializar_IO(int parametro1) {
	void* stream = malloc(sizeof(op_code_instrucciones) + sizeof(uint8_t));

	    op_code_instrucciones cop = IO;
	    memcpy(stream, &cop, sizeof(op_code_instrucciones));
	    memcpy(stream+sizeof(op_code_instrucciones), &parametro1, sizeof(uint8_t));

	    return stream;
}

static void deserializar_IO(void* stream, uint8_t* parametro1) {
	size_t tam_param;

	memcpy(parametro1, stream + sizeof(tam_param),sizeof(uint8_t));
}

bool send_IO(int fd, uint8_t parametro1) {
    size_t size;
    void* stream = serializar_IO(parametro1);
    if (send(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }
    free(stream);
    return true;
}

bool recv_IO(int fd, uint8_t* parametro1) {
    size_t size_payload;
    if (recv(fd, &size_payload, sizeof(size_t), 0) != sizeof(size_t))
        return false;

    void* stream = malloc(size_payload);
    if (recv(fd, stream, size_payload, 0) != size_payload) {
        free(stream);
        return false;
    }

    deserializar_IO(stream, parametro1);

    free(stream);
    return true;
}

// DEBUG

