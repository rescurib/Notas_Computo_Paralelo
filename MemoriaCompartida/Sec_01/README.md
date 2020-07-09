# Introducción a OpenMP
## Concepto de hilo (*thread*)
A grandes rasgos, un sistema operativo (SO) es un programa que administra la ejecución de otros programas. Una de sus tareas principales es mantener una tabla de tiempos para que los recursos de una computadora sean repartidos entre varias tareas. Se le llama **proceso** a un programa independiente en ejecución. En Linux se pueden ver los procesos en ejecución con el comando "ps -aux". Pero un proceso no es necesariamente una unidad desde el punto de vista del sistema operativo; el *hilo* si lo es. Todos los procesos tienen al menos un hilo (llamado hilo principal), pero pueden dividirse en más en la siguiente estructura:

<p align="center">
<img src="https://4.bp.blogspot.com/-ToVr3duqNYQ/WWLvSYqAaBI/AAAAAAAAByc/fS4ExJtBI2AeK_6_vX1f3rBGY0aLGE8YACLcBGAs/s1600/Proceso_Hilos_de_Ejecucion.png" alt="alt text">
</p>

Todos los hilos tienen acceso a las variables globales del proceso pero cada hilo puede tener sus variables locales que solo son accesibles para sus hilos correspondientes. ¿Son los procesos e hilos ejecuciones paralelas? No necesariamente. El sistema operativo realiza una división por tiempo entre todos los procesos. Aún cuando se tenga un procesador con un solo núclo, el usuario experimenta una ejecución multi-proceso "paralela" debido a la rapidez con la que el SO alterna entre tareas. Pero si el procesador es multi-núcleo, entonces el SO puede asignar un proceso o un hilo a un núcleo particular. Sólo existe ejecución paralela entre dos hilos si cada uno se ejecuta en su propio núcleo. En otro caso, se dice que su ejecución en *concurrente*.       

## OpenMP
OpenMP (*Open Multi-Processing*) es un API estandar para escribir aplicaciones paralelas de memoria compartida basada en el 
concepto multi-hilo bajo un modelo *fork-join*:

<p align="center">
<img src="https://2.bp.blogspot.com/-Kzz9i3yd9Cw/XwaWxwcCXrI/AAAAAAAACXQ/cGX9Iqju33cfOVszt9uama15ITzCS5g6wCLcBGAsYHQ/s1600/Fork_Join.jpg" alt="alt text" width = 700>
</p>

Todos los hilos de OpenMP tienen acceso a la lectura y escritura de variables. Esta API está basado en *directivas*. Esto significa 
la mayor parte de las istrucciones son macros que paralelizan un segmento de código de C o Fotran. Un ejemplo de directiva es:
```
#pragma omp parallel num threads(4)
```

La API provee de funciónes que realizan otras operaciones. Un manual completo de todas las directivas, funciones y atributos 
se puede encontrar en la documentación de GNU [libgomp](https://gcc.gnu.org/onlinedocs/libgomp/index.html). En estas notas se trataran 
las implementaciones más comunes y utiles para paralelizar código a nivel de hilos en un programa en C.

### Instalación
Es muy probable que OpenMP ya esté instaldo en un sistema Linux. En caso de que no, para sistemas basados en Debian:
```
sudo apt install libomp-dev

```
Verificamos que OpenMP esté configurado en nuestro compilador:
```
echo |cpp -fopenmp -dM |grep -i open
```

Si todo está en orden debe mostrar algo similar a "#define _OPENMP 201511". Es impartante saber que antes de ejecutar nuestro codigo 
debemos establecer el número de hilos de la siguente manera (8 por ejemplo):

```
export OMP_NUM_THREADS=8
```
## Ejemplo 3.1: Impresión de identificadores de hilo
Cada hilo en el programa tiene un identificador o etiqueta diferente. El *Hola Mundo* de los programas parelos suele ser la impresión de
estas etiquetas para verificar que la ejecución multi-hilo o multi-proceso se este llevando a cabo de forma correcta. El programa completo es:
```C
#include <omp.h> 
#include <stdio.h> 
#include <stdlib.h> 
  
int main(int argc, char* argv[]) {

    int nthreads, tid; // Num. de hilos, identificador 
  
    
    #pragma omp parallel 
    {//-- Inicio de segmento de código paralelo --
        // Tomar identificador de hilo 
        tid = omp_get_thread_num(); 
        printf("Hola, soy el hilo: = %d\n",tid); 
        
        // Si el hilo es el maestro
        if(tid == 0){ 
            nthreads = omp_get_num_threads(); 
            printf("Número de hilos: %d\n", nthreads); 
        } 
    }//-- Fin de segmento de código 
    
    return 0;
} 
```
Compilamos desde terminal con [gcc](https://iie.fing.edu.uy/~vagonbar/gcc-make/gcc.htm):
```
gcc Ejemplo_3_1_OpenMP.c - fopenmp
```
Cuando no se pone la opción *-o* el archivo ejecutable de salida se llama por defecto *a.out*. Para ejecutar simplemente escribimos:
```
./a.out
```
Notarán que el orden de ejecución de los hilos es aleatoria. Por ejemplo, mi ejecución con 4 hilos fue:
```
Hola, soy el hilo: = 0
Número de hilos: 4
Hola, soy el hilo: = 1     
Hola, soy el hilo: = 3
Hola, soy el hilo: = 2
```
En las próximas secciones verémos como establecer puntos de sincronización.
