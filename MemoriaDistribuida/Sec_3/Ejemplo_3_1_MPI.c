#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define  N 16        // Número de elementos del array
#define Sz N/proc_num //Numero de elementos por proceso

int main(int argc, char **argv){
    int rank, proc_num, i;

    // Inicialización MPI
    MPI_Init(&argc,&argv);

    // Tamaño del comunicador
    MPI_Comm_size(MPI_COMM_WORLD,&proc_num);    

    // Obtener rango
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    //---- Si es el proceso maestro ----
    if(rank == 0){
        // Arrray que se va a distribuir
    	int A[N]= {6,1,8,5,7,3,1,5,9,4,1,3,5,6,8,1};

        // Distribución In-place 
        MPI_Scatter(A,Sz,MPI_INT,
	            MPI_IN_PLACE,0,MPI_INT,0,
		    MPI_COMM_WORLD);

        printf("Proceso 0: Yo me quedo el array entero\n");
    }
    //-----------------------------------
    //-- Para el resto de los procesos --
    else{
        int Alocal[Sz];
	MPI_Scatter(NULL,Sz,MPI_INT,
	            Alocal,Sz,MPI_INT,0,
		    MPI_COMM_WORLD);

        printf("Proceso %d. Me toca el segmento: ",rank);

        for(i=0;i<N/proc_num;i++)
            printf("%d,",Alocal[i]);

        printf("\n");
	
    }

    // Fin de sección de intrucciones de MPI
    MPI_Finalize();
    return 0;
}
