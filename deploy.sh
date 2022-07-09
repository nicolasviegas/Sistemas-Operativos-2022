#!/usr/bin/env bash
#Compilar e instalar las commons

#Compilar e instalar las shared
(cd shared/Debug && make install)
#Compilar resto de los proyectos
(cd consola/ && make build) 
(cd kernel/ && make build) 
(cd cpu/ && make build) 
(cd memoria/ && make build) 