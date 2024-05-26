* NUMEROS NEGATIVOS
mov	ax, %110D
mov	dx, 1
mov	cx, 1
mov   [1],#32767
sys 2
add   [1],#1
sys 2
add   [1],#1
sys 2
sub   [1],#2
sys 2
mov   [1],%FFFF
sys 2
add   [1],#1
sys 2
add   [1],#1
sys 2
add   [1],#1
sys 2
