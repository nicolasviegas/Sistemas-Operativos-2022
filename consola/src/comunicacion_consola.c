#include "../include/comunicacion_consola.h"

//////////////////////
//NUEVA EN CONSOLA, ESTABA EN KERNEL ANTES
bool generar_conexiones(t_log* logger, char* ip, char* puerto, int* fd_mod2) {
    // No hardcodear, levantar de config


    *fd_mod2 = crear_conexion(logger,"KERNEL", ip, puerto );

    return *fd_mod2 != 0;
}

void send_instrucciones(t_list* lista_instrucciones,int fd_mod2){

	instrucciones* a = malloc(sizeof(instrucciones));
	int cant_instrucciones = list_size(lista_instrucciones);
	int indice = 0;

	log_error(log_consola,"El socket dentro de send instrcciones es : %d",fd_mod2);

	while(indice < cant_instrucciones){
		a = list_get(lista_instrucciones,indice);

		//log_warning(log_consola,"El id de las intrucciones en send instrucciones es: %d",a->id);

		if(a->id == NO_OP){
			log_warning(log_consola,"entre en NO_OP dentro de send_instrucciones");
			send_NO_OP(fd_mod2,a->parametro1);

		}
		else if(a->id == IO){
			//log_warning(log_consola,"entre en I/O dentro de send_instrucciones");
		}
		else if(a->id == READ){
		//	log_warning(log_consola,"entre en READ dentro de send_instrucciones");
		}
		else if(a->id == COPY){
		//	log_warning(log_consola,"entre en COPY dentro de send_instrucciones");
		}
		else if(a->id == WRITE){
		//	log_warning(log_consola,"entre en WRITE dentro de send_instrucciones");
		}
		else if(a->id == EXIT){
			//log_warning(log_consola,"entre en EXIT dentro de send_instrucciones");
		}

		indice++;
	}

	free(a);


}
