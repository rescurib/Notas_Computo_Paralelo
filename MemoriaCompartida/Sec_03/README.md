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

<p align="center">
<img src="https://2.bp.blogspot.com/-kbVtAk852uc/XyDm5h7gLmI/AAAAAAAACXw/YJh4a7AUhJsMqyscgYz3dJ04zop6rSSNgCLcBGAsYHQ/s1600/matriz_vector.png" alt="alt text">
</p>

Podemos escribir una función en C que implemente el **algoritmo serial** de la multiplicación matriz-vector de la siguiente forma:
```C
void mxv(int m, int n, double *A, double *v, double *u){
    int i, j;
    for(i=0; i<m; i++){
        a[i] = 0.0;
        for (j=0; j<n; j++)
            u[i] += A[i*n+j]*v[j];
   }
}
````

Antes de continuar con la paralización hay unos cuantos recordatorios que es bueno hacer para los que siguen aprendiendo lenguaje C.

### Asignación de memoria dinámica

Cada proceso maneja un espacio de [memoria virtual](https://es.wikipedia.org/wiki/Memoria_virtual) cuya abstracción se ve así:

<p align="center">
<img src="https://i.stack.imgur.com/HOY4C.png" alt="alt text" height = 350>
</p>

Cuando se declara una variable o arreglo de la manera tradicional lo que hace el sistema operativo es colocarlas en la región llamada *stack*. Mientras estas variables no sean muy grandes no habrá mayor problema. Pero cuando se va trabajar con datos muy grandes es muy importante realizar delaraciones dinámicas de memeoria para no llenar al *stack* (cuando eso ocurre se llama *stackoverflow*). Las variables declaradas de forma dinámica se mandan al *heap*.

La manera correcta de declarar matrices en programas paralelos es como un arreglo lineal contiguo:

<p align="center">
<img src="https://3.bp.blogspot.com/-EGF6HYLyjo8/XyDzqT1ZuII/AAAAAAAACX8/diyAq83DWgUk7FvVZEycc41-Sk1l-KzzQCLcBGAsYHQ/s1600/ArregloLineal_Matriz.png">
</p>

En C hacemos una declaración de memoria dinamica con *malloc()*:

```C
double *A = (double*)malloc(m*n*sizeof(double)); 
````
Ahora ya debe quedar clara la razón de la línea: *u[i] += A[i*n+j]*v[j];*.

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
