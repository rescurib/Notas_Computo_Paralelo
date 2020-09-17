#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

#define MESSTAG 0

int main(int argc,char** argv)
{
    int rank,num,i;
    
    // Inicio de instrucciones de MPI
    MPI_Init(&argc,&argv);

    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&num);

    if (rank == 0) // Si es el proceso maestro
    {
        double dato = 683.761; // Dato a trasmitir a todo el Comm.
        MPI_Bcast(&dato,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
    }

    else // Para los demás
    {
        double dato = 0; 
        MPI_Bcast(&dato,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
        printf("Dato recibido: %lf en %d. \n",dato,rank);
    }

    // Fin de sección de intrucciones de MPI
    MPI_Finalize();
    return 0;
}

