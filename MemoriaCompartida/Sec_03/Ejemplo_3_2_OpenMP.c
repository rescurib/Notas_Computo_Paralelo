#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
//#include <time.h>
#include "leerBin.h"

# define N 20

void mxv_serial(int m, int n, double *A, double *v, double *u);
void mxv_paralela(int m, int n, double *A, double *v, double *u);

int main(int argc,char *argv){

    int i,k,n,m,vlen;
    double *A,*v;

    //-- Variables de medición
    double start,end,T,Tsum;

    //start = clock();
    leerMatriz("Matriz.dat",&A,&m,&n);
    leerVector("Vector.dat",&v,&vlen);
    
    //-- Vector resultado
    double *u = (double*) malloc(vlen*sizeof(double)); 
    
    Tsum = 0;
    for(i=0;i<10;i++){
        start = omp_get_wtime();
        mxv_serial(m,n,A,v,u);
        end = omp_get_wtime();
        Tsum += end-start;
    }

    T = Tsum/20.0; //T. serial promedio 
    printf("T. de ejecución serial: %f s\n",T);
    
    // Verificar resultado (solo para m pequeñas)
    //for(i=0;i<n;i++)
    //    printf("%f\n",u[i]);  
    
    Tsum = 0;
    for(i=0;i<N;i++){
        start = omp_get_wtime();
        mxv_paralela(m,n,A,v,u);
        end = omp_get_wtime();
        Tsum += end - start;
    }
    
    T = Tsum/20.0;
    printf("T. de ejecución paralela: %f s\n",T); 

    // Verificar resultado (solo para m pequeñas)
    //for(i=0;i<n;i++)
    //    printf("%f\n",u[i]);  
}
// -- Función de multiplicación Matriz-Vector serial
void mxv_serial(int m, int n, double *A, double *v, double *u){
    int i, j;
    double parsum;
    for(i=0; i<m; i++){
        u[i] = 0.0;
        for (j=0; j<n; j++)
            u[i] += A[i*n+j]*v[j];
    }

}

// -- Función de multiplicación Matriz-Vector paralela
void mxv_paralela(int m, int n, double *A, double *v, double *u){
    int i, j;
    double parsum;
  
    #pragma omp parallel for private(j,parsum) 
    for(i=0; i<m; i++){
        parsum = 0.0;
        for (j=0; j<n; j++)
            parsum += A[i*n+j]*v[j];
        u[i] = parsum;
    }

}
