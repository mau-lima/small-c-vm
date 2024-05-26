\\ASM STACK=1000
* CALCULO DEL FACTORIAL DE UN NUMERO
      MOV   AX, %1101
      MOV   DX, %000A
      MOV   CX, %0001
      SYS   1
      MOV   AX,1
      PUSH  [10]
      CALL  FACT
      ADD   SP,1
      MOV   [10],AX
      MOV   AX, %1101
      MOV   DX, %000A
      MOV   CX, %0001
      SYS   2
      STOP
* SUBRUTINA RECURSIVA PARA EL CALCULO DEL FACTORIAL *
FACT: PUSH  BP
      MOV   BP,SP
      CMP   [BP+2],1
      JZ    FIN
      JN    FIN
      MUL   AX,[BP+2]
      SUB   [BP+2],1
      PUSH  [BP+2]
      CALL  FACT
      ADD   SP,1
FIN:  MOV   SP,BP
      POP   BP
      RET
