#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "funciones.h"
#include "apodos.h"
extern int REG[16];
extern int RAM[cantCeldas];

int lineainutil(char* linea)
{
    int pos = 0;
    while(linea[pos]==' ' || linea[pos]=='\t')
        (pos)++;
    return linea[pos]=='\n';
}


int rotulovalido(char* posiblerotulo,rotulo* rotulos,int cantRotulos, int* errorTrad)
{

    char reservados[50][27];
    strcpy(reservados[0],"STOP");
    strcpy(reservados[0x01],"MOV");
    strcpy(reservados[7],"CMP");
    strcpy(reservados[14],"JZ");
    strcpy(reservados[21],"OR");
    strcpy(reservados[0x02],"ADD");
    strcpy(reservados[8],"SWAP");
    strcpy(reservados[15],"JP");
    strcpy(reservados[22],"NOT");
    strcpy(reservados[0x03],"SUB");
    strcpy(reservados[9],"RND");
    strcpy(reservados[16],"JN");
    strcpy(reservados[23],"XOR");
    strcpy(reservados[0x04],"MUL");
    strcpy(reservados[10],"JMP");
    strcpy(reservados[17],"JNZ");
    strcpy(reservados[24],"SHL");
    strcpy(reservados[0x05],"DIV");
    strcpy(reservados[11],"JE");
    strcpy(reservados[18],"JNP");
    strcpy(reservados[25],"SHR");
    strcpy(reservados[0x06],"MOD");
    strcpy(reservados[12],"JG");
    strcpy(reservados[19],"JNN");
    strcpy(reservados[26],"SYS");
    /*lalalaAAAAAAAAAAAAAA*/    	     strcpy(reservados[13],"JL");
    strcpy(reservados[20],"AND");
    int i=1;
    int valido=1;
    while(i<=27 && valido)
    {
        valido=abs(strcmp(reservados[i],posiblerotulo));
        i++;
    }

    i=0;
    while(valido && i<cantRotulos-1)
    {

        valido=abs(strcmp(rotulos[i].nombre,posiblerotulo));
        i++;
    }
    if(valido == 0)
    {
        *errorTrad = 1;
        printf("ERROR DE DUPLICADO O PALABRA RESERVADA %s\n",posiblerotulo);
    }
    /* else if(i == cantRotulos)
     {
         *errorTrad = 1;
         printf("NO EXISTIA EL ROTULO %s\n",posiblerotulo);
     }
     */

    return valido;

}

int esEQU(char* linea)
{
    char aux[200];
    int pos=0;
    strcpy(aux,linea);
    salteaespacios(aux,&pos);
    while(aux[pos]!=' ' && aux[pos]!='\t')
        pos++;
    salteaespacios(aux,&pos);
    if(toupper(aux[pos]) == 'E' && toupper(aux[pos+1]) == 'Q' && toupper(aux[pos+2]) == 'U')
        return 1;
    else
        return 0;
}

void rotuladora(FILE* Fasm,int* cantOut,rotulo** rotulosOut, int* errorTrad)
{
    int cantRotulos=0;
    rotulo* rotulos = (rotulo*)malloc(RMax*sizeof(rotulo));


    char linea[990] = "";
    int lineaactual=0;
    int pos=0;
    int posI=0;
    while(!feof(Fasm))
    {

        pos=0;
        posI=0;
        fgets(linea,990,Fasm);
        while (linea[pos]==' ' || (int)linea[pos] == 9)  //SALTEADOR DE ESPACIOS
        {
            pos++;
            posI++;
        }
        if(linea[pos]!='*' && linea[pos]!='\\')//si no es un comentario && !esEQU(linea)
        {
            lineaactual++;
            //printf("Linea actual nro %i: %s",lineaactual,linea); //P DEBUG NO MAS

            while(linea[pos]!=':' && linea[pos]!='\n' && linea[pos]!='\0' && linea[pos]!=' ' &&(int) linea[pos] !=9)
                pos++;
            if(linea[pos] == ':')
            {
                char posiblerotulo[50];
                int largo=pos-posI;
                strncpy(posiblerotulo,(&linea[posI]),largo);
                posiblerotulo[largo]='\0'; //jaja
                strcpy(posiblerotulo,strupr(posiblerotulo));
                if(rotulovalido(posiblerotulo,rotulos,cantRotulos,errorTrad))
                {
                    char* nombre = (char*)malloc(50);
                    strcpy(nombre,posiblerotulo);

                    rotulos[cantRotulos].nombre=nombre;
                    rotulos[cantRotulos].linea=lineaactual;
                    cantRotulos++;
                }
                else
                {
                    printf("Se encontro un rotulo invalido:%s \n",posiblerotulo);
                }
            }
            else //if la palabra q siugue es EQU asdasdsadsa
            {
                int posfinaldelaprimerapalabra =pos;

                salteaespacios(linea,&pos);
                if(toupper(linea[pos]) == 'E' && toupper(linea[pos+1]) == 'Q' && toupper(linea[pos+2]) == 'U')
                {
                    pos+=3;
                    //aca arriba buscamos el nro de la constante
                    salteaespacios(linea,&pos);
                    int arg;
                    switch(linea[pos])
                    {
                    case '#':
                        sscanf(&linea[pos+1],"%d",&arg);
                        break;
                    case '@':
                        sscanf(&linea[pos+1],"%o",&arg);
                        break;
                    case '%':
                        sscanf(&linea[pos+1],"%x",&arg);
                        break;
                    case '\'':
                        arg = (int)linea[pos+1];
                        break;
                    default:
                        arg = atoi(&linea[pos]);
                    }

                    // PONER NUEVO SIMBOLO EN ROTULOS
                    char posiblerotulo[50];
                    int largo=posfinaldelaprimerapalabra-posI;
                    strncpy(posiblerotulo,(&linea[posI]),largo);
                    posiblerotulo[largo]='\0'; //jaja
                    if(rotulovalido(posiblerotulo,rotulos,cantRotulos,errorTrad))
                    {
                        char* nombre = (char*)malloc(50);
                        strcpy(nombre,posiblerotulo);
                        rotulos[cantRotulos].nombre=strupr(nombre);
                        if(arg==-1)
                            rotulos[cantRotulos].linea=12345678;
                        else
                            rotulos[cantRotulos].linea=arg;
                        cantRotulos++;
                    }
                }
                //else
                //    printf("salgo jojo\n");
            }
            if(esEQU(linea)|| lineainutil(linea))
                lineaactual--;
        }
    }
    //printf("Se encontraron %i rotulos:\n",cantRotulos);
    //for(int i=0;i<cantRotulos;i++){
    // printf("el rotulo numero %i es %s y esta en la linea numero %i\n",i,rotulos[i].nombre,rotulos[i].linea);
    *rotulosOut=rotulos;
    *cantOut=cantRotulos;
}

//aca mando todo bien a donde tiene que ir

//}
