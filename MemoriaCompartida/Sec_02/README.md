# Variables privadas y compartidas

Antes de continuar con los metodos de paralelización es importante hacer algunos ejercicios sobre el tema del alcance de variables. En la primera sección se mencionó que los hilos pueden acceder a las variables compartidas del proceso pero que también pueden tener variables locales que sólo pueden ser accedidas por los hilos que las poseen. Por defecto, la directiva *#pragma omp parallel* considera que todas las variables que aparezcan dentro de la sección que marca son variables compartidas a menos que se indique explícitamente que sean privadas. El no indicar que una variable debe actuar como privada para cada hilo cuando debería hacerlo en un algoritmo en particular, ocasionará un error lógico (no hará lo que esperas que haga). Para indicar que una variable debe ser manejada como privada en una sección paralela se agrega la cláusula *private(miVar1,miVar2,...miVarN)*  a la directiva de sección paralela como se muestra en el siguiente ejemplo.

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
