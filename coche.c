// SLAVE PROCESS

// C
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// MPI
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
    int recibido = -1; // Sera si se ha aparcado o no
    int enviado[3]; // [id hilo][operacion][tipo de vehiculo]
    MPI_Status status;
    // Inicializacion MPI
    MPI_Init(&argc, &argv);

    MPI_Barrier(MPI_COMM_WORLD);

    int id;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    
    enviado[0] = id; // Sera el id
    enviado[1] = 0; // 0 = aparcar, 1 = salir. Se deja en 0 ya que primero siempre se aparca
    enviado[2] = 0; // 0 = coche, 1 = camion, siempre coche en este fichero

    while(1){
        // Pedir plaza
        printf("Coche %d quiere aparcar...", enviado[0]);
        while(recibido == -1){
            MPI_Send(enviado, 3, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Recv(&recibido, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        }
        printf("Coche %d aparcado", enviado[0]);
        // Cambio de variables
        enviado[1] = 1; // Aparcado
        // Esperar en el parking
        sleep(5);
        // Pedir salir
        printf("Coche %d quiere salir...", enviado[0]);
        MPI_Send(enviado, 3, MPI_INT, 0, 0, MPI_COMM_WORLD);
        // Cambio variables
        enviado[1] = 0;
        recibido = -1; // Cambiado a mano ya que al salir del parking el orquestador no envia nada, TODO mas adelante
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // Fin
    MPI_Finalize();
}
