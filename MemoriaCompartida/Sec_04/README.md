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
## Constructo *Critical*
