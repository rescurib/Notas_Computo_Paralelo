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

Todo muy sencillo hasta acá, pero en este punto hay una mala noticia que dar. Aunque la directiva de paralelización de ciclos es muy simple y directa, es dificil configurarla de manera que tenga alto desempeño para algoritmos que realicen operaciones fundamentales. Esto no se suele mencionar en los tutoriales y como resultado Stack Overflow está lleno de gente preguntado porque sus ejemplos, de apariencia inofensiva, tienen igual o peor desempeño que sus versiones seriales. Normalmente los ejemplos que se dan al principio son operaciones matriciales u operaciones a lo largo de un array y este tipo de algoritmos son vulnerables a problemas como desalineamiento de datos o [false sharing](https://docs.oracle.com/cd/E19205-01/819-5270/aewcy/index.html) a la hora de implementarlos con varios hilos. Se pueden corregir pero hay que tener un entendimiento de arquitectura de computadoras y esos temas están fuera del scope de estas notas. Pero es necesario advertirlo para evitarles frustración innecesaria. La buena noticia es que en la práctica muchas veces no es necesario hacer un paralelismo a ese nivel de operaciones fundamentales. Ya existen bibliotecas que implementan operaciones matriciales eficientes como BLAS, LAPAC y ATLAS. Lo importante es concentrarse a nivel de subprocesos (en el contexto de un procedimiento o simulación numérica). Los siguientes dos ejemplos tal vez no serán tan sencillos o intutivos para todos, pero mostrarán despeño paralelo funcional.     

## Paréntesis: Memoria dinámica y lectura/escritura de ficheros

Antes de continuar con paralización de código hay unos cuantos recordatorios que es bueno hacer para los que siguen aprendiendo lenguaje C.

### Asignación de memoria dinámica

Cada proceso maneja un espacio de [memoria virtual](https://es.wikipedia.org/wiki/Memoria_virtual) cuya abstracción se ve así:

<p align="center">
<img src="https://i.stack.imgur.com/HOY4C.png" alt="alt text" height = 350>
</p>

Cuando se declara una variable o arreglo de la manera tradicional lo que hace el sistema operativo es colocarlas en la región llamada *stack*. Mientras estas variables no sean muy grandes no habrá mayor problema. Pero cuando se va trabajar con datos muy grandes es muy importante realizar delaraciones dinámicas de memeoria para no llenar al *stack* (cuando eso ocurre se llama *stack overflow*). Las variables declaradas de forma dinámica se mandan al *heap*.

La manera correcta de declarar matrices en programas paralelos es como un arreglo lineal contiguo:

<p align="center">
<img src="https://3.bp.blogspot.com/-EGF6HYLyjo8/XyDzqT1ZuII/AAAAAAAACX8/diyAq83DWgUk7FvVZEycc41-Sk1l-KzzQCLcBGAsYHQ/s1600/ArregloLineal_Matriz.png">
</p>

En C hacemos una declaración de memoria dinamica con *malloc()*:

```C
double *A = (double*)malloc(m*n*sizeof(double)); 
````
Ahora ya debe quedar clara la razón de la línea: *u[i] += A[i\*n+j]\*v[j];*.

### Escritura de archivos binarios desde Python
En esta sección y la siguiente vamos a usar vectores y matrices monstrusas por lo que vamos a escribir primero un script en Python para crear ficheros binarios que los contengan y que serán leídos desde nuestros programas en C. Este ejercicio tambien es intencional porque en la práctica se suele usar lenguajes como Python o R para hacer preprocesado de datos antes de ponerles todo el poder de C/C++ paralelo y es bueno saber crear ficheros densos y no archivos de texto que son muy ineficientes en memoria. Aquí una forma:
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

### Lectura de ficheros binarios desde C

Vamos a utilizar las siguientes funciones para convertir bytes crudos en datos *double* e *int*:

```C
float readfloat(FILE *f) {
  float v;
  fread((void*)(&v), sizeof(v), 1, f);
  return v;
}

int readint(FILE *f) {
  int v;
  fread((void*)(&v), sizeof(v), 1, f);
  return v;
}
```
En el código en Python hemos establecido que los primeros bytes son para *m*, los isguientes para *n* y el resto para el contenido de la matriz. Entonces la función completa para la lectura es:

```C
int leerMatriz(char *nom_archivo,double **A,int *m,int *n){
    int i,j;
    double* temp;

    //-- Leer desde archivo binario---    
    FILE *file = fopen(nom_archivo, "rb");
    
    *m = readint(file); // Cargar número de filas
    *n = readint(file); // Cargar número de columnas
      
    temp = (double*) malloc((*n)*(*m)*sizeof(double));
 
    //-- Cargar datos --
    for(i=0;i<*m;i++)
       for(j=0;j<*n;j++)
           temp[i*(*n)+j] = readdouble(file);

    *A = temp;
    fclose(file);

    return 0;
}
```

La lectura para vectores es similar pero se dejará como referencia revisar el archivo "leerBin.h".

## Ejemplo 3.2 Algoritmo *bubble sort* paralelo
He elegido este algorimo porque es muy lento y hará más notorio el *speedup* al implementar su versión paralela. [Aquí](https://www.youtube.com/watch?v=P_xNb8nFgmA) hay una explicación del algoritmo pero el siguiente gif vale más que mil palabras:

<p align="center">
<img src="https://upload.wikimedia.org/wikipedia/commons/0/06/Bubble-sort.gif">
</p>

Implementado en C nos queda:
```C
void intercambiar(double *a, double *b){ 
    double t = *a; 
    *a = *b; 
    *b = t; 
} 

//------ BubbleSort Serial -----
int bubbleSort_Serial(double v[],int N){
    int i,j;
    for (i = 0; i < N-1; i++) 
        for (j = 0; j < N-i-1; j++) 
            if (v[j] > v[j+1]){
                intercambiar(&v[j+1],&v[j]);                   
            }
    }
```
He hecho estas notas pensando en mis amigos de ciencias naturales y sé que no todo el mundo está familiarizado con la [complejidad computacional](https://www.fing.edu.uy/inco/cursos/teocomp/unidad02/transpSesion07.pdf). Pero creo que deteniendonos a pensar un momento podemos notar que si tenemos dos *for's* anidados que corren a lo largo de 0 hasta N realizando una operación por iteración, tendremos N\*N operaciones. A muy grandes rasgos, por eso se dice que este algoritmo tiene una complejidad de O(N^2) dónde N en este caso sería el número de elementos que hay que ordenar. ¿Pueden imaginarse a ustedes mismos ordenando N cartas de baraja inglesa con este algoritmo? ¿Qué pasaría si tuvieran un número K ~= N de personas ayudandoles de manera que cada persona realice una comparación e intercambio simultaneamente? Lo que ocurriría es que ahora el tiempo de ejecución crecería linealmente con respecto al número de cartas y creo que es intutitvo el hecho de que tardarían menos tiempo que si lo hicieran solos.

Para la versión paralela usaremos la mágica directiva de OpenMP que ya sabemos usar pero vamos a tener que hacer algunos cambios muy sutiles pero muy importantes en el algoritmo para volverlo paralelo. 

```C
//------ BubbleSort Paralelo -----
int bubbleSort_Paralelo(double v[],int N){
    int i=0, j=0; 
    int inicio;
    for(i=0;i<N-1;i++){
	inicio = i % 2; 
	#pragma omp parallel for default(none),shared(v,inicio,N)
	for(j=inicio;j<N-1;j+=2){
	    if(v[j] > v[j+1]){
		intercambiar(&v[j],&v[j+1]);
	    }
	}
    }      
}
```
