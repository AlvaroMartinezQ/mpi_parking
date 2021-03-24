// MASTER PROCESS

/*
    Controlador del parking:
        - Asignador de plaza
    Definicion de variables de:
        - Plantas
        - Plazas por planta
    Tiempo de espera dentro del parking aleatorio -> (sleep()) -> en cada proceso SLAVE
*/

// C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// MPI
#include <mpi.h>

// LOCAL


// Funciones


int main(int argc, char* argv[]){
    // Declaracion de variables antes de iniciar
    int plazas = atoi(argv[1]);
    int plantas = atoi(argv[2]);

    printf("\033[1;35m");
    printf("\nPlantas: %d, con %d plazas por planta.\n", plantas, plazas);
    printf("\033[0;0m");

    int parking[plantas][plazas];
    int i, j, k;
    int asignado = 0;
    for(i=0; i < plantas; i++){
        for(j=0; j < plazas; j++){
            parking[i][j] = 0; // 0 sera libre y cualquier otro numero aparcado [id del proceso que pide plaza]
        }
    }
    printf("\033[1;31m");
    printf("\n\nParking inicializado.\n\n");
    printf("\033[0;0m");
    MPI_Status status;

    int aparcado = -1;

    printf("\nEstado actual del parking: \n");
    printf("\033[1;33m");
    for(i=0; i < plantas; i++){
        printf("-> Planta %d: ", i);
        for(j=0; j < plazas; j++){
            printf("[%d]", parking[i][j]);
        }
        printf("\n");
    }
    printf("\033[0;0m");

    // Inicializacion MPI
    MPI_Init(&argc, &argv);

    // Argumentos pasados
    int recibido[3]; // [id hilo][operacion][tipo de vehiculo]

    MPI_Barrier(MPI_COMM_WORLD);

    // Programa principal
    while(1){
        MPI_Recv(recibido, 3, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        printf("\033[1;36m");
        printf("\nVehiculo: %d. Entrar=0/Salir=1, operacion: %d. Coche=0/Camion=1, tipo: %d\n\n", recibido[0], recibido[1], recibido[2]);
        printf("\033[0;0m");
        // Realizar comparaciones necesarias, primero tipo de operacion, despues tipo de vehiculo.
        if(recibido[1] == 0){ // Quiero entrar
            if(recibido[2] == 0) { // Soy un coche
                int posicion[2]; // [planta][plaza]
                posicion[0] = -1;
                posicion[1] = -1;
                for(i=0; i < plantas; i++){
                    for(j=0; j < plazas; j++){
                        if(parking[i][j] == 0 && asignado == 0){
                            // Aqui somos capaces de asignar plaza
                            parking[i][j] = recibido[0];
                            posicion[0] = i;
                            posicion[1] = j;
                            asignado = 1; // Se pudo asignar
                        }
                    }
                }
                asignado = 0;
                if(posicion[0] != -1){ // Se ha asignado
                    printf("\033[1;32m");
                    printf("Coche %d aparcado en la planta %d, en la plaza %d.\n", recibido[0], posicion[0], posicion[1]);
                    printf("\033[0;0m");
                    aparcado = 1;
                    MPI_Send(&aparcado, 1, MPI_INT, recibido[0], 0, MPI_COMM_WORLD);
                    aparcado = -1;
                } else if (posicion[0] == -1){ // No se ha asigando
                    printf("\033[0;31m");
                    printf("Coche %d no ha podido aparcar.\n", recibido[0]);
                    printf("\033[0;0m");
                    MPI_Send(&aparcado, 1, MPI_INT, recibido[0], 0, MPI_COMM_WORLD);
                }
            } else if (recibido[2] == 1) { // Soy un camion
                int posicion[4]; // [planta][plaza][planta][plaza]
                posicion[0] = -1;
                posicion[1] = -1;
                posicion[2] = -1;
                posicion[3] = -1;
                for(i=0; i < plantas; i++){
                    for(j=0; j < plazas; j++){
                        k = j;
                        k++;
                        if(k < plazas){ // Un camion debe de aparcar en dos huecos contiguos
                            if(parking[i][j] == 0 && parking[i][k] == 0 && asignado == 0){
                                parking[i][j] = recibido[0];
                                parking[i][k] = recibido[0];

                                posicion[0] = i;
                                posicion[1] = j;
                                posicion[2] = i;
                                posicion[3] = k;
                                asignado = 1; // Se pudo asignar
                            }
                        } else {
                            k = 0;
                        }
                    }
                }
                asignado = 0;
                if(posicion[0] != -1){ // Se ha asignado
                    printf("\033[1;32m");
                    printf("Camion %d aparcado en la planta %d, en las plazas %d y %d.\n", recibido[0], posicion[0], posicion[1], posicion[3]);
                    printf("\033[0;0m");
                    aparcado = 1;
                    MPI_Send(&aparcado, 1, MPI_INT, recibido[0], 0, MPI_COMM_WORLD);
                    aparcado=-1;
                } else if (posicion[0] == -1){ // No se ha asigando
                    printf("\033[0;31m");
                    printf("Camion %d no ha podido aparcar.\n", recibido[0]);
                    printf("\033[0;0m");
                    MPI_Send(&aparcado, 1, MPI_INT, recibido[0], 0, MPI_COMM_WORLD);
                }
            }
        } else if(recibido[1] == 1) { // Quiero salir
            if(recibido[2] == 0) { // Soy un coche
                int salida; // salida = 0 -> correcto, EOC -> error
                for(i=0; i < plantas; i++){
                    for(j=0; j < plazas; j++){
                        if(parking[i][j] == recibido[0]) {
                            parking[i][j] = 0;
                            printf("\033[1;35m");
                            printf("Coche %d dejando la plaza %d en la planta %d.\n", recibido[0], j, i);
                            printf("\033[0;0m");
                            salida = 0; // Encontrado
                        }
                    }
                }
                salida = -1; // No encontrado
            } else if (recibido[2] == 1) { // Soy un camion
                int salida; // salida = 0 -> correcto, EOC -> error
                int k = 0;
                for(i=0; i < plantas; i++){
                    for(j=0; j < plazas; j++){
                        k = j+1;
                        if(parking[i][j] == recibido[0] && parking[i][k] == recibido[0]){
                            parking[i][j] = 0;
                            parking[i][k] = 0;
                            printf("\033[1;35m");
                            printf("Camion %d dejando las plazas %d y %d en la planta %d.\n", recibido[0], j, k, i);
                            printf("\033[0;0m");
                            salida = 0; // Encontrado
                        } else {
                            k = 0;
                        }
                    }
                }
                salida = -1; // No encontrado
            }
        }
        printf("\nEstado actual del parking: \n");
        printf("\033[1;33m");
        for(i=0; i < plantas; i++){
            printf("-> Planta %d: ", i);
            for(j=0; j < plazas; j++){
                printf("[%d]", parking[i][j]);
            }
            printf("\n");
        }
        printf("\033[0;0m");
        sleep(1); // Para evitar mucha repeticion de imprimir el parking
    }
    // Fin
    MPI_Finalize();
}
