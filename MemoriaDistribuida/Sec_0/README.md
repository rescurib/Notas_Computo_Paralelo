# Sección 0: Introducción a MPI

## Ejemplo 0.1
```C
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv){
    int rank, proc_num, i;

    // Inicialización MPI
    MPI_Init(&argc,&argv);
    
    // Obtener rango
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    // Obtener tamaño del comunicador
    MPI_Comm_size(MPI_COMM_WORLD,&proc_num);
    
    printf("Hola desde el proceso %d de %d\n",rank,proc_num);

    // Fin de sección de intrucciones de MPI
    MPI_Finalize();
    return 0;
}
```

Compilación:
```
mpicc ejemplo0_1.c -p ej01.o
```

Ejecución:
```
mpirun -np 8 ej01.o
```
