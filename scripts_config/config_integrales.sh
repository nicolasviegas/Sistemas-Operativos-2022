#!/usr/bin/env bash

#Creo los archivos de configuracion de MEMORIA

cd ..
cd memoria/

FILE=/home/utnso/tp-2022-1c-yaguarethreads-/memoria.config
if test -f "$FILE"; then
    rm memoria.config
fi

touch memoria.config

echo 'PUERTO_ESCUCHA=8002' >memoria.config
echo 'TAM_MEMORIA=10240' >>memoria.config
echo 'TAM_PAGINA=256' >>memoria.config
echo 'ENTRADAS_POR_TABLA=4' >>memoria.config
echo 'RETARDO_MEMORIA=500' >>memoria.config
echo 'ALGORITMO_REEMPLAZO=CLOCK-M' >>memoria.config
echo 'MARCOS_POR_PROCESO=4' >>memoria.config
echo 'RETARDO_SWAP=1000' >>memoria.config
echo 'PATH_SWAP=/home/utnso/swap' >>memoria.config

cd .. 

#Creo los archivos de configuracion de KERNEL

cd kernel/

FILE=/home/utnso/tp-2022-1c-yaguarethreads-/kernel.config
if test -f "$FILE"; then
    rm kernel.config
fi

touch kernel.config

echo 'IP_MEMORIA=192.168.3.x' >kernel.config
echo 'PUERTO_MEMORIA=8002' >>kernel.config
echo 'IP_CPU=192.168.3.x' >>kernel.config
echo 'PUERTO_CPU_DISPATCH=8001' >>kernel.config
echo 'PUERTO_CPU_INTERRUPT=8005' >>kernel.config
echo 'PUERTO_ESCUCHA=8000' >>kernel.config
echo 'ALGORITMO_PLANIFICACION=SRT' >>kernel.config
echo 'ESTIMACION_INICIAL=50000' >>kernel.config
echo 'ALFA=0.2' >>kernel.config
echo 'GRADO_MULTIPROGRAMACION=6' >>kernel.config
echo 'TIEMPO_MAXIMO_BLOQUEADO=1000' >>kernel.config

cd .. 


#Creo los archivos de configuracion de CPU

cd cpu/

FILE=/home/utnso/tp-2022-1c-yaguarethreads-/cpu.config
if test -f "$FILE"; then
    rm cpu.config
fi

touch cpu.config

echo 'ENTRADAS_TLB=2' >cpu.config
echo 'REEMPLAZO_TLB=LRU' >>cpu.config
echo 'RETARDO_NOOP=100' >>cpu.config
echo 'IP_MEMORIA=192.168.3.x' >>cpu.config
echo 'PUERTO_MEMORIA=8002' >>cpu.config
echo 'PUERTO_ESCUCHA_DISPATCH=8001' >>cpu.config
echo 'PUERTO_ESCUCHA_INTERRUPT=8005' >>cpu.config

cd ..