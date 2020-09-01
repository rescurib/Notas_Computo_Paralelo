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
