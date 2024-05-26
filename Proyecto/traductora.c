#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "funciones.h"
#include "apodos.h"
extern int REG[16];
extern int RAM[cantCeldas];
extern int pilaDefault;

int getNroRegistro(char* instruccion)  //VIENE CON CORCHETES
{

    //AX 5720
    //BX 5808
    //CX 5896
    //DX 5984
    //EX 6072
    //FX 6160
    //DS 5644
    //IP 5840
    //CC 4489
    //SS 6889
    //BP 5280
    //SP 6640
    long long int p1 = toupper(instruccion[1]);
    long long int p2 = toupper(instruccion[2]);
    long long int valor = (long long int)(p1*p2);
    int resultado = -1;
    switch(valor)
    {
    case 5720:
        resultado=10;
        break;
    case 5808:
        resultado=11;
        break;
    case 5896:
        resultado=12;
        break;
    case 5984:
        resultado=13;
        break;
    case 6072:
        resultado=14;
        break;
    case 6160:
        resultado=15;
        break;
    case 5644:
        resultado=2;
        break;
    case 5840:
        resultado=8;
        break;
    case 4489:
        resultado=9;
        break;
    case 6889:
        resultado=4;
        break;
    case 5280:
        resultado=7;
        break;
    case 6640:
        resultado=6;
        break;
    }
    return resultado;
}

int buscaCODOP(char* codop)
{
    codop=strupr(codop);
    char reservados[0x90][50];
    strcpy(reservados[0x01],"MOV");
    strcpy(reservados[0x13],"CMP");
    strcpy(reservados[0x24],"JZ");
    strcpy(reservados[0x32],"OR");
    strcpy(reservados[0x02],"ADD");
    strcpy(reservados[0x17],"SWAP");
    strcpy(reservados[0x25],"JP");
    strcpy(reservados[0x33],"NOT");
    strcpy(reservados[0x03],"SUB");
    strcpy(reservados[0x19],"RND");
    strcpy(reservados[0x26],"JN");
    strcpy(reservados[0x34],"XOR");
    strcpy(reservados[0x04],"MUL");
    strcpy(reservados[0x20],"JMP");
    strcpy(reservados[0x27],"JNZ");
    strcpy(reservados[0x37],"SHL");
    strcpy(reservados[0x05],"DIV");
    strcpy(reservados[0x21],"JE");
    strcpy(reservados[0x28],"JNP");
    strcpy(reservados[0x38],"SHR");
    strcpy(reservados[0x06],"MOD");
    strcpy(reservados[0x22],"JG");
    strcpy(reservados[0x29],"JNN");
    strcpy(reservados[0x81],"SYS");
    /*lalalaAAAAAAAAAAAAAA*/    	   strcpy(reservados[0x23],"JL");
    strcpy(reservados[0x31],"AND");
    strcpy(reservados[0x8F],"STOP");


    //SEGUNDA PARTEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
    strcpy(reservados[0x40],"CALL");
    strcpy(reservados[0x48],"RET");
    strcpy(reservados[0x44],"PUSH");
    strcpy(reservados[0x45],"POP");
    strcpy(reservados[0x46],"PUSHALL");
    strcpy(reservados[0x47],"POPALL");
    strcpy(reservados[0x50],"SLEN");
    strcpy(reservados[0x51],"SMOV");
    strcpy(reservados[0x53],"SCMP");

    int i=0;
    while(i<=0x8F && strcmp(codop,reservados[i]))//horrible
        i++;


    if (!strcmp(codop,reservados[i]))
        return i;
    else
        return -1;

}




int buscaRotulo(char* entrada,int cantRotulos,rotulo* rotulos)
{
    int i=0;
    while(i<cantRotulos && strcmp(entrada,rotulos[i].nombre)!=0)
        i++;
    if (i<cantRotulos && strcmp(strupr(entrada),rotulos[i].nombre) == 0)
        return rotulos[i].linea;
    else
        return -1;



}


void purificadora(FILE* Fasm)
{
    fseek(Fasm, 0, SEEK_SET);
    char linea[990] ="";
    FILE* tmp = fopen("aaa.tmp","wt");
    int pos;
    while(!feof(Fasm))
    {
        char hola = fgetc(Fasm);

        if(hola==(char) -1) //FEOF
            linea[0]=hola;  //is
        else                //evil
            linea[0]=hola;  //no se enojen

        fgets(&linea[1],990,Fasm);
        pos =0;
        salteaespacios(linea,&pos);

        if(linea[0] == '\\' && linea[1] == '\\')
        {
            int actual= 2;
            while(linea[actual]>'9' || linea[actual] < '0')
                actual++;
            pilaDefault=strtol(&linea[actual],NULL,10);

        }

        if(linea[pos]!='*')
            while(linea[pos]!='\n' && linea[pos]!='\0')
            {
                if((linea[pos] == ','  && linea[pos-1] != (char)39) || linea[pos]==(char)9)
                    linea[pos] = ' ';
                if(linea[pos-1] == (char)39 && linea[pos-3]==(char)39)
                    linea[pos] = ' ';
                pos++;
            }
        if (hola!=(char)-1 && linea[0]!='\\')
            fputs(linea,tmp);
        else
            fputs("    ",tmp);


    }
    fclose(tmp);
    fclose(Fasm);
    Fasm = fopen("aaa.tmp","rt");

}

