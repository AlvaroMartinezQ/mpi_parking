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
void imprime_plazas(int plantas, int plazas, int ** parking);
int** init_parking(int plantas, int plazas);

int main(int argc, char* argv[]){
    // Declaracion de variables antes de iniciar
    int plazas = strtol(argv[1], NULL, 10);
    int plantas = strtol(argv[2], NULL, 10);

    printf("Plantas: %d, con %d plazas por planta.\n", plantas, plazas);

    int ** parking = init_parking(plantas, plazas);

    MPI_Status status;

    int aparcado = -1;

    imprime_plazas(plantas, plazas, parking);

    // Inicializacion MPI
    MPI_Init(&argc, &argv);

    // Argumentos pasados
    int recibido[3]; // [id hilo][operacion][tipo de vehiculo]

    MPI_Barrier(MPI_COMM_WORLD);

    // Programa principal
    while(1){
        MPI_Recv(recibido, 3, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

        printf("Vehiculo: %d. Entrar=0/Salir=1, operacion: %d. Coche=0/Camion=0, tipo: %d\n", recibido[0], recibido[1], recibido[2]);

        // Realizar comparaciones necesarias, primero tipo de operacion, despues tipo de vehiculo.
        if(recibido[1] == 0){ // Quiero entrar
            if(recibido[2] == 0) { // Soy un coche
                int i, j;
                int posicion[2]; // [planta][plaza]
                posicion[0] = -1;
                posicion[1] = -1;
                for(i=0; i < plantas; i++){
                    for(j=0; j < plazas; j++){
                        if(parking[i][j] == 0){
                            // Aqui somos capaces de asignar plaza
                            parking[i][j] = recibido[0]; // Se pudo asignar
                            posicion[0] = i;
                            posicion[1] = j;
                        }
                    }
                }// No se pudo asignar
                if(posicion[0] != -1){ // Se ha asignado
                    printf("Coche %d aparcado en la planta %d, en la plaza %d.\n", recibido[0], posicion[0], posicion[1]);
                    aparcado = 1;
                    MPI_Send(&aparcado, 1, MPI_INT, recibido[0], 0, MPI_COMM_WORLD);
                    aparcado = -1;
                } else if (posicion[0] == -1){ // No se ha asigando
                    printf("Coche %d no ha podido aparcar.\n", recibido[0]);
                    MPI_Send(&aparcado, 1, MPI_INT, recibido[0], 0, MPI_COMM_WORLD);
                }
            } else if (recibido[2] == 1) { // Soy un camion
                int i, j, k;
                int posicion[4]; // [planta][plaza][planta][plaza]
                posicion[0] = -1;
                posicion[1] = -1;
                posicion[2] = -1;
                posicion[3] = -1;
                for(i=0; i < plantas; i++){
                    for(j=0; j < plazas; j++){
                        k = j;
                        k++;
                        if(k < plazas){ // Un camion debe de aparcar en dos huecos contigos
                            if(parking[i][j] == 0 && parking[i][k] == 0){
                                parking[i][j] = recibido[0];
                                parking[i][k] = recibido[0];

                                posicion[0] = i;
                                posicion[1] = j;
                                posicion[2] = i;
                                posicion[3] = k; // Se pudo asignar
                            }
                        } else {
                            k = 0;
                        }
                    }
                }// No se pudo asignar
                if(posicion[0] != -1){ // Se ha asignado
                    printf("Camion %d aparcado en la planta %d, en las plazas %d y %d.\n", recibido[0], posicion[0], posicion[1], posicion[3]);
                    aparcado = 1;
                    MPI_Send(&aparcado, 1, MPI_INT, recibido[0], 0, MPI_COMM_WORLD);
                    aparcado=-1;
                } else if (posicion[0] == -1){ // No se ha asigando
                    printf("Camion %d no ha podido aparcar.\n", recibido[0]);
                    MPI_Send(&aparcado, 1, MPI_INT, recibido[0], 0, MPI_COMM_WORLD);
                }
            }
        } else if(recibido[1] == 1) { // Quiero salir
            if(recibido[2] == 0) { // Soy un coche
                int salida; // salida = 0 -> correcto, EOC -> error
                int i, j;
                for(i=0; i < plantas; i++){
                    for(j=0; j < plazas; j++){
                        if(parking[i][j] == recibido[0]) {
                            parking[i][j] = 0;
                            printf("Coche %d dejando la plaza %d en la planta %d.\n", recibido[0], j, i);
                            salida = 0; // Encontrado
                        }
                    }
                }
                salida = -1; // No encontrado
            } else if (recibido[2] == 1) { // Soy un camion
                int salida; // salida = 0 -> correcto, EOC -> error
                int i, j; 
                int k = 0;
                for(i=0; i < plantas; i++){
                    for(j=0; j < plazas; j++){
                        k = j+1;
                        if(parking[i][j] == recibido[0] && parking[i][k] == recibido[0]){
                            parking[i][j] = 0;
                            parking[i][k] = 0;
                            printf("Camion %d dejando las plazas %d y %d en la planta %d.\n", recibido[0], j, k, i);
                            salida = 0; // Encontrado
                        } else {
                            k = 0;
                        }
                    }
                }
                salida = -1; // No encontrado
            }
        }
        imprime_plazas(plantas, plazas, parking);
        sleep(1); // Para evitar mucha repeticion de imprimir el parking
    }
    // Fin
    MPI_Finalize();
}

void imprime_plazas(int plantas, int plazas, int ** parking){
    printf("Estado actual del parking: \n");
    int i, j;
    for(i=0; i < plantas; i++){
        printf("-> Planta %d: ", i);
        for(j=0; j < plazas; j++){
            printf("[%d]", parking[i][j]);
        }
        printf("\n");
    }
}

int** init_parking(int plantas, int plazas){
    int** parking;
    int i, j;
    for(i=0; i < plantas; i++){
        for(j=0; j < plazas; j++){
            parking[i][j] = 0; // 0 sera libre y cualquier otro numero aparcado [id]
        }
    }
    return parking;
}
