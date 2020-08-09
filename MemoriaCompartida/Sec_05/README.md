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

# Ejemplo 5.1 N-vo número de la serie de Fibonacci recursiva
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