void escribeRAM(char instruccion[][50],int cantPalabras,int cantRotulos, rotulo* rotulos, int *error)
{
    int i = 1;
    int ins, op, arg=0,arg1=0,arg2=0,top1=0,top2=0,errorLinea = 0;
    op=buscaCODOP(instruccion[0]);
    if(op==-1)
    {
        *error = 1;
        errorLinea = 1;
        // printf("FFFF FFFF FFFF FFFF\n");
    }
    while(i < cantPalabras && *error != 1 && errorLinea != 1)
    {
        ins = 0;
        if(instruccion[i][0] == '-' || instruccion[i][0] == '#' || instruccion[i][0] == '@' || instruccion[i][0] == '%' || instruccion[i][0] == '\'' || (instruccion[i][0]<='9' && '0'<=instruccion[i][0])) //INMEDIATO
        {
            switch(instruccion[i][0])
            {
            case '#':
                sscanf(&instruccion[i][1],"%d",&arg);
                break;
            case '@':
                sscanf(&instruccion[i][1],"%o",&arg);
                break;
            case '%':
                sscanf(&instruccion[i][1],"%x",&arg);
                break;
            case '\'':
                arg = (int)instruccion[i][1];
                break;
            //case '-': arg = -1*atoi(&instruccion[i][1]);
            //  break;

            default:
                arg = atoi(instruccion[i]);
            }
            switch(i)
            {
            case 0:
            {
                *error = 1;
                errorLinea = 1;
                i = 10;
            }
            break;
            case 1:
            {
                arg1 = arg;
                top1 = 0b00;
            }
            break;
            case 2:
            {
                arg2 = arg;
                top2 = 0b00;
            }
            break;
            }
        }
        else if(toupper(instruccion[i][1]) == 'X' || !strcmp(strupr(instruccion[i]),"SP") || !strcmp(strupr(instruccion[i]),"BP"))   //REGISTRO
        {
            switch(i)
            {
            case 0x0:
            {
                *error = 1;
                errorLinea = 1;
                i = 10;
            }
            break;
            case 0x1:
            {
                top1 = 0b01;
                if(toupper(instruccion[i][1]) == 'X')
                    arg1 = (toupper(instruccion[i][0]) - 55);    //Convierte string(num) en int
                else if(strcmp(strupr(instruccion[i]),"SP") == 0)
                    arg1=6;
                else
                    arg1=7;
            }
            break;
            case 0x2:
            {
                top2 = 0b01;
                if(toupper(instruccion[i][1]) == 'X')
                {
                    arg2 = (toupper(instruccion[i][0]) - 55);

                }
                else if(strcmp(strupr(instruccion[i]),"SP") == 0)
                    arg2=6;
                else
                    arg2=7;

            }
            break;

            }

        }
        else
        {
            if(instruccion[i][0] == '[' && instruccion[i][strlen(instruccion[i]) - 1] == ']') //DIRECTO E INDIRECTO
            {
                int arg = -1;
                int esindirecto=0;
                int esEQU = 0;
                int esEQU1 = 0;
                char* aux1 = (char*) malloc(50*sizeof(char));
                strcpy(aux1,&instruccion[i][1]);
                aux1[strlen(aux1) - 1] = '\0';
                esEQU1 = buscaRotulo(strupr(aux1),cantRotulos,rotulos);
                if(toupper(instruccion[i][1])>='A'  && toupper(instruccion[i][1])<='Z' && esEQU1 == -1) //los indirectos
                {
                    esindirecto=1;
                    int nroregistro=getNroRegistro(instruccion[i]);
                    int offset=0;
                    if(instruccion[i][3] =='+' || instruccion[i][3]=='-')
                    {
                        char* aux = (char*) malloc(50*sizeof(char));
                        strcpy(aux,&instruccion[i][4]);
                        aux[strlen(aux) - 1] = '\0';
                        esEQU = buscaRotulo(strupr(aux),cantRotulos,rotulos);
                        if(esEQU == -1)
                            sscanf(&instruccion[i][3],"%d",&offset);
                        else{
                            if(esEQU==12345678)
                                offset=-1;
                            else
                                offset = esEQU;

                            if(instruccion[i][3] == '-')
                                offset*=-1;
                        }

                        //int offset = atoi(&instruccion[i][4]);
                        offset = offset << 8;
                    }


                    arg = nroregistro | offset;


                }
                else
                {
                    if(esEQU1 == -1)
                        arg=atoi(&instruccion[i][1]);
                    else
                        arg = esEQU1;
                }


                switch(i)
                {
                case 0x0:
                {
                    *error = 1;
                    errorLinea = 1;
                    i = 10;
                }
                break;
                case 0x1:
                {
                    arg1 = arg;
                    top1 = esindirecto?0b11:0b10;
                }
                break;
                case 0x2:
                {
                    arg2 = arg;
                    top2 = esindirecto?0b11:0b10;
                }
                break;
                }
            }
            else //ROTULO
            {
                if( cantRotulos > 0)
                {
                    int referencia = buscaRotulo(strupr(instruccion[i]),cantRotulos,rotulos);
                    if (referencia!=-1)
                    {
                        switch(i)
                        {
                        case 0:
                        {
                            *error = 1;
                            errorLinea = 1;
                            i = 10;
                        }
                        case 1:
                        {
                            arg1=referencia;
                            top1=0b00;
                            break;
                        }
                        case 2:
                        {
                            arg2=referencia;
                            top2=0b00;
                            break;
                        }
                        }
                    }
                }

                else
                {
                    *error = 1;
                    errorLinea = 1;
                    i = 10;
                }

            }
        }
        i++;
    }
    if(!errorLinea)
    {
        // PASAR A HEXA
        printf("[0000 %04x]: ",DS);
        printf("%04x ",op);
        op = op << 16;
        printf("00%d%d ",top1,top2);
        top1 = top1 << 4;
        ins = op | top1 | top2;
        int arg11 = (arg1 & 0xFFFF0000) >> 16;
        int arg12 = arg1 & 0x0000FFFF;
        int arg21 = (arg2 & 0xFFFF0000) >> 16;
        int arg22 = arg2 & 0x0000FFFF;
        printf("%04x ",arg11);
        printf("%04x ",arg12);
        printf("%04x ",arg21);
        printf("%04x ",arg22);
        printf("%d:", (int)DS/3 +1);
        switch(cantPalabras)
        {
        case 3:
            printf("%s %s,%s",instruccion[0],instruccion[1],instruccion[2]);
            break;
        case 2:
            printf("%s %s",instruccion[0],instruccion[1]);
            break;
        case 1:
            printf("%s ",instruccion[0]);
            break;

        }




        RAM[DS] = ins;
        RAM[DS+1] = arg1;
        RAM[DS+2] = arg2;
        DS+=3;
    }
    else
    {
        printf("FFFF FFFF FFFF FFFF");
        RAM[DS] = 0xFFFFFFFF;
        RAM[DS+1] = 0xFFFFFFFF;
        RAM[DS+2] = 0xFFFFFFFF;
        DS+=3;
    }
}

