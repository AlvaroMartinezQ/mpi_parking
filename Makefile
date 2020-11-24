# Makefile v1.0.0

all: orquestador coche camion

orquestador: orquestador_parking.c
	mpicc orquestador_parking.c -o orquestador

coche: coche.c
	mpicc coche.c -o coche

camion: camion.c
	mpicc camion.c -o camion

##########################################################################################
############ 1 orquestador(control), 10 coches(slaves), 4 camiones(slaves) ###############
############# En el fichero de config hay 15 nodos preparados para ejecucion #############
##########################################################################################

run: orquestador coche camion
	mpirun --hostfile hostfile.config -np 1 orquestador 10 2 : -np 10 coche : -np 4 camion

##########################################################################################

clean:
	rm orquestador
	rm coche
	rm camion