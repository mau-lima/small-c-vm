MIN EQU %80000000
MAX EQU %7FFFFFFF
UNO EQU 1
\\ASM stack=1000
     MOV    [1],0
     MOV    [2],MIN
     MOV    [3],MAX
     mov bx,1
     sys %3D
     jn bye     
     jz fin
INI: mov ax, %0101
     mov bx,1
     mov dx,4
     mov cx,1
     sys %3F
     jn bye 
     jz fin    
     MOV    AX,[4]
     ADD    [1],UNO
     JG     [2],SIG
     MOV    [2],AX
SIG: JL     [3],INI
     MOV    [3],AX
     JMP    INI
FIN: sys %E3 ;cierra el archivo
* muestra la cantidad leida el minimo y m�ximo
     mov ax, %110C
     mov dx, 1
     mov cx, 3
     sys 2
bye: STOP