void salteaespacios(char* linea, int* pos)
{
    while(linea[*pos]==' ' || linea[*pos]=='\t') //aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
        (*pos)++;
}

void traductora(FILE *Fasm,int cantRotulos, rotulo* rotulos, int *error)
{
    purificadora(Fasm);
    char linea[990] = " ";
    char instruccion[5][50] = {" "," "," "," "," "};
    fseek(Fasm, 0, SEEK_SET);
    int pos;
    int cantPalabras=0;
    while(!feof(Fasm))//while de linea en linea
    {
        strcpy(instruccion[0],"");
        strcpy(instruccion[1],"");
        strcpy(instruccion[2],"");
        pos = 0;
        fgets(linea,990,Fasm);
        cantPalabras=0;
        salteaespacios(linea,&pos);
        if(linea[pos]!='\n' && linea[pos]!='\0')
        {
            if (linea[pos]!='*' && linea[pos]!=';' && linea[pos]!='\\')
            {
                //if anti comentarios
                pos = 0;
                salteaespacios(linea,&pos);
                while(linea[pos] != '\n' && linea[pos]!='\0' && linea[pos]!=';') //while para la linea
                {
                    salteaespacios(linea,&pos);
                    if(linea[pos]=='\'')
                    {
                        instruccion[cantPalabras][0]=linea[pos++];
                        instruccion[cantPalabras][1]=linea[pos++];
                        instruccion[cantPalabras][2]='\0';
                        cantPalabras++;
                        if(linea[pos]=='\'')
                            pos++;
                    }
                    else
                    {
                        //ahora leo la palabrita
                        int minipos =0;
                        while(linea[pos]!='\n' && linea[pos]!='\0' && linea[pos]!=':' && linea[pos]!=';' && linea[pos]!=' ' && linea[pos]!='\t')
                        {
                            //while por palabra
                            instruccion[cantPalabras][minipos]=linea[pos];
                            minipos++;//una mas en la palabra
                            pos++;// una mas en la linea
                        }
                        instruccion[cantPalabras][minipos]='\0';
                        if(linea[pos]!=':')
                        {
                            if(strcmp(instruccion[cantPalabras],""))
                                cantPalabras++;
                        }
                        else
                            pos++;
                    }
                }//fin de linea
                if(strcmp(strupr(instruccion[1]),"EQU")!=0)
                    escribeRAM(instruccion,cantPalabras, cantRotulos, rotulos,error);
                if(linea[pos]==';') //comentario corto
                {
                    linea[strlen(linea)-1]='\0'; //jojo
                    printf("%s",&linea[pos]);
                }
                printf("\n");
            }//fin if anti comentarios
            else  //comentario largo
            {
                linea[strlen(linea)-1]='\0'; //jojo
                printf("%s\n",linea);
            }
        }
    }//fin linea en linea
}
