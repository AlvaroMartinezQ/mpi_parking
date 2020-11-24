// SLAVE PROCESS

#include <stdio.h>
#include <mpi.h>


/*
    Un coche ocupa 1 espacio en el parking
    Necesito: 
    - Id del proceso o algo parecido que lo identifique
    - Operacion a realizar, primero sera entrada, luego salida
    - Tipo de vehiculo, en este caso no cambiara -> coche
*/

int main(int argc, char* argv[]){
    // Declaracion de variables antes de iniciar
    int recibido; // Sera si se ha aparcado o no
    int enviado[3]; // [id hilo][operacion][tipo de vehiculo]
    MPI_Status status;

    // Inicializacion MPI
    MPI_Init(&argc, &argv);

    MPI_Barrier(MPI_COMM_WORLD);

    // TODO

    // Fin
    MPI_Finalize();
}
