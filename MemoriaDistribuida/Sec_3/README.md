# Scatter
Se le llama *scattering* a un tipo particular de comunicación colectiva en donde se realiza una distribución de un conjunto de datos entre todos los procesos de un comunicador. Aquí un ejemplo simple donde tenemos un un arreglo de enteros de 4 enteros y 4 procesos. Al terminar el *scattering* cada proceso imprimirá el segmento que le tocó.

<p align="center">
<img src="https://raw.githubusercontent.com/rescurib/Notas_Computo_Paralelo/master/MemoriaDistribuida/Sec_3/MPI_Scatter.png">
</p>

El prototipo de la función MPI_Scatter es el siguiente:

```C
MPI_Scatter(
    void* send_data,            //Puntero al dato que va a enviarse
    int send_count,             //Número de elementos a enviar a cada proceso
    MPI_Datatype send_datatype, //Tipo de dato a enviar
    void* recv_data,            //Puntero de recepción 
    int recv_count,             //Número de elementos que van a recibirse
    MPI_Datatype recv_datatype, //Tipo de dato de recepcción
    int root,                   //Fuente de emisión
    MPI_Comm communicator)      //Comunicador
```
## Ejemplo 3.1 Distribuir un arreglo de enteros
En este ejemplo vamos a segmentar un arreglo de 16 elementos en 4 secciones que vamos a distribuir entre 4 procesos.

```C
#define  N 16        // Número de elementos del array
#define Sz N/proc_num //Numero de elementos por proceso
```

```C
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
```
