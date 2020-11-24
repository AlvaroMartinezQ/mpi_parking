# Makefile v1.0.0

all: orquestador coche camion

orquestador: orquestador_parking.c
	mpicc orquestador_parking.c -o orquestador

coche: coche.c
	mpicc coche.c -o coche

camion: camion.c
	mpicc camion.c -o camion

clean:
	rm orquestador
	rm coche
	rm camion