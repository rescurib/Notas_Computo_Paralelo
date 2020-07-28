# Paralelización de bucles *for*
La directiva *omp paralallel for* es la herrmaniebtas más cómoda de OpenMP. Mágicamente paraleliza un ciclo *for* sin hacer nada más. Hay requisitos para que esta directiva sea aplicable, pero por ahora veamos que es lo hace. Esta directiva divide el intervalo de iteración entre en el número máximo de hilos establecidos. Por ejemplo, si la variable de iteración *i* va de 0 a 100 y tenemos 4 hilos, entonces la iteración se dividirá en la siguiente forma:

* Hilo 0: 0-24
* Hilo 1: 25-49
* Hilo 2: 50-74
* Hilo 3: 75-99

## Ejemplo 3.1: Imprimir reparto de iteraciones
La directiva se coloca justo por encima de la sentencia for:
```C
#pragma omp parallel for
for(i=0;i<100;i++){
    printf("i = %d en hilo %d\n",i,omp_get_thread_num());
}
```
Pueden verificar esto compilando y ejecutando el Ejemplo_3_1_OpenMP.c. La variable de iteración (*i* en este ejemplo) se vuelve privada automáticamente. La directiva para los bucles tambien conciderá por defecto a todas las demás varibales como compartidas por lo que se deberá indicar explicitamente como privadas aquellas que lo requieran.   

## Ejemplo 3.2: Producto Matriz-Vector
Sea *A* una matriz *nxm* y *v* un vector de dimensión *m*, el producto *u = Av* se define como:

$u_i = \sum^n_{j=0}$

### Escritura de archivos binarios desde Python
En esta sección vamos a usar vectores y matrices monstrusas por lo que vamos a escribir primero un script en Python para crear ficheros binarios. Este ejercicio tambien es intencional porque en la práctica se suele usar lenguajes como Python o R para hacer preprocesado de datos antes de ponerles todo el poder de C/C++ paralelo y es bueno saber crear ficheros densos y no archivos de texto que son muy ineficientes en memoria. Aquí una forma:
```Python
import numpy as np

n = 2000
m = 2000

A = np.random.rand(n,m)

#-- numpyarray -> Arreglo de bytes --
A_bytes = A.tobytes('C')

#-- Concatenación de bytes (nmA)
Dimbyte = np.int32(n).tobytes('C') + np.int32(m).tobytes('C')
Data_bytes = Dimbyte + A_bytes

#-- Escribir archivo de matriz --
f = open("Matriz.dat", "wb")
f.write(Data_bytes)
f.close()
```
Lo que estamos haciendo es crear un archivo en donde los primeros 4 bytes son el número de filas en formato *int*, los siguientes 4 son el número de columnas y el resto de los bytes son la matriz A en formato *double*.
