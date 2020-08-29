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

## Conceptos básicos
**Communicator Group**. Referido también como *communicator* es un conjunto de procesos. Por defecto, todos los procesos controlados por el programa son asignados a MPI_COMM_WORLD. Este comunicador puede subdividirse en grupos internos (denominados *colores*):

<p align="center">
<img src="https://www.codingame.com/servlet/fileservlet?id=14312128064536" alt="alt text" width = 250>
</p>

Los colores pueden dividirse en nuevos comunicadores con nombres definidos por el programador:

<p align="center">
<img src="https://www.codingame.com/servlet/fileservlet?id=14312136027966" alt="alt text" width = 700>
</p>

**Rank**. Es el nombre que recibe el identificar relativo al comunicador para cada proceso (es similar al identificador de hilo en OpenMP).

**Estructura elemental de un programa de MPI**. Todas las intrucciones de MPI deben estar después de la función de inicialización *MPI_Init()* y antes de *MPI_Finalize()*. 

**Ejecución multi-proceso**. La manera en que un programa de MPI se ejecuta es un poco dificil de entender al principio. El proceso padre es ejecutado en la computadora desde dónde haya sido llamado el archivo ejecutable. El segmento paralelo es copiado *N* veces y ejecutado en *N* procesos distintos que pueden estar en *K* (menor o igual a *N*) computadoras distintas conectadas en la red de trabajo (pueden estar en diferentes partes del mundo). La pregunta que nos hemos hecho todos al principio es: ¿cómo puedo hacer cosas distintas si el programa es el mismo para todos los procesos? La respuesta: siendo ingeniesos con el manejo de los rangos y con el uso de las funciones de envío y recepción de datos. 

<p align="center">
<img src="https://skirt.ugent.be/skirt9/mpi_initfinal.png">
</p>

Las flechas verticales bidireccionales representan el envío/recepción de mensajes entre los procesos.

## Ejemplo 1.1 Impresión de rangos

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

La compilación de los programas de MPI requieren el compilador mpicc (que se instaló junto con Open MPI o MPICH):
```
mpicc ejemplo0_1.c -p ej01.o
```

Para ejecutar el programa se requiere *mpirun*. El atributo *-np* define el número de procesos. En este ejemplo abriremos 8:
```
mpirun -np 8 ej01.o
```
De forma similar a lo que ocurría con las ejecuciones de programas multihilo, la ejecución de los rangos no tiene por qué ser secuencial:
```
Hola desde el proceso 0 de 8
Hola desde el proceso 3 de 8
Hola desde el proceso 7 de 8
Hola desde el proceso 6 de 8
Hola desde el proceso 4 de 8
Hola desde el proceso 5 de 8
Hola desde el proceso 1 de 8
Hola desde el proceso 2 de 8
```
## Técinas de paralelismo: SPMD y MPMD
En esta sección hemos hablado de como un mismo programa es copiado y ejecutado como procesos diferentes. Incluso con OpenMP ocurría el mismo prinicipio pero a nivel de secciones de código e hilos: una misma sección paralela era ejecutada en diferentes hilos. Cuando esta técnica se utiliza para procesar diferentes partes de un mismo conjunto de datos recibe el nombre de *Single Program Multiple Data* (SPMD). Esta técnica tiene la ventaja de ser escalable si se diseña el programa de manera ingeniosa. Es decir, si un mismo programa se le da la capacidad de auto-organizarse para repartir el trabajo entre sus gemelos. 

Cuando se permite tener programas distintos para diferentes procesos dónde cada uno procesa diferentes secciones de un mismo conjunto de datos se denomina *Multple Program Multiple Data* (MPMD). MPI también permite ejecutar programas completamente diferentes en procesos independientes de la siguiente forma:
```
mpirun -np <n_procesos> programa1 : -np <k_procesos> program2
```
Si es el esquema de asiganición es complicado, se puede escibir un script (llamado Appfile) en un archivo de texto:
```
# Los comentarios estan soportados de esta manera
# Procesos A:
-np 2 a.out
# Procesos B:
-np 2 b.out
```
y ejecutarlos de esta manera:
```
mpirun --app my_appfile
```
Esta tecnica tiene el inconveniente que es más complicada de escalar a menos que se automatice la escritura de código. Aunque tiene la ventaja de una mayor flexibilidad para dar trato muy especial a alguna sección de los datos de trabajo. 
