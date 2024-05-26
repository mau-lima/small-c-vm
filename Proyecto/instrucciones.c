#include "instrucciones.h"
#include <stdlib.h>
#include "funciones.h"
#include "apodos.h"
extern int REG[16];
extern int RAM[cantCeldas];
extern archivo archivos[10];

void setCC(int resultado) //el bit de la derecha del todo es 1 si el resultado es 0
{
    //el de la izq del todo es 1 si es negativo
    CC = 0;
    CC = (resultado==0) | ((resultado<0) <<31);
}


int ADD(int *op1,int *op2)
{
    *op1 += *op2;
    setCC(*op1);
    return 0;
}
int MOV(int *op1,int *op2)
{
    *op1 = *op2;
    return 0;
}
int SUB(int *op1,int *op2)
{
    *op1 -= *op2;
    setCC(*op1);
    return 0;
}
int MUL(int *op1,int *op2)
{
    *op1 *= *op2;
    setCC(*op1);
    return 0;
}
int DIV(int *op1,int *op2)
{
    if(*op2 == 0)
    {
        printf("\n\nDIVISION POR CERO!");
        return 1;
    }
    else
    {
        (*op1) /= (*op2);
        setCC(*op1);
        return 0;
    }
}
int MOD(int *op1,int *op2)
{
    if(*op2 == 0)
    {
        printf("\n\nDIVISION POR CERO!");
        return 1;
    }
    else
    {
        setCC(*op1/(*op2));//asi
        *op1 = (*op1) % (*op2);
        return 0;
    }

}
int CMP(int *op1,int *op2)
{
    setCC((*op1) - (*op2)); // jeje
    return 0;
}
int SWAP(int *op1,int *op2)
{
    int aux;
    aux = *op1;
    *op1 = *op2;
    *op2 = aux;
    return 0;
}
int RND(int *op1,int *op2)
{
    *op1 = rand() % (*op2) + 1;
    return 0;

}
int JMP(int *op1, int *op2)
{
    IP = (*op1-1)*3;
    return 0;
}
int JE(int *op1,int *op2)
{
    if(*op1 == AX)
        IP = (*op2-1)*3;
    return 0;
}
int JG(int *op1,int *op2)
{
    if(*op1 > AX)
        IP = (*op2-1)*3;
    return 0;
}
int JL(int *op1,int *op2)
{
    if(*op1 < AX)
        IP = (*op2-1)*3;
    return 0;
}
int JZ(int *op1,int *op2)
{
    if((CC & 0b1) == 1)
        IP = (*op1-1)*3;
    return 0;
}
int JP(int *op1,int *op2)
{
    if(CC==0)
        IP = (*op1-1)*3;
    return 0;
}
int JN(int *op1,int *op2)
{
    if (CC & (0b1<<31))
        IP = (*op1-1)*3;
    return 0;
}
int JNZ(int *op1,int *op2)
{

    if((CC & 0b1) == 0b0)
        IP = (*op1-1)*3;
    return 0;
}
int JNP(int *op1,int *op2)
{
    if(CC != 0)
        IP = (*op1-1)*3;
    return 0;
}
int JNN(int *op1,int *op2)
{
    if(!(CC & (0b1<<31)))
        IP = (*op1-1)*3;
    return 0;
}
int AND(int *op1,int *op2)
{
    *op1 = (*op1) & (*op2);
    setCC(*op1);
    return 0;
}
int OR(int *op1,int *op2)
{
    *op1 = (*op1) | (*op2);
    setCC(*op1);
    return 0;
}
int NOT(int *op1,int *op2)
{
    *op1= ~(*op1);
    setCC(*op1);
    return 0;
}
int XOR(int *op1,int *op2)
{
    *op1 = (*op1) ^ (*op2);
    setCC(*op1);
    return 0;
}
int SHL(int *op1,int *op2)
{
    *op1 = (*op1) << (*op2);
    setCC(*op1);
    return 0;
}
int SHR(int *op1,int *op2)
{
    *op1 = (*op1) >> (*op2);
    setCC(*op1);
    return 0;
}
int SYS(int *op1, int *op2)
{
    int i =0;
    int errorarch=0;
    int findearch=0;
    switch(*op1)
    {
    case 1: //read AX=formato CX=largo DX=direccion RAM donde exribir
    {
        //mostrar la direc

        if (AX & (1<<8)) //es un numerito!!
        {
            char palabras[50][50];
            for(i=0; i<CX; i++)
            {
                if (AX & (1 <<12))
                    printf("[0000 %04d]:",DS+DX+i);
                fgets(palabras[i],50,stdin);
            }


            switch(AX & 0xF)
            {
            case 0b1:
            {
                for(i=0; i<CX; i++)
                    RAM[DS+DX+i]=strtol(palabras[i],NULL,10);
                break;
            }
            case 0b100:
            {
                for(i=0; i<CX; i++)
                    RAM[DS+DX+i]=strtol(palabras[i],NULL,8);
                break;
            }
            case 0b1000:
            {
                for(i=0; i<CX; i++)
                    RAM[DS+DX+i]=strtol(palabras[i],NULL,16);
                break;
            }
            default:
            {
                printf("Error de lectura: no se especifico una base correcta para los numeros leidos en AX");
            }
            }
        }
        else
        {
            char palabra[500];
            fgets(palabra,500,stdin);
            if(CX != -1)
            {
                ///
                for(i=0; i<CX; i++) //grabar caracter a caracter
                    RAM[DS+DX+i]=palabra[i];
                RAM[DS+DX+i]='\0';
            }
            else
            {
                while(palabra[i] != '\n') //grabar caracter a caracter
                {
                    RAM[DS+DX+i]=palabra[i];
                    i++;
                }
                RAM[DS+DX+i]='\0';
            }
        }

        break;
    }
    case 2:
    case 3:
    {
        if(*op1 == 3)
        {
            printf("[DS]: %d\n",DS);
            printf("[IP]: %d\n",IP);
            printf("[CC]: %d\n",CC);
            printf("[AX]: %d\n",AX);
            printf("[BX]: %d\n",BX);
            printf("[CX]: %d\n",CX);
            printf("[DX]: %d\n",DX);
            printf("[EX]: %d\n",EX);
            printf("[FX]: %d\n",FX);
            printf("[SS]: %d\n",SS);
            printf("[SP]: %d\n",SP);
            printf("[BP]: %d\n",BP);
        }
        for(i=0; i<CX; i++)
        {
            if(AX & 0x1000)
                printf("[0000 %04d]:",DS+DX+i);

            if((AX & 1) == 0b1) //DECIMAL
                printf("%d ",RAM[DS+DX+i]);

            if((AX & 0b10) == 0b10) //BINARIO
                fwrite(&RAM[DS+DX+i],sizeof(int),1,stdout);

            if((AX & 1<<2) ==1<<2) //OCTAL
                printf("%o ",RAM[DS+DX+i]);

            if((AX & 1<<3) == 1<<3) //HEXA
                printf("%x ",RAM[DS+DX+i]);

            if((AX & 1<<4) == 1<<4) //CARACTER
                printf("%c",(RAM[DS+DX+i] & 0xFF));

            if(AX & (1<<8)) ////Endline
                printf("\n");
        }
        if(CX == -1)
        {
            int i = 0;
            while(RAM[DS+DX+i] != '\0')
            {
                printf("%c",(RAM[DS+DX+i] & 0xFF));
                i++;
            }
        }

        break;
    }

    case 0x3D:
    {
        archivos[BX].open = 1;
        archivos[BX].puntero = fopen(archivos[BX].nombre,"r+");
        if(archivos[BX].puntero == NULL)
            archivos[BX].puntero = fopen(archivos[BX].nombre,"w+");

        if(archivos[BX].puntero==NULL)
            errorarch=1;
    } //open file
    break;

    case 0x3E: //close file
    {
        if(archivos[BX].open != 0)
        {
            archivos[BX].open=0;
            fclose(archivos[BX].puntero);
        }
        else
            errorarch = 1;
    }
    break;

    case 0x3F: //read file
    {
        if(archivos[BX].open != 0 && !feof(archivos[BX].puntero))
        {
            // HUEVO
            if ((AX & 0xF) == 0x2)  //CUENTA CELDAS
            {
                int cantLeidos = 0;
                if(CX!=-1)
                {
                    while(!feof(archivos[BX].puntero) && cantLeidos<CX)
                    {
                        fread(&RAM[DS+DX+cantLeidos],sizeof(int),1,archivos[BX].puntero);
                        cantLeidos++;
                    }
                    if(feof(archivos[BX].puntero))
                        findearch=1;
                }
                else
                {
                    int j=0;
                    while(!feof(archivos[BX].puntero))
                    {
                        fread(&RAM[DS+DX+j],sizeof(int),1,archivos[BX].puntero);
                        j++;
                    }
                    findearch=1;
                }

            }
            else if((AX & 0xF0) == 0x10 && ((AX & 0x0100) == 0)) //CUENTA BYTES 0x10
            {
                int cantLeidos = 0;
                fflush(archivos[BX].puntero);
                if(CX!=-1)
                {
                    while(!feof(archivos[BX].puntero) && cantLeidos<CX)
                    {
                        fread(&RAM[DS+DX+cantLeidos],1,1,archivos[BX].puntero);
                        cantLeidos++;
                    }
                    if(feof(archivos[BX].puntero))
                        findearch=1;
                }
                else
                {
                    char linea[50] = "";
                    fgets(linea,50,archivos[BX].puntero);
                    int i =0;
                    while(linea[i] != '\n' && linea[i]!='\0') // o '\0'
                    {
                        RAM[DS+DX+i] = linea[i];
                        i++;
                    }
                    RAM[DS+DX+i] = '\0';
                    if(feof(archivos[BX].puntero))
                        findearch=1;
                }

            }
            else if (AX & 0x0100) // CUENTA LINEAS
            {
                int cuentaN = 0;
                int i =0;
                if(AX & 0x0010) //leyendoe se string
                {
                    //ORGA1 SI ESTA EN 0110 HAY Q LEER CX RENGLONES, SACAR \N Y PONER \0
                    //SI 0010 CON CX EN UN NUMERO ENTONCES LEE CX NUMERITO, SI ESTA EN -1 LEE TODO EL RENGLON
                    //X si ax ES 0002 ENTONCES SI CX ES -1 DUMPEA TODO E LARCHIOVOP
                    (CX==-1)?cuentaN=-2:0;
                    while(cuentaN < CX && !feof(archivos[BX].puntero))
                    {
                        char dou = RAM[DS+DX+i];
                        fread(&dou,sizeof(char),1,archivos[BX].puntero);
                        RAM[DS+DX+i] = dou;
                        if(dou == '\n')
                        {
                            RAM[DS+DX+i] = '\0';
                            cuentaN++;
                            if(CX==-1)
                                cuentaN--;//solo cuentaN si hay q contar, si no deja q salga por fin de archivo
                        }
                        i++;
                    }
                    if(feof(archivos[BX].puntero))
                        findearch=1;
                    //ORGA2
                }
                else //leyendo numeritos
                {
                    while(cuentaN < CX && !feof(archivos[BX].puntero))
                    {
                        char palabra[50] ="hola";
                        int numero;
                        fgets(palabra,500,archivos[BX].puntero);
                        // CHOCLO DE FORMATO (numero = algo)
                        switch(AX & 0xF)
                        {
                        case 0b1:
                        {
                            numero=strtol(palabra,NULL,10);
                            break;
                        }
                        case 0b100:
                        {
                            numero=strtol(palabra,NULL,8);
                            break;
                        }
                        case 0b1000:
                        {
                            numero=strtol(palabra,NULL,16);
                            break;
                        }
                        default:
                        {
                            printf("Error de lectura: no se especifico una base correcta para los numeros leidos en AX");
                        }
                        }
                        RAM[DS+DX+i] = numero;
                        cuentaN++;
                        i++;
                    }
                    if(feof(archivos[BX].puntero))
                        findearch=1;
                }


            }
            else
                printf("Error de formato en la lectura de archivo: el valor de AX es invalido");
            // fin HUEVO
        }
        else if(archivos[BX].open == 0)
            errorarch=1;
        else
            findearch=1;
    }
    break;

    case 0x40://write file

        //hola pego aca
        if(archivos[BX].open != 0)
        {
            // HUEVO
            if ((AX & 0xF) == 0x2)  //CUENTA CELDAS
            {
                fwrite(&RAM[DS+DX],sizeof(int),CX,archivos[BX].puntero);
            }
            else if((AX & 0xF0) == 0x10 && ((AX & 0x0100) == 0)) //CUENTA BYTES 0x10
            {
                //lala
                if(CX==-1)
                {
                    fflush(archivos[BX].puntero);
                    char linea[50] = "";
                    int i =0;
                    while(linea[i]!='\0') // o '\0'
                    {
                        linea[i] = RAM[DS+DX+i];
                        i++;
                    }
                    fputs(linea,archivos[BX].puntero);


                }
                else
                {
                    fflush(archivos[BX].puntero);
                    char linea[50] = "";
                    int i =0;
                    while(i<CX) // o '\0'
                    {
                        linea[i] = RAM[DS+DX+i];
                        i++;
                    }
                    fputs(linea,archivos[BX].puntero);

                }


                //lolo
            }
            else       // CUENTA LINEAS
            {
                int i =0;

                if(AX & 0x0010) //leyendose string
                {
                    int cuentaCero=0;
                    while(cuentaCero < CX)
                    {
                        char dou = RAM[DS+DX+i];
                        fwrite(&dou,sizeof(char),1,archivos[BX].puntero);
                        if(dou == '\0')
                        {
                            fseek(archivos[BX].puntero,-1,SEEK_CUR);
                            char barraene = '\n';
                            fwrite(&barraene,sizeof(char),1,archivos[BX].puntero);
                            cuentaCero++;
                        }
                        i++;
                    }
                }
                else //pegando esos numeritos
                {
                    int cantEscritos=0;
                    while(cantEscritos < CX)
                    {
                        char palabra[50] ="hola";
                        // CHOCLO DE FORMATO (numero = algo)
                        switch(AX & 0xF)
                        {
                        case 0b1:
                        {
                            itoa(RAM[DS+DX+cantEscritos],palabra,10);
                            break;
                        }
                        case 0b100:
                        {
                            itoa(RAM[DS+DX+cantEscritos],palabra,8);
                            break;
                        }
                        case 0b1000:
                        {
                            itoa(RAM[DS+DX+cantEscritos],palabra,16);
                            break;
                        }
                        default:
                        {
                            printf("Error de formato en la escritura de archivo: el valor de AX es invalido");
                        }
                        }
                        fputs(strcat(palabra,"\n"),archivos[BX].puntero);
                        cantEscritos++;
                    }
                }



            }
            // fin HUEVO
        }
        else if(archivos[BX].open == 0)
            errorarch=1;
        //aca termine
        break;

    case 0x42: //seek
        if(archivos[BX].open != 0)
        {
            if(AX &0x002)
                fseek(archivos[BX].puntero,4*CX,DX);
            else if((AX &  0x10) && ((AX & 0x0100) == 0))
                fseek(archivos[BX].puntero,CX,DX);
            else if(AX & 0x0100)
            {
                char palabraTemp[500];
                int i;
                for(i = 0; i<CX; i++)
                    fgets(palabraTemp,500,archivos[BX].puntero);

            }

            if(feof(archivos[BX].puntero))
                findearch=1;
        }
        else
            errorarch=1;
        break;
    default:
    {
        printf("SYS con parámetro inexistente");
    }
    }
    CC = findearch | (errorarch<<31);
    return 0;
}
int STOP(int *op1, int *op2)
{
    IP=-900;
    return 0;
}

