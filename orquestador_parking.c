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
void imprime_plazas(int plantas, int plazas_planta, int *** parking);
int** init_parking(int plantas, int plazas_planta);
int* asigna_plaza_coche(int *** parking, int plantas, int plazas_planta, int id);
int* asigna_plaza_camion(int *** parking, int plantas, int plazas_planta, int id);
int desasigna_plaza_coche(int *** parking, int plantas, int plazas_planta, int id);
int desasigna_plaza_camion(int *** parking, int plantas, int plazas_planta, int id);

int main(int argc, char* argv[]){
    // Declaracion de variables antes de iniciar
    int plazas = strtol(argv[1], NULL, 10);
    int plantas = strtol(argv[2], NULL, 10);

    printf("Plantas: %d, con %d plazas por planta.\n", plantas, plazas);

    int ** parking = init_parking(plantas, plazas);

    MPI_Status status;

    int aparcado;

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
                int asignado_coche[2];
                asignado_coche = asigna_plaza_coche(&parking, plazas, plantas, recibido[0]);
                if(asignado_coche[0] != -1){ // Se ha asignado
                    printf("Coche %d aparcado en la planta %d, en la plaza %d.\n", recibido[0], asignado_coche[0], asignado_coche[1]);
                    aparcado = 1;
                    MPI_Send(&aparcado, 1, MPI_INT, recibido[0], 0, MPI_COMM_WORLD);
                    aparcado = 0;
                } else if (asignado_coche[0] == -1){ // No se ha asigando
                    printf("Coche %d no ha podido aparcar.\n", recibido[0]);
                    MPI_Send(&aparcado, 1, MPI_INT, recibido[0], 0, MPI_COMM_WORLD);
                }
            } else if (recibido[2] == 1) { // Soy un camion
                int asignado_camion[4];
                asignado_camion = asigna_plaza_camion(&parking, plazas, plantas, recibido[0]);
                if(asignado_camion[0] != -1){ // Se ha asignado
                    printf("Camion %d aparcado en la planta %d, en las plazas %d y %d.\n", recibido[0], asignado_camion[0], asignado_camion[1], asignado_camion[3]);
                    aparcado = 1;
                    MPI_Send(&aparcado, 1, MPI_INT, recibido[0], 0, MPI_COMM_WORLD);
                    aparcado=0
                } else if (asignado_camion[0] == -1){ // No se ha asigando
                    printf("Camion %d no ha podido aparcar.\n", recibido[0]);
                    MPI_Send(&aparcado, 1, MPI_INT, recibido[0], 0, MPI_COMM_WORLD);
                }
            }
        } else if(recibido[1] == 1) { // Quiero salir
            if(recibido[2] == 0) { // Soy un coche
                int salida = desasigna_plaza_coche(&parking, plantas, plazas, recibido[0]); // salida = 0 -> correcto, EOC -> error
            } else if (recibido[2] == 1) { // Soy un camion
                int salida = desasigna_plaza_camion(&parking, plantas, plazas, recibido[0]); // salida = 0 -> correcto, EOC -> error
            }
        }
        imprime_plazas(plantas, plazas, &parking);
        sleep(1); // Para evitar mucha repeticion de imprimir el parking
    }
    // Fin
    MPI_Finalize();
}

void imprime_plazas(int plantas, int plazas_planta, int *** parking){
    printf("Estado actual del parking: \n");
    int i, j;
    for(i=0; i < plantas; i++){
        printf("-> Planta %d: ", i);
        for(j=0; j < plazas_planta; j++){
            printf("[%d]", parking[i][j]);
        }
        printf("\n");
    }
}

int** init_parking(int plantas, int plazas_planta){
    int** parking;
    int i, j;
    for(i=0; i < plantas; i++){
        for(j=0; j < plazas_planta; j++){
            parking[i][j] = 0; // 0 sera libre y cualquier otro numero aparcado [id]
        }
    }
    return parking;
}

int* asigna_plaza_coche(int *** parking, int plantas, int plazas_planta, int id){
    int i, j;
    int posicion[2]; // [planta][plaza]
    posicion[0] = -1;
    posicion[1] = -1;
    for(i=0; i < plantas; i++){
        for(j=0; j < plazas_planta; j++){
            if(parking[i][j] == 0){
                // Aqui somos capaces de asignar plaza
                parking[i][j] = id;

                posicion[0] = i;
                posicion[1] = j;

                return posicion; // Se pudo asignar
            }
        }
    }
    return posicion; // No se pudo asignar
}

int* asigna_plaza_camion(int *** parking, int plantas, int plazas_planta, int id){
    int i, j, k;
    int posicion[4]; // [planta][plaza][planta][plaza]
    posicion[0] = -1;
    posicion[1] = -1;
    posicion[2] = -1;
    posicion[3] = -1;
    for(i=0; i < plantas; i++){
        for(j=0; j < plazas_planta; j++){
            k = j;
            k++;
            if(k < plazas_planta){ // Un camion debe de aparcar en dos huecos contigos
                if(parking[i][j] == 0 && parking[i][k] == 0){
                    parking[i][j] = id;
                    parking[i][j] = id;

                    posicion[0] = i;
                    posicion[1] = j;
                    posicion[2] = i;
                    posicion[3] = k;

                    return posicion; // Se pudo asignar
                }
            } else {
                k = 0;
            }
        }
    }
    return posicion; // No se pudo asignar
}

int desasigna_plaza_coche(int *** parking, int plantas, int plazas_planta, int id){
    for(i=0; i < plantas; i++){
        for(j=0; j < plazas_planta; j++){
            if(parking[i][j] == id) {
                parking[i][j] = 0;
                printf("Coche %d dejando la plaza %d en la planta %d.\n", id, j, i);
                return 0; // Encontrado
            }
        }
    }
    return -1; // No encontrado
}

int desasigna_plaza_camion(int *** parking, int plantas, int plazas_planta, int id){
    int k = 0;
    for(i=0; i < plantas; i++){
        for(j=0; j < plazas_planta; j++){
            k = j+1;
            if(parking[i][j] == id && parking[i][j] == k){
                parking[i][j] = 0;
                parking[i][k] = 0;
                printf("Camion %d dejando las plazas %d y %d en la planta %d.\n", id, j, k, i);
                return 0; // Encontrado
            } else {
                k = 0;
            }
        }
    }
    return -1; // No encontrado
}