# Sincronización

En esta seccción trataremos los constructos de OpenMP que permiten administrar los accesos a memoria desde los hilos activos en un programa. En muchas ocaciones un algoritmo requiere realizar una coordinación entre las lecturas y escrituras a las variables compartidas para garantizar la correctitud del programa.

## Constructo *Barrier*

Todas la directivas que hemos visto hasta ahora tienen una barrera de sincronización implicita. Esto significa que si un hilo terminó primero la sección o la parte de un ciclo for que le tocaba ejecutar, no avanzara hacía una nueva directiva hasta que el resto termine. Sin embargo no es raro que se requiera una barrera de sincrnización explícita dentro de una sección paralela. Veamos wl siguiente ejemplo.

### Ejemplo 4.1: Barrera de sicronización explícita
Lo que harémos en este ejemplo es simular con un retardo una tarea que toma 3 segundos en completarse que será asignada solo a la mitad de los hilos y la segunda mitad no hará nada. La barrera implicita *#pragma omp barrier* obligará a los hilos sin trabajo a esperar a que los demás hilos terminen.

```C
 #pragma omp parallel private(tid)
    { // Inicio de sección paralela
     
    tid = omp_get_thread_num();
    if (tid < omp_get_num_threads()/2 ) 
       sleep(3);
    printf("Hilo %d antes de la barrera en %f s\n",tid,omp_get_wtime()-start);

    #pragma omp barrier

    printf("Hilo %d después de la barrera en %f s\n",tid,omp_get_wtime()-start);
   } // Fin de sección paralela
```
El resultado de su ejecuión para mi fue:
```
Hilo 2 antes de la barrera en 0.000189 s
Hilo 3 antes de la barrera en 0.000189 s
Hilo 0 antes de la barrera en 3.000277 s
Hilo 1 antes de la barrera en 3.000277 s
Hilo 0 después de la barrera en 3.000306 s
Hilo 1 después de la barrera en 3.000313 s
Hilo 2 después de la barrera en 3.000314 s
Hilo 3 después de la barrera en 3.002478 s
```
## Cáusula *nowait* (rompimiento de barrera implícita)
El caso contrario al anterior es cuando no hay conflictos lógicos en el programa y podemos darnos el lujo de que no haya hilos ociciosos. Para esto utilizamos la cláusula *nowait*.
```C
a();
#pragma omp parallel
{
    b();
    #pragma omp for nowait
    for (int i = 0; i < 10; ++i) {
        c(i);
    }
    d();
}
z();
```

