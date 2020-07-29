#include<stdlib.h>

double readdouble(FILE *f) {
  double v;
  fread((void*)(&v), sizeof(v), 1, f);
  return v;
}

int readint(FILE *f) {
  int v;
  fread((void*)(&v), sizeof(v), 1, f);
  return v;
}


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

int leerVector(char *nom_archivo,double **A,int *size){
  int i;
  double* temp;

    //-- Leer desde archivo binario---    
    FILE *file = fopen(nom_archivo, "rb");
    
    *size = readint(file); // Cargar número de elementos
      
    temp = (double*) malloc((*size)*sizeof(double));
 
    //-- Cargar datos --
    for(i=0;i<*size;i++)
        temp[i] = readdouble(file);

    *A = temp;
    fclose(file);

    return 0;
}