/*
SEGUNDA PARTE  SEGUNDA PARTE  SEGUNDA PARTE  SEGUNDA PARTE  SEGUNDA PARTE  SEGUNDA PARTE  SEGUNDA PARTE  SEGUNDA PARTE  SEGUNDA PARTE
SEGUNDA PARTE  SEGUNDA PARTE  SEGUNDA PARTE  SEGUNDA PARTE  SEGUNDA PARTE  SEGUNDA PARTE  SEGUNDA PARTE  SEGUNDA PARTE  SEGUNDA PARTE
*/

int PUSH(int *op1, int *op2)
{
    if(SP > 0)
    {
        SP--;
        RAM[SS+SP]=*op1;
        return 0;
    }
    else
        printf("STACK OVERFLOW: Pila llena y se intento un PUSH\n");
    return 1;
}
int POP(int *op1, int *op2)
{
    if(SP != cantCeldas)
    {
        *op1 = RAM[SS+SP];
        SP++;
        return 0;
    }
    printf("STACK UNDERFLOW: Pila vacía y se intento un POP\n");
    return 1;


}
int PUSHALL(int *op1, int *op2)
{

    if(SP >= 6)
    {
        int i;
        for(i = 10; i<16; i++)
        {
            SP--;
            RAM[SS+SP]=REG[i];
        }
        return 0;
    }
    else
        printf("STACK OVERFLOW: Pila llena y se intento un PUSHALL\n");
    return 1;
}
int POPALL(int *op1, int *op2)
{
    //if(SP != cantCeldas)
    if(SS+SP+6<cantCeldas+1)
    {
        int i;
        for(i = 15; i>9 && (SS+SP < cantCeldas); i--)
        {
            REG[i]=RAM[SS+SP];
            SP++;
        }
        return 0;
    }
    else

        printf("STACK UNDERFLOW: Pila sin suficientes elementos y se intento un POPALL\n");
    return 1;
}

