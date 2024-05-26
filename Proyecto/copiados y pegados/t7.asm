* STRING TEST
* Lee una frase por teclado
		mov 	ax, %1010
		mov 	dx, 1
		mov 	cx, -1
		sys 	1
* calcula longitud y la guarda en 0	
		slen 	fx, [1]
		mov 	[0], fx
* copia a 101 y pone en mayusculas
		smov 	[101],[1]
		mov 	bx, 101
otro:	cmp		[bx],0
		jz 		sigue
		sub 		[bx],' '
            add         bx,1
		jmp		otro
* imprime la frase en mayusculas		
sigue:	mov		ax, %0110
		mov		dx, 101
		mov 	cx, -1
		sys 	3
		stop
