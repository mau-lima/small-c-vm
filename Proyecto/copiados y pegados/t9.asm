* SUBRUTINAS (POTENCIA DE DOS NUMEROS POSITIVOS)
\\ASM STACK=300
	MOV     AX, %1101
	MOV     DX, 1
        MOV     CX, 1
        SYS  	1
	MOV     AX, %1101
	MOV     DX, 2
        MOV     CX, 1
        SYS  	1
	MOV	AX,%A
	MOV	CX,%C
	MOV	DX,%D
	PUSH	[2]
	PUSH	[1]
	CALL 	POTEN
	POP	[3]
	ADD	SP, 1
        MOV     [4],AX
        MOV     [5],CX
        MOV     [6],DX
	MOV     AX, %1101
        MOV     DX, 1
        MOV     CX, 6
	SYS     3
	POP   AX ; <<< debe dar stack empty: quitar
** verificar al final que ax, bx y cx queden con los valores setados al principio.	
	STOP
* SUBRUTINA POTEN
POTEN: 	PUSH	BP
	MOV	BP, SP
	PUSH    AX
	PUSH    CX
	PUSH	DX
	MOV  	AX,[BP+2]
	MOV     DX,[BP+3]
L1:	MOV     CX,[BP+2]
    	SUB     DX,#1
       	CMP     DX,#0
       	JZ     	FIN1
       	CALL   	PROD
       	JMP     L1
FIN1:  	MOV     [BP+2],AX
	POP	DX
	POP	CX
	POP	AX
	MOV	SP, BP
	POP	BP
       	RET
* SUBRUTINA PROD
PROD:  PUSH     AX
       POP      BX
L2:    SUB      CX,#1
       CMP      CX,#0
       JZ     	FIN2
       ADD      AX,BX
       JMP      L2
FIN2:  RET