int CALL(int *op1, int *op2)
{
    int hola= IP+1-1;
    PUSH(&hola,NULL);
    JMP(op1,op2);
    return 0;
}
int RET(int *op1, int *op2)
{
    int hola =-1;
    POP(&hola,NULL);
    hola = (hola/3) +1;
    JMP(&hola,NULL);
    return 0;
}

//ZONA string  ZONA string  ZONA string ZONA string ZONA string ZONA string ZONA string ZONA string ZONA string ZONA string ZONA string ZONA string
//ZONA string  ZONA string  ZONA string ZONA string ZONA string ZONA string ZONA string ZONA string ZONA string ZONA string ZONA string ZONA string
//ZONA string  ZONA string  ZONA string ZONA string ZONA string ZONA string ZONA string ZONA string ZONA string ZONA string ZONA string ZONA string
//ZONA string  ZONA string  ZONA string ZONA string ZONA string ZONA string ZONA string ZONA string ZONA string ZONA string ZONA string ZONA string
//ZONA string  ZONA string  ZONA string ZONA string ZONA string ZONA string ZONA string ZONA string ZONA string ZONA string ZONA string ZONA string

char* armaPal(int* op2)
{

    int i = 0;
    int* posactual = op2;
    char* palabra = (char*)malloc(900*sizeof(char));
    while(((*posactual) & 0xFF) != '\0')
    {
        palabra[i] = (char)((*posactual) & 0xFF);
        posactual++;
        i++;
    }
    palabra[i] = '\0';
    return palabra;
}


int SLEN(int *op1, int *op2) //ej SLEN AX [20]
{

    *op1 = strlen(armaPal(op2));
    return 0;
}

int SCMP(int *op1, int *op2)
{
    setCC(strcmp(armaPal(op1),armaPal(op2)));
    return 0;
}
int SMOV(int *op1, int *op2)
{
    int* posactual = op2;
    int i=0;
    while(((*posactual) & 0xFF) != '\0')
    {
       // RAM[i+(*op1)] =(*posactual);
       *(op1+i)=(*posactual);
        posactual++;
        i++;
    }
    RAM[i+(*op1)] ='\0';
    return 0;
}

