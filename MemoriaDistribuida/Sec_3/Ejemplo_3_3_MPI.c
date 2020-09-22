#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define filas 8
#define cols  10


int main(int argc, char **argv){
    int rank, proc_num, i,j;

    // Inicialización MPI
    MPI_Init(&argc,&argv);

    // Tamaño del comunicador
    MPI_Comm_size(MPI_COMM_WORLD,&proc_num);  
    
    // Obtener rango
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    //-- Para evitar fallas de segmentación -- 
    int filasAlloc = filas;
    
    //Se agregan más filas vacias si el número
    // original de filas no es divisible entre el
    // número de procesos ejecutados.
    if(filas % proc_num != 0 ) 
        filasAlloc = (filas/proc_num + 1)*proc_num;
    int filasPorProc = filasAlloc/proc_num;
    //----------------------------------------

    //------- Si es el proceso maestro -------
    if(rank == 0){
        // Matriz que se va a distribuir
    	int *M = (int*)malloc(filasAlloc*cols*sizeof(int));
        for(i=0;i<filas;i++)
            for(j=0;j<cols;j++)
                M[i*cols+j] = i;

        // Distribución In-place 
        MPI_Scatter(M,cols*filasPorProc,MPI_INT,
	            MPI_IN_PLACE,0,MPI_INT,0,
		    MPI_COMM_WORLD);

        printf("Proceso 0: Yo me quedo la matriz entera\n");
    }
    //-----------------------------------
    //-- Para el resto de los procesos --
    else{

        int *Mlocal = (int*)malloc(cols*filasPorProc*sizeof(int));

	MPI_Scatter(NULL,cols*filasPorProc,MPI_INT,
	            Mlocal,cols*filasPorProc,MPI_INT,0,
		    MPI_COMM_WORLD);

        printf("Proceso %d. Me toca(n) la(s) fila(s): ",rank);

        for(i=0;i<filasPorProc;i++)
        {
            printf("[");
            for(j=0;j<cols;j++)
                printf("%d,",Mlocal[i*cols+j]);
            printf("] ");
        }

        printf("\n");
    }

    // Fin de sección de intrucciones de MPI
    MPI_Finalize();
    return 0;
}
