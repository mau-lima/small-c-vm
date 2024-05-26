* DIRECCIONAMIENTO INDIRECTO
\\ASM STACK=100
ant		EQU 	-1
sig		EQU		1
* inicio los registros
		mov 	ax, 2
		mov		bx, 5
		pushall
* relleno la memoria de 1 a 10
		mov 	ax, 1
otro:	cmp		ax, 7
		jz		sigue
		mov		[ax], ax
		add		ax, 1
		jmp 	otro
* recupero registros y multiplico x10
sigue:	popall
		mul 	[ax+ant],10
		mul 	[ax],10
		mul 	[ax+sig],10
		mul 	[bx-1],10
		mul 	[bx],10
		mul 	[bx+1],10
		mov 	[bx+2],[bx-2] ;[7] <- 30
* imprime todo 
		mov 	ax, %110D
		mov		dx, 1
		mov		cx, 7
		sys 2
		stop
* resultados:
* [0000 0001]: 10
* [0000 0002]: 20
* [0000 0003]: 30
* [0000 0004]: 40
* [0000 0005]: 50
* [0000 0006]: 60
* [0000 0007]: 30