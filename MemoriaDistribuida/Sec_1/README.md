# Introducción a MPI

En la primera parte de estas notas se describieron programas en dónde todos los datos eran acceboles por por todos los hilos a menos que se creran copias privadas de algunas variables. Todos los programas corrían también en una misma computadora. Aquí las cosas serán diferentes por que será necesario aprender nuevas maneras de pensar y escribir algoritmos. Mientras en los programas de memoria distribuida se tenía al hilo como unidad de trabajo, en el enfoque de memoría distribuido la unidad de trabajo es el proceso. Mencionamos que el proceso es un programa administrado por el sistema operativo. El proceso tiene asignado su espacio de memoria virtual y un identificador único. Mientras un hilo puede tener acceso a las variables globales de un proceso, un proceso no puede acceder directamente a las variables de otros procesos. Para que los procesos puedan intercambiar datos deben mandarse mensajes entre ellos. La idea central en la programación paralela con memoria distribuida es el intercambio de datos entre varios procesos de manera ordenada y eficiente. Estos procesos pueden estar corriendo incluso en diferentes computadoras concectadas a través de una red.

## Message Passing Interface (MPI)
MPI es un estándar abstracto de funciones de comunicación entre procesos sobre una red de computadoras. No es en si misma una biblioteca sino un conjunto de especificaciones para poder crear una. Las siguientes implementaciones concretas si lo son:
* [Open MPI](https://www.open-mpi.org/)
* [MPICH](https://www.anl.gov/mcs/mpich-a-highperformance-portable-implementation-of-mpi)

Aunque hay diferencias especificas entre ellas, todos los ejemplos de estas notas son portables a cualqueira de esas dos implementaciones. No es necesario tener acceso a un cluster de computo para utilizarlo. Para uso cotidiano en una PC el enfoque de memoria compartida y el uso OpenMP son suficientes y más adecuados. Pero para fines de aprendizaje pueden instalar Open MPI en una PC con Debian o derivados con:

```
sudo apt-get install openmpi-bin openmpi-common openmpi-doc libopenmpi-dev 
```

Si tienen la curiosidad de construir su propio cluster con varias computadoras en red, [aquí](http://mpi-cluster.blogspot.com/p/blog-page.html) hay un tutorial para configurarlo. Para usar el API con una sola PC no es necesario configurar nada después de instalar. 


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
