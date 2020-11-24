// SLAVE PROCESS

// C
#include <stdio.h>

// MPI
#include <mpi.h>


/*
    Un camion ocupa 2 espacios en el parking
    Necesito: 
    - Id del proceso o algo parecido que lo identifique
    - Operacion a realizar, primero sera entrada, luego salida
    - Tipo de vehiculo, en este caso no cambiara -> camion
*/

int main(int argc, char* argv[]){
    // Declaracion de variables antes de iniciar
    int recibido = -1; // Sera si se ha aparcado o no
    int enviado[3]; // [id hilo][operacion][tipo de vehiculo]

    // Inicializacion MPI
    MPI_Init(&argc, &argv);

    MPI_Barrier(MPI_COMM_WORLD);

    enviado[0] = MPI_Comm_rank(MPI_COMM_WORLD); // Sera el id
    enviado[1] = 0; // 0 = aparcar, 1 = salir. Se deja en 0 ya que primero siempre se aparca
    enviado[2] = 1; // 0 = coche, 1 = camion, siempre coche en este fichero

    while(1){
        // Pedir plaza
        printf("Camion %d quiere aparcar...", enviado[0]);
        while(recibido == -1){
            MPI_Send(enviado, 3, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Recv(&recibido, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
        printf("Camion %d aparcado", enviado[0]);
        // Cambio de variables
        enviado[1] = 1; // Aparcado
        // Esperar en el parking
        sleep(5);
        // Pedir salir
        printf("Camion %d quiere salir...", enviado[0]);
        MPI_Send(enviado, 3, MPI_INT, 0, 0, MPI_COMM_WORLD);
        // Cambio variables
        enviado[1] = 0;
        recibido = -1; // Cambiado a mano ya que al salir del parking el orquestador no envia nada, TODO mas adelante
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // Fin
    MPI_Finalize();
}
