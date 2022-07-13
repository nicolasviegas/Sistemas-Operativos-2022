#include "../include/comunicacion_consola.h"

//////////////////////
//NUEVA EN CONSOLA, ESTABA EN KERNEL ANTES
//bool generar_conexiones(t_log* logger, char* ip, char* puerto, int* fd_mod2) {
//    // No hardcodear, levantar de config
//
//
//    *fd_mod2 = crear_conexion(logger,"KERNEL", ip, puerto );
//
//    return *fd_mod2 != 0;
//}

void send_instrucciones(t_list* lista_instrucciones,int fd_mod2){

	instrucciones* a ;
	int cant_instrucciones = list_size(lista_instrucciones);
	int indice = 0;

	//log_error(log_consola,"El socket dentro de send instrcciones es : %d",fd_mod2);
	//log_error(log_consola,"El tam recibido por parametro a enviar es: %d",tam);



	while(indice < cant_instrucciones){
		a = list_get(lista_instrucciones,indice);

		//log_warning(log_consola,"El id de las intrucciones en send instrucciones es: %d",a->id);

		if(a->id == NO_OP){
			log_info(log_consola,"Envio la intruccion NO_OP");
			send_NO_OP(fd_mod2,a->parametro1);

		}
		else if(a->id == IO){
			//log_warning(log_consola,"entre en I/O dentro de send_instrucciones");
			//printf( "el parametro 1  de IO es: %d\n",a->parametro1);
			log_info(log_consola,"Envio la intruccion I/O con un tiempo de: %d",a->parametro1);
			send_IO(fd_mod2,a->parametro1);
		}
		else if(a->id == READ){
			//log_warning(log_consola,"entre en READ dentro de send_instrucciones");
			log_info(log_consola,"Envio la intruccion READ con la dir log: %d",a->parametro1);

			send_READ(fd_mod2,a->parametro1);
		}
		else if(a->id == COPY){
			//log_warning(log_consola,"entre en COPY dentro de send_instrucciones");
			send_COPY(fd_mod2,a->parametro1,a->parametro2);
			log_info(log_consola,"Envio la intruccion COPY con la dir log: %d y %d",a->parametro1,a->parametro2);


		}
		else if(a->id == WRITE){
			//log_warning(log_consola,"entre en WRITE dentro de send_instrucciones");
			send_WRITE(fd_mod2,a->parametro1,a->parametro2);
			log_info(log_consola,"Envio la intruccion WRITE con la dir log: %d y el valor %d",a->parametro1,a->parametro2);
		}
		else if(a->id == EXIT){
			//log_warning(log_consola,"Entre en EXIT dentro de send_instrucciones");
			log_info(log_consola,"Envio la intruccion EXIT");
			send_EXIT(fd_mod2);
		}

		indice++;
	}

	//free(a);

}
