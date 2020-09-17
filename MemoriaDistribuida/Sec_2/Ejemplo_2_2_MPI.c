#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

#define MESSTAG 0

//-- Función que devuelve el bit más significativo --
int MSB(int n) 
{
    int pos = 0;
    int bitMask = 0xFFFFFFFE; // Mascara de 32 bits
    while(n != 0) 
    {
        n = n & bitMask ;
        pos ++;
        bitMask <<= 1 ; // Desplazamiento a la izq. para
                        // incrementar el número de ceros en
                        // la mascara: 1...110 -> 1...100
    }
    return pos - 1;    // Si el argumento es 0 se retorna -1
}
//---------------------------------------------------

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
        int destID = 1;
        double data = 683.761; // Dato a trasmitir a todo el Comm.

        while (destID < num )
        {
            // subconjunto de procesos a enviar el dato
            MPI_Send (& data ,1,MPI_DOUBLE,destID,MESSTAG,MPI_COMM_WORLD) ;
            destID <<= 1;
        }
    }

    else // Para los demás
    {
        int msbPos = MSB(rank); // Bit más significativo del rango del proc.
        int srcID = rank^(1 << msbPos); //Rango de la fuente del envío

        printf ("Recibiendo en %i desde %i \n" , rank , srcID ) ;

        double data ;
        MPI_Recv(&data,1,MPI_DOUBLE,srcID,MESSTAG,MPI_COMM_WORLD,&status);
        printf("Node #%i recibido: %lf \n",rank,data);

        // Calcular id's de destino
        int destID = rank | (1 << (msbPos + 1));
        while (destID < num)
        {
            MPI_Send(&data,1,MPI_DOUBLE,destID,MESSTAG,MPI_COMM_WORLD);
            msbPos ++;
            destID = rank | (1 << (msbPos + 1));
        }
    }

    // Fin de sección de intrucciones de MPI
    MPI_Finalize();
    return 0;
}


