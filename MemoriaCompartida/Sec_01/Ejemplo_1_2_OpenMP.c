#include <omp.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <math.h>

double f1(double x){
    return 4*sin(x)+6;
}

double f2(double x){
    return 8*exp(-2*x);
}

double f3(double x){
    return log10(x)+cos(x);
}

double f4(double x){
    return 3*pow(x,6)+6*pow(x,5);
}

int main(int argc, char* argv[]) {

    int tid; 
    double a,b,c,d,y;

    #pragma omp parallel 
    { 
      // Obtener id de hilo  
      tid = omp_get_thread_num();

      //-- Reparto de tareas --                   
      if(tid == 0)
          a = f1(27.5);

      if(tid == 1)
          b = f2(0.43);
      
      if(tid == 2)
          c = f3(16.3);
     
      if(tid == 3)
          d = f4(2.3);
      //---------------------- 
    }
    
    //-- Ejecución serial
    y = (a*b+c)*d;
    printf("Resultado: %f\n",y);
                
    return 0;
} 
