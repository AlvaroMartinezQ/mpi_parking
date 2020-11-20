// MASTER PROCESS

// C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// MPI
#include <mpi.h>

// LOCAL


/*
    Controlador del parking:
        - Salida, mayor prioridad
        - Entrada, menor prioridad
        - Asignador de plaza
    Definicion de variables de:
        - Plantas
        - Plazas por planta
    Tiempo de espera dentro del parking aleatorio -> (sleep())
*/


int main(int argc, char* argv[]){
    // Declaracion de variables antes de iniciar

    // Inicializacion
    MPI_Init(&argc, &argv);

    // Argumentos pasados
    
    // Programa principal

    // Fin
    MPI_Finalize();
}