El siguiente diagrama (tomado de [aquí](http://ppc.cs.aalto.fi/ch3/nowait/)) muestra la distribución de tareas en el tiempo (de izquierda a derecha):

<p align="center">
<img src="http://ppc.cs.aalto.fi/ch3/parallel_for_nowait.png" alt="alt text" width = 800>
</p>

Noten como los hilos 2 y 3 no esperan a 0 y 1 a que terminer su parte del for. Simplemente continuan con la función d();

## Constructo *Critical*
Es es un constructo multilinea. Lo que hace es obligar a todos los hilos en la sección a tomar turno para ejecutar la secuencia de instrucciónes dentro de la subsección *critical*.
### Ejemplo 4.2 Impresión multiple ordenada
Tomemos el siguiente segmento de código:
```C
    #pragma omp parallel private(tid)
    { // Inicio de sección paralela
       tid = omp_get_thread_num();
 
       printf("Hilo %d: Línea 1\n",tid);
       if(tid==2) sleep(1);
       if(tid==1) sleep(3);
       printf("Hilo %d: Línea 2\n",tid);
       if(tid==0) sleep(2);
       printf("Hilo %d: Línea 3\n",tid);
   } // Fin de sección paralela
   ```
   Aquí hemos simulado nuevamente diferentes tiempos de ejecucíón de la misma sección para diferentes hilos. La continuidad de la salida será afectada por estas diferencias:
   ```
Hilo 0: Línea 1
Hilo 0: Línea 2
Hilo 2: Línea 1
Hilo 3: Línea 1
Hilo 3: Línea 2
Hilo 3: Línea 3
Hilo 1: Línea 1
Hilo 2: Línea 2
Hilo 2: Línea 3
Hilo 0: Línea 3
Hilo 1: Línea 2
Hilo 1: Línea 3
```
Esto se resuelve estableciendo una región crítica:
```C
    #pragma omp parallel private(tid)
    { // Inicio de sección paralela
       tid = omp_get_thread_num();
       #pragma omp critical
       {
          printf("Hilo %d: Línea 1\n",tid);
          if(tid==2) sleep(1);
          if(tid==1) sleep(3);
          printf("Hilo %d: Línea 2\n",tid);
          if(tid==0) sleep(2);
          printf("Hilo %d: Línea 3\n",tid);
      }
   } // Fin de sección paralela
```

Este fue el resultado en mi ejecución:
```
Hilo 1: Línea 1
Hilo 1: Línea 2
Hilo 1: Línea 3
Hilo 3: Línea 1
Hilo 3: Línea 2
Hilo 3: Línea 3
Hilo 2: Línea 1
Hilo 2: Línea 2
Hilo 2: Línea 3
Hilo 0: Línea 1
Hilo 0: Línea 2
Hilo 0: Línea 3
```
En este ejemplo hay dos cosas importantes que hay que comentar. Lo que se busca con la sección critica no es establecer un orden secuencial entre los hilos (en mi ejecion primero fue 1, luego 3, 2 y 0) sino un candado de turno en una región especifica en el programa. La segunda es que este candado puede tener el costo de volver más lenta la ejecución de esa sección. Depende del programador diseñar un algoritmo que equlibre esta desventaja en favor de la correctitud lógica del resultado. 

## Constructo *Atomic*
Este contructo es similar a *critical* pero sólo aplica a operaciones sobre una misma variable. Este ejemplo demostrará por que es necesario.

### Ejemplo 4.3 Reducción explcítica
En la sección anterior se mostró como ralizar operaciones de reducción automaticamente con la cláusula *reduction*. En este ajemplo haremos la misma operación sin útilizar esa cláusula pero sólo para fines explicativos. Veamos el siguiente código que repite 5 veces la suma paralela de los primeros 500,000 enteros e imprime el resultado cada vez:
```C
    for(k=0;k<5;k++){
        suma = 0;
        #pragma omp parallel for
        for(i=0;i<500000;i++)
            #pragma omp atomic
            suma = suma + i;

        printf("Suma total: %d\n",suma);
    }
```
Todo parece estar en orden hasta que lo ejecutamos:
```
Suma total: -1870266438
Suma total: -517572889
Suma total: 526321667
Suma total: 1148492490
Suma total: 76774993
```
¡5 resultados diferentes! Peor aún, hay resultados negativos. Si se vuelve a ejecutar se encontrarán con resultados aleatorios. La razon por la que ocurre esto tal vez no sea evidente. Este problema aparace porque en varios momentos en la ejecución un hilo esta apunto de hacer la operación Suma_nueva = Suma_vieja + i_local cunado otro hilo llega y cambia el valor de suma_vieja o de suma_nueva. [Este vídeo](https://www.youtube.com/watch?v=7ENFeb-J75k) del canal *Computerphile* explica muy bien esta situación. Para evitarla necesitamos poner un candado temporal a la variable compartida Suma para que sólo pueda ser accedida y modificada por un hilo a la vez. Arreglamos el código de la siguiente manera:
```C
for(k=0;k<5;k++){
        suma = 0;
        #pragma omp parallel for
        for(i=0;i<500000;i++)
            #pragma omp atomic
            suma = suma + i;

        printf("Suma total: %d\n",suma);
    }
 ```
    Ahora los resultados son consistentes:
```
Suma total: 445698416
Suma total: 445698416
Suma total: 445698416
Suma total: 445698416
Suma total: 445698416
```
Las condiciones de uso completas de este contructo se pueden encontrar en la documentación de [Atomic](https://www.openmp.org/spec-html/5.0/openmpsu95.html#x126-4840002.17.7).
