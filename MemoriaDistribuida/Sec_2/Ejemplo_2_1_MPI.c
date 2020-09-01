#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 100 // 1000 elementos

int main(int argc, char **argv){
    int rank, proc_num, i;

    // Estructura de estado
    MPI_Status estado;

    // Inicialización MPI
    MPI_Init(&argc,&argv);
    
    // Obtener rango
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    //---- Si es el proceso maestro ----
    if(rank == 0)
    {
        // Memoria para el buffer de envío
    	int *A = (int*) malloc(N*sizeof(int));

        //Llenado de Arreglo
        for(i=0;i<N;i++) A[i]=i;
        
        //Envío
        MPI_Send(A,N,MPI_INT,1,0,MPI_COMM_WORLD);  
    }

    //---Si es el rango 1 ---
    if(rank==1)
    {
        // Memoria para el buffer de recepción
    	int *Arec = (int*) malloc(N*sizeof(int));

        //Envío
        MPI_Recv(Arec,N,MPI_INT,0,0,MPI_COMM_WORLD,&estado); 
        
        printf("Hola, soy el proceso %d. Recibí: \n");

        //Imprimir arreglo recibido
        for(i=0;i<N;i++){
            if(i%20==0) puts("\n");
            printf("%d ",Arec[i]);
        }

        puts("\n");

    }

    // Fin de sección de intrucciones de MPI
    MPI_Finalize();
    return 0;
}
