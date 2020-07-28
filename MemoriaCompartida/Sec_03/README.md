# Paralelización de bucles *for*
La directiva *omp paralallel for* es la herrmaniebtas más cómoda de OpenMP. Mágicamente paraleliza un ciclo *for* sin hacer nada más. Hay requisitos para que esta directiva sea aplicable, pero por ahora veamos que es lo hace. Esta directiva divide el intervalo de iteración entre en el número máximo de hilos establecidos. Por ejemplo, si la variable de iteración *i* va de 0 a 100 y tenemos 4 hilos, entonces la iteración se dividirá en la siguiente forma:

* Hilo 0: 0-24
* Hilo 1: 25-49
* Hilo 2: 50-74
* Hilo 3: 75-99

La directiva se coloca justo por encima de la sentencia for:
```C
#pragma omp parallel for
for(i=0;i<100;i++){
    printf("i = %d en hilo %d\n",i,omp_get_thread_num());
}
```
Pueden verificar esto compilando y ejecutando el Ejemplo_3_1_OpenMP.c. La variable de iteración (*i* en este ejemplo) se vuelve privada automáticamente. La directiva para los bucles tambien conciderá por defecto a todas las demás varibales como compartidas por lo que se deberá indicar explicitamente como privadas aquellas que lo requieran.   
