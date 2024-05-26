#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"
#include "instrucciones.h"
#define DS REG[2]
#define IP REG[8]
#define CC REG[9]
#define AX REG[10]
#define BX REG[11]
#define CX REG[12]
#define DX REG[13]
#define EX REG[14]
#define FX REG[15]
//segunda parte
#define SS REG[4]
#define SP REG[6]
#define BP REG[7]


//#define argc 4

typedef int (*f)(int *op1, int *op2);
int REG[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int RAM[cantCeldas];

archivo archivos[10];
int pilaDefault =200;


//TA OK t9 ok

int main(int argc,char** argv)
//int main()
{
    // USAR EL DEFINE const int argc=2;
/*
        char argv[argc][500];
        strcpy(argv[0],"mv.exe");
        strcpy(argv[1],"tA.asm");
        strcpy(argv[2],"vaasdascio.txt");
        strcpy(argv[3],"vacio.txt"); // REVISAR Q ESTE EXISTA!!!! AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
*/
    //strcpy(argv[2],"porotin.img");
    //FIN variables crotas p poder ejecutarlo desde el debugger*/
    FILE *Fasm=NULL;
    FILE *Fimg=NULL;
    int cantRotulos=0,error=0;
    rotulo* rotulos = NULL;

    int arg = abrirArchivos(argc,argv,&Fasm,&Fimg);
    /* arg 1= solo ejecutar
        arg 2 = solo traducir
        arg 3 = todo*/

    if (arg!=-1)
    {
        if (arg & 0b10)//este es el if traductor (arg == 2 || arg == 3)
        {
            /*inicio del interprete*/
            rotuladora(Fasm,&cantRotulos,&rotulos,&error); //gabriel
            traductora(Fasm,cantRotulos,rotulos,&error);
            printf("\n");
            fclose(Fasm);
            /*fin del interprete*/
            //aca ya esta todo traducido y catgado en los vectores
            if (!(arg & 0b1) && !error)//lo meto en le archivo si no hay que ejecutar
            {
                fwrite(RAM, 4, cantCeldas, Fimg); //es mas lindo
                fwrite(REG, 4, 16, Fimg); // 16*4
                fclose(Fimg);
            }
        }

        if(arg & 0b01 && !error) //este es el if ejecutor (arg == 1 || arg == 3)
        {
            if((arg & 0b10) == 0) //archivo pasado a bin si no hubo q interpretar
            {
                fread(RAM, 4, cantCeldas, Fimg);
                fread(REG, 4, 16, Fimg);
                fclose(Fimg);
            }
            //parte q ejecuta
            SS=cantCeldas-pilaDefault;
            SP=pilaDefault;
            ejecucion();
        }
        if(error)
            printf("ERROR DE TRADUCCION\n");
    }
    else
        printf("La ejecucion no puede continuar por que hubo errores al abrir los archivos");


    printf("\n\n");
    printf("*****************************************************\n");
    printf("*                                                   *\n");
    printf("*  Ejecucion terminada, presione enter para cerrar  *\n");
    printf("*                                                   *\n");
    printf("*****************************************************\n");


    getchar();
    return 0;
}




void iniciaf(f func[])
{
    func[0x01] = MOV;
    func[0x13] = CMP;
    func[0x24] = JZ;
    func[0x32] = OR;
    func[0x02] = ADD;
    func[0x17] = SWAP;
    func[0x25] = JP;
    func[0x33] = NOT;
    func[0x03] = SUB;
    func[0x19] = RND;
    func[0x26] = JN;
    func[0x34] = XOR;
    func[0x04] = MUL;
    func[0x20] = JMP;
    func[0x27] = JNZ;
    func[0x37] = SHL;
    func[0x05] = DIV;
    func[0x21] = JE;
    func[0x28] = JNP;
    func[0x38] = SHR;
    func[0x06] = MOD;
    func[0x22] = JG;
    func[0x29] = JNN;
    func[0x81] = SYS;
    func[0x23] = JL;
    func[0x31] = AND;
    func[0x8F] = STOP;

    //segunda parte
    func[0x44] = PUSH;
    func[0x45]=POP;
    func[0x46]=PUSHALL;
    func[0x47]=POPALL;
    func[0x40]=CALL;
    func[0x48]=RET;
    func[0x50]=SLEN;
    func[0x51]=SMOV;
    func[0x53]=SCMP;
}

void OP(int top1,int top2,int **op1,int **op2)
{
    if(top1 == 0b00)
        *op1 = &RAM[IP+1];
    else if(top1 == 0b01)
        *op1 = &REG[RAM[IP+1]];
    else if(top1 == 0b10)
        *op1 = &RAM[DS + RAM[IP+1]];
    else
    {
        int offset= (RAM[IP+1] & 0xFFFFFF00);
        offset = offset >> 8;
        int regnum = RAM[IP+1] & 0xFF;
        if(regnum != 7 && regnum!=6)
            *op1 = &RAM[DS + REG[regnum]+offset];
        else
            *op1 = &RAM[SS + REG[regnum]+offset];
    }


    if(top2 == 0b00)
        *op2 = &RAM[IP+2];
    else if(top2 == 0b01)
        *op2 = &REG[RAM[IP+2]];
    else if(top2 == 0b10)
        *op2 = &RAM[DS + RAM[IP+2]];
    else
    {
        int offset= (RAM[IP+2] & 0xFFFFFF00);
        offset =  offset >> 8;
        int regnum = RAM[IP+2] & 0xFF;
        if(regnum != 7 && regnum!=6)
            *op2 = &RAM[DS + REG[regnum]+offset];
        else
            *op2 = &RAM[SS + REG[regnum]+offset];
    }
}
void ejecucion()
{
    f func[0x90];
    iniciaf(func);
    int ins,top1,top2,*op1,*op2,error = 0;
    IP = 0;
    while(IP>= 0 && IP < DS && error == 0)
    {
        ins = (RAM[IP] & 0xFFFF0000) >> 16;
        top1 = (RAM[IP] & 0b110000) >> 4;
        top2 = (RAM[IP] & 11);
        OP(top1,top2,&op1,&op2);
        IP+=3;
        error = func[ins](op1,op2);
    }
    //printf("el print q usamos para debugear");
}
