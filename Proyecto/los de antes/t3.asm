* MULTIPLICACION DE DOS NUMEROS - ALGORITMO 'SHIFT AND ADD'
MOV  AX,%1101
MOV DX, 1
MOV 	CX,2
SYS 1
SYS 2
XOR     CX,CX
MOV     BX, [2]
MOV     AX, BX
AND     AX, %0001
JZ      12
ADD     CX, [1]
shl  [1],1
shr  BX,1
MOV     AX,BX
JZ     %11
JMP     9
MOV [3],CX
MOV AX, %1101
MOV DX, 3
MOV CX,1
SYS 2