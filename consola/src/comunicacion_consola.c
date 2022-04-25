#include "../include/comunicacion_consola.h"

//////////////////////
//NUEVA EN CONSOLA, ESTABA EN KERNEL ANTES
bool generar_conexiones(t_log* logger, char* ip, char* puerto, int* fd_mod2) {
    // No hardcodear, levantar de config


    *fd_mod2 = crear_conexion(logger,"KERNEL", ip, puerto );

    return *fd_mod2 != 0;
}

void send_instrucciones(t_list* lista_instrucciones,int* fd_mod2){

	instrucciones* a = malloc(sizeof(instrucciones));
	int cant_instrucciones = list_size(lista_instrucciones);
	int indice = 0;

	while(indice <= cant_instrucciones){
		a = list_get(lista_instrucciones,indice);

		if(a->id == NO_OP){
			log_warning(log_consola,"entre en NO_OP dentro de send_instrucciones");
		}
		else if(a->id == IO){
			log_warning(log_consola,"entre en I/O dentro de send_instrucciones");
		}
		else if(a->id == READ){
			log_warning(log_consola,"entre en READ dentro de send_instrucciones");
		}
		else if(a->id == COPY){
			log_warning(log_consola,"entre en COPY dentro de send_instrucciones");
		}
		else if(a->id == WRITE){
			log_warning(log_consola,"entre en WRITE dentro de send_instrucciones");
		}
		else if(a->id == EXIT){
			log_warning(log_consola,"entre en EXIT dentro de send_instrucciones");
		}

		indice++;
	}


}
