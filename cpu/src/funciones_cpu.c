#include "../include/cpu.h"
#include "../include/funciones_cpu.h"


void cargar_instruccion_cpu(int id, char* nombre, uint32_t parametro1, uint32_t parametro2){
	instrucciones* estructura_instrucciones = malloc(sizeof(instrucciones));

	estructura_instrucciones->id = id;
	estructura_instrucciones->nombre = nombre;
	estructura_instrucciones->parametro1 = parametro1;
	estructura_instrucciones->parametro2 = parametro2;

	list_add(lista_instrucciones_cpu,estructura_instrucciones);
	//free(estructura_instrucciones);
}

instrucciones* fetch(pcb_cpu* pcb){
	instrucciones* a = malloc(sizeof(instrucciones));
	a = list_get(pcb->instrucciones,pcb->PC);
	return a;
}

void decode_and_execute(pcb_cpu* pcb,instrucciones* instruccion_a_decodificar){
	int co_op = instruccion_a_decodificar->id;


	switch (co_op) {
			case NO_OP:{
				tiempo_bloqueante = 0;
				log_warning(log_cpu,"Antes del sleep");
				//log_warning(log_cpu,"EL PARAMETRO 1 ES: %d",instruccion_a_decodificar->parametro1);
				int retardo_NO_OP = config_get_int_value(config_cpu,"RETARDO_NOOP");
				retardo_NO_OP = retardo_NO_OP / 1000;
				for(int i=0;i<instruccion_a_decodificar->parametro1;i++){
					sleep(retardo_NO_OP);
				}
				log_warning(log_cpu,"El program counter es: %d",pcb->PC);

				log_warning(log_cpu,"Termine NO_OP donde hago el execute");

				pcb->PC += 1;

			break;
			}
			case IO:{

				log_warning(log_cpu,"Entre en IO");
				tiempo_bloqueante = instruccion_a_decodificar->parametro1;
				log_warning(log_cpu,"El tiempo bloqueante es: %d",tiempo_bloqueante);
				pcb->PC += 1;
				//interrupcion = true; // ESTO ES DE PRUEBA, VA ESTO O NO?
				break;
			 }
			 case READ:{
				 tiempo_bloqueante = 0;
				 log_warning(log_cpu,"Entre en READ");
				 pcb->PC += 1;
				 break;
			}
			 case COPY:{
				 tiempo_bloqueante = 0;
				 log_warning(log_cpu,"Entre en COPY");
				 pcb->PC += 1;
				 break;
			}
			 case WRITE:{
				 tiempo_bloqueante = 0;
				 log_warning(log_cpu,"Entre en WRITE");
				 pcb->PC += 1;
				 break;
			}
			 case EXIT:
			 {
				 tiempo_bloqueante = 0;
				 log_trace(log_cpu,"ENTRE EN EXIT");
				 log_trace(log_cpu,"Finalizo el proceso");
				 pcb->PC += 1;
				break;
			}

			 default:
				 log_error(log_cpu, "No es una instruccion valida");
				 //log_info(log_cpu, "Cop: %d", co_op);
				 //return;
				 break;
		 }

	log_warning("Se aumento el program counter, la sig instruccion es: %d",pcb->PC);
}

bool check_interrupt(){

}
