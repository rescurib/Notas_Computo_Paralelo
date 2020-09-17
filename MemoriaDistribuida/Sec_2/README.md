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
Es importante entender que las asignaciones de memoria de los buffers de envío y recepción ocurren en procesos diferentes (o computadoras diferentes si fuera el caso de un cluster). En mi caso particular esto me fue motivo de confución al principio, pero es una idea sencilla una vez que se visualiza. Tomense en cuenta de que las visualizaciones de las salidas estándar siempre se hacen en el equipo que esté corriendo al proceso maestro.

En teoría se puede realizar computo paralelo con memoria distribuida unicamente como las 6 funciones de MPI que ya hemos visto hasta ahora:

* MPI_Init()
* MPI_Comm_rank()
* MPI_Comm_size()
* MPI_Send()
* MPI_Recv()
* MPI_Finalize()

Lo único que resta es la creatividad y la astucia del programador. Pero desarollar algorimos paralelos ya es una tarea díficil por lo que es importante ahorrar el trabajo de la implemantación de esquemas de comunicación colectiva (uno-a-muchos, muchos-a-uno) eficientes, entre otras subtareas.

## Ejemplo 2.2 Transmisión colectiva eficiente

Antes de hablar de la función MPI_Broadcast() será útil realizar un ejemplo que mostrará por qué las comunicaciones colectivas eficientes no son un asunto trivial. Supongamos que queremos enviar un número entero desde el proceso maestro hacia todos los procesos del comunicador de manera que todos los procesos tengan la misma copia de ese dato. ¿Cómo hacemos esto explotando el paralelismo de la ejecución de MPI? Solo por comparación pensemos en la peor manera de hacerlo: un ciclo for desde el proceso maestro en el que se envíe el dato de manera iterativa hacia cada uno de los procesos. Esto es una perdida de tiempo porque aquellos procesos que ya tengan el dato podrían ayudar a envier una copia al resto. Una manera ingeniosa de hacer esta comunicación colectiva es la siguiente: elijamos un número de nodos que sea múltiplo de 2, digamos 16. La distribición del dato se representa gráficamente de la siguiente manera (*Multicore and GPU Programing*, Barlas):

<p align="center">
<img src="https://1.bp.blogspot.com/-BHYeND9v-EI/X2F_iTnM3TI/AAAAAAAACas/bVjH2o1uBjUvNtLh3By-y4oTuVmQ2ld1gCLcBGAsYHQ/s16000/Broadcast_MPI_FromScratch.png">
</p>
<center>Centered text</center>

Cada proceso tiene una etiqueta en binario. Si ponen atención en esta etiqueta notarán rápidamente cual es el patrón de envío para cada proceso. La clave esta en el '1' más cernano a la izquierda (más significativo) en la etiqueta del proceso desde el que se envían las copias. El proceso maestro (0000) no tiene unos así que la secuencia inicia de esta forma: 0000 -> 0001,0010,0100,1000. Tomemos el caso de los procesos que le tocan al proceso 2: 0010 -> 0110,1010. Vamos a hora con el 5: 0101 -> 1101 ¿Observan como el 1 más significativo queda fijo y sus denstinos están determinados con despazamientos de 1's hacia la izquiera despues de el? En código nos queda:

```C
    if (rank == 0) // Si es el proceso maestro
    {
        int destID = 1;
        double data = 683.761; // Dato a trasmitir a todo el Comm.

        while (destID < num )
        {
            // subconjunto de procesos a enviar el dato
            MPI_Send (& data ,1,MPI_DOUBLE,destID,MESSTAG,MPI_COMM_WORLD ) ;
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
    ```
    
    ## Ejemplo 2.3 Comunicación colectiva con *MPI_Broadcast()*
    
    Este ejemplo es exactamente el mismo que el anterior pero simplificado con el uso de una función de comunicación colectiva definida en el estdar de MPI




