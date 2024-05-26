#define RMax 10
#define cantCeldas 8192
#include <string.h>
#include <stdio.h>
#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

typedef struct archivo
{
    char nombre[1000];
    FILE* puntero;
    int open;
} archivo;


typedef struct rotulo
{
    char* nombre;
    int linea;
} rotulo;

int rotulovalido(char* posiblerotulo,rotulo* rotulos, int cantRotulos,int* errorTrad);
void rotuladora(FILE* Fasm,int* cantOut,rotulo** rotulosOut, int* errorTrad);
void traductora(FILE* Fasm,int cantOut,rotulo* rotulosOut, int *error);
void salteaespacios(char* linea, int* pos);
int abrirArchivos(int argc,char** argv,FILE** Fasm,FILE** Fimg);
//int abrirArchivos(int argc,char argv[][500],FILE** Fasm,FILE** Fimg);
void ejecucion();


#endif // FUNCIONES_H_INCLUDED
