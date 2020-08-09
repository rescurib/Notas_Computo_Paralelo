# Calendarización
En la sección 3 aprendimos a paralelizar ciclos *for* con la directiva *omp parrallel for*. Recordemos que la distribución del intervalo iteración se realizó (por *default*) en subintervalos consecutivos: hilo 0: \[0,2,3\], hilo 1: \[4,5,6\], etc. Mientras el tiempo de ejecución de cada hilo en su subintervalo sea casi el mismo, la carga de trabajo estará bien balanceada (no habrá ningún hilo que trabaje de más o de menos de forma significativa). El problema es que dependiendo del algoritmo que se esté utilizando, el tiempo de ejecución para cada hilo puede depender de los valores de indice que le tocan. Tomaré nuevamente un ejemplo y un diagrama de ejecuciones de las hermosas notas del curso [Programming Parallel Computers](http://ppc.cs.aalto.fi/) de la Universidad Aalto de Finlandia. Veamos el siguiente segmento de código:
```C
a();
#pragma omp parallel for
for (int i = 0; i < 16; ++i) {
    w(i);
}
z();
```
La función w(x) es una función cuyo tiempo de ejecución es directamente proporcional al valor de su argumento. Como no hay ninguna cláusula de calendarización en la directiva, el diagrama de ejecución paralela se verá así:
<p align="center">
<img src="http://ppc.cs.aalto.fi/ch3/parallel_for_uneven.png" alt="alt text" width = 800>
</p>

Observen que la mayor parte de la carga de trabajo se le deja al hilo 3. ¿Cómo podemos arrerlar esto? Con la cláusula **schedule**.

## Calendarización estática
Este tipo de calendarización reparte los indices del intervalo de iteración entre los hilos no deforma contigua sino con un espaciado de *N* pasos llamado *chunk size*. La sintaxis de la cláusula es *schedule(static,<chunk size>)*. Por ejemplo, para 4 hilos en un intervalo de iteración de 0 a 7 con un *chunk size* de 1 el reparto sería:
* Hilo 0: 0,4
* Hilo 1: 1,5
* Hilo 2: 2,6
* Hilo 3: 3,7

### Ejemplo 5.1 N-vo número de la serie de Fibonacci recursiva
Tomemos la versión recursiva del algoritmo de generación de numeros de Fibonacci (muy ineficiente, por cierto, lo cual es bueno para nuestros fines didácticos):
```C
int fibbonacci(int n){
   if(n == 0){
      return 0;
   } else if(n == 1){
      return 1;
   } else{
      return (fibbonacci(n-1) + fibbonacci(n-2));
   }
}
```
Se puede observar que el número de recursiones es dependiente del valor de *n*. Veamos que ocurre si hacemos lo siguiente:
```C
    float thilo,tglobal,start = omp_get_wtime();
    #pragma omp parallel private(tid,thilo)
    {
        tid = omp_get_thread_num();

        #pragma omp for nowait
        for(i=0;i<42;i++){
            fibonacci(i);
        }

        thilo = omp_get_wtime()-start;
        printf("Hilo %d: %f s \n",tid,thilo);
     }
     tglobal = omp_get_wtime()-start;
     printf("Tiempo global: %f s\n",tglobal);
```
   La salida del programa en mi máquina fue:
```
Hilo 0: 0.000017 s 
Hilo 1: 0.000479 s 
Hilo 2: 0.053513 s 
Hilo 3: 19.142344 s 
Tiempo global: 19.142582 s

```
Lo que demuestra lo que ya sospechabamos. Todo el trabajo se le dejo al hilo 3. Resolveremos esto de la siguiente forma:

```C
#pragma omp for nowait schedule(static,1)
        for(i=0;i<42;i++){
            fibonacci(i);
        }
```
Ahora el resultado es:
```
Hilo 0: 4.312376 s 
Hilo 1: 7.885260 s 
Hilo 2: 9.211325 s 
Hilo 3: 13.015017 s 
Tiempo global: 13.015040 s
```
Ahora la repartición es más justa. Prueben incrementando el tamaño del *chunk*. El balanceo no puede ser perfecto debido a que la complejidad del algoritmo recursivo de Fibonacci no es lineal.

## Calendarización dinámica
En el ejemplo anterior teníamos la ventaja de tener de antemano una intución de los tiempos de ejecución para cada hilo en la versión no calendarizada del programa. ¿Qué ocurre cuando sabemos que no serán iguales pero no tenemos idea de la relación entre el tiempo y el índice de iteración? Aquí es cunado debemos usar una calendarización dinamica con *schedule(dynamic,<chunk size>)*. Este tipo de calendarización reparte los indices según la disponibilidad de los hilos en en tiempo de ejecución.
    
### Ejemplo 5.2 Función de retardo irregular

Tenemos ahora la siguiente función de tiempos de ejecución irregular:
```C
void f(int x){
    int T[16] = {1,20,30,5,4,20,7,2,1,2,8,5,4,2,8,2};
    sleep(T[x]);
}
```
Los resultados para una iteracción de 0 a 15 sin calendarizar fue:
```
Hilo 2: 16.001228 s 
Hilo 3: 16.001217 s 
Hilo 1: 33.001244 s 
Hilo 0: 56.001286 s 
Tiempo global: 56.001312 s
```
Veamos que ocurre si hacemos esto:
```C
#pragma omp for nowait schedule(dynamic,1)
        for(i=0;i<16;i++){
            f(i);
        }
```

Los resultados ahora fueron:
```
Hilo 1: 29.003302 s 
Hilo 3: 29.003345 s 
Hilo 2: 30.003145 s 
Hilo 0: 33.003731 s 
Tiempo global: 33.003757 s
```
Mucho mejor balanceados. La calendarización dinámica no puede encontrar la distribución óptima pero puede mejorar muchisimo el balanceo de carga entre los hilos.


