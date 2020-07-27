# Variables privadas y compartidas

Antes de continuar con los metodos de paralelización es importante hacer algunos ejercicios sobre el tema del alcance de variables. En la primera sección se mencionó que los hilos pueden acceder a las variables compartidas del proceso pero que también pueden tener variables locales que sólo pueden ser accedidas por los hilos que las poseen. Por defecto, la directiva *#pragma omp parallel* considera que todas las variables que aparezcan dentro de la sección que marca son variables compartidas a menos que se indique explícitamente que sean privadas. El no indicar que una variable debe actuar como privada para cada hilo cuando debería hacerlo en un algoritmo en particular, ocasionará un error lógico (no hará lo que esperas que haga) o hará que la ejecución paralela sea más lenta que su equivalente serial. Para indicar que una variable debe ser manejada como privada en una sección paralela se agrega la cláusula *private(miVar1,miVar2,...miVarN)*  a la directiva de sección paralela como se muestra en el siguiente ejemplo.

## Ejemplo 2.1: Cláusula *private*

```C
#include<omp.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc,char *argv){
   
    int a = 10;
    
    puts("------- Sección Paralela -------");
    #pragma omp parallel private(a)
    {
        a += omp_get_thread_num();
        printf("a = %d\n",a);
    }

    puts("------- Sección Serial -------");
    printf("a = %d\n",a);
    

    return 0;
}
```
Lo que hace este atributo es decirle al compilador que en la sección paralela la variable *a* acutuará como variable privada pero dejando a la variable *a* global intacta. Observese que la variable privada se inicializa en 0. Es se verifica porque se esta haciando una operación de acumulación: *a += 40* equivale *a = a + 40*. La salida del programa fue en mi caso con 4 hilos:
```
------- Sección Paralela -------
a = 0
a = 2
a = 1
a = 3
------- Sección Serial -------
a = 10

```
## Ejemplo 2.2: Cláusula *firstprivate*
¿Que ocurre si necesitamos que cada hilo mantenga el valor inicial de la sección serial? Para esto utilizamos el atributo *firstprivate*:
```C
#include<omp.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc,char *argv){
   
    int a = 10;
    
    puts("------- Sección Paralela -------");
    #pragma omp parallel firstprivate(a)
    {
        a += omp_get_thread_num();
        printf("a = %d\n",a);
    }

    puts("------- Sección Serial -------");
    printf("a = %d\n",a);
    

    return 0;
}
```
Ahora la salida será (nuevamente para 4 hilos):

```
------- Sección Paralela -------
a = 10
a = 12
a = 11
a = 13
------- Sección Serial -------
a = 10
```

Existe un cláusula de alcance más llmada *lastprivate* pero sólo puede entendenrse claramente cuando hay una distribución de trabajo así que la dejaremos para la próxima sección.

## Ejemplo 2.3 Uso de rand()
Las funciones generadoras de números pseudoaleatorios tienen dos problemas a la hora de utilizarlas en un programa paralelo. El primero es que utilizan una variable de estado para generar el número siguiente. Si se usa la función *rand()* estandar hará que un programa paralelo sea más lento que su equivalente serial porque los hilos dberán "pelear" por la misma variable compartida. El segundo problema es que además tendrán la misma semilla aletoria lo que hará que cada hilo genere el mismo número pseudoaleatorio. Estos dos probelmas se pueden solucionar de la siguiente manera:
* Sustituir rand() por [rand_r()](http://manpages.org/rand_r)
* Hacer privada la variable de estado de que utiliza rand_r(&edo)
* Realizar una operación a la variable de estado que dependa del identificador de hilo (para diverger los resultados)
```C
#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(int argc,char *argv){
   
    int seed = time(NULL); // Semilla inicial por tiempo
    float r;
    
    #pragma omp parallel firstprivate(seed,r)
    {
        seed += omp_get_thread_num();
        
        //Número aleatorio entre 0 y 1
        r = (float)rand_r(&seed) / (float)RAND_MAX;
       
        printf("r = %f\n",r);
    }
    
    return 0;
}
```
El resultado será algo como:
```
r = 0.422685
r = 0.866688
r = 0.644199
r = 0.088201
```
