# Envío, recepción y *broadcast*

El concepto fundamental de MPI es la comunicación inter-proceso. En esta sección veremos las 3 maneras más básicas para envíar y recibir mensajes. Aunque suene repetitivo, quiero hacer enfasis en que un programa ejecutado en MPI sigue el método SPMD: un mismo programa diferentes tareas. Las funciones incluidas en el API ayudan enormemente en este proceso. Cada una de ellas sabrá que hacer en su proceso si se utilizan correctamente. Comenzaremos describiendo la sintaxis de las funciones para el envío y para la recepción de mensajes (envío y recepción de datos).

```C
int MPI_Send(
    void ∗buf,             // Dirección del buffer de envío
    int count ,            // Número de elmentos a enviar (si el buffer es un array, este argumeto es su tamaño)
    MPI_Datatype datatype, // Tipo de los datos a enviar (dado en formatos MPI)
    int dest,              // Rango de proceso de destino
    int tag,               // Etiqueta de tipo de mensaje
    MPI_Comm comm          // Comunicador
)
```
```C
int MPI_Recv( 
    void *buf.             // Dirección del buffer de recepción
    int count,             // Número de elementos a recibir
    MPI_Datatype datatype, // Tipo de los datos a recibir
    int source,            // Rango de proceso de origen
    int tag,               // Etiqueta de tipo de mensaje
    MPI_Comm comm,         // Comunicador
    MPI_Status ∗status     // Dirección de estructura de estado
)
```
| Tipos MPI       | Tipos C       | 
| ----------------|:-------------:| 
| MPI_CHAR        | char          |
| MPI_INT         | int           |
| MPI_LONG        | long int      |
| MPI_DOUBLE      | double        |
| MPI_LONG_DOUBLE | long double   |

Pueden consular la lista completa de tipos [aquí](https://www.mpich.org/static/docs/latest/www3/Constants.html).

## Ejemplo 2.1 Envío recepción de un array de enteros
En este ejemplo vamos a envíar desde el proceso 0 un array de enteros de 100 elementos.

```C
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
    }
```
Es importante entender que las asignaciones de memoria de los buffers de envío y recepción ocurren en procesos diferentes (o computadoras diferentes si fuera el caso de un cluster).
