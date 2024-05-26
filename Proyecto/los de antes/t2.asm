* PRUEBA INSTRUCCIONES DE SALTOS Y 'READ'
	mov 	[1]	,0
	mov 	[2]	,0
 mov aX     , %1101
mov Dx,      0
mov cx,1
leo: SyS 1
add [1],[0]
add [2],1
cmp [0],0
jnz leo
mov ax,[1]
mov [3],0
otro: 	add [3],1
		je  [3],16
		jmp otro
mov ax,[2]
mov [4],0
uno: add [4],1
sub ax,1
jl  0,uno
mov dx,1
mov cx,4
mov ax,%1101
sys 2
stop
