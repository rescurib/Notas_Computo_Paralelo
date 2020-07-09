# Introducción a OpenMP
## Concepto de hilo (*thread*)
A grandes rasgos, un sistema operativo es un programa que administra la ejecución de otros programas. Una de sus tareas principales es mantener una tabla de tiempos para que los recursos de una computadora sean repartidos entre varias tareas. Se le llama **proceso** a un programa independiente en ejecución. En Linux se pueden ver los procesos en ejecución con el comando "ps -aux". Pero un proceso no es necesariamente una unidad desde el punto de vista del sistema operativo; el *hilo* si lo es. Todos los procesos tienen al menos un hilo (llamado hilo principal), pero pueden dividirse en más.



OpenMP (*Open Multi-Processing*) es un API estandar para escribir aplicaciones paralelas de memoria compartida basada en el 
concepto multi-hilo bajo un modelo *fork-join*:

Todos los hilos de OpenMP tienen acceso a la lectura y escritura de variables. Esta API está basado en *directivas*. Esto significa 
la mayor parte de las istrucciones son macros que paralelizan un segmento de código de C o Fotran. Un ejemplo de directiva es:
```
#pragma omp parallel num threads(4)
```

La API provee de funciónes que realizan otras operaciones. Un manual completo de todas las directivas, funciones y atributos 
se puede encontrar en la documentación de GNU [libgomp](https://gcc.gnu.org/onlinedocs/libgomp/index.html). En estas notas se trataran 
las implementaciones más comunes y utiles para paralelizar código a nivel de hilos en un programa en C.

### Instalación
Para sistemas basados en Debian:
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
estas estiquetas para verificar que la ejecución paralela se está llevando a cabo. El programa completo es:
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
