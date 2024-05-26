* PRUEBA ASCII
  mov AX, %0110
  mov DX, 0 
  mov CX, 4
  sys 1 
  mov ex,'z' 
  sub ex, [0]
  add ex,'a' 
  mov [0], ex
  mov ex,'z' 
  sub ex, [1]
  add ex,'a' 
  mov [1], ex
  mov ex,'z' 
  sub ex, [2]
  add ex,'a' 
  mov [2], ex
  mov ex,'z' 
  sub ex, [3]
  add ex,'a' 
  mov [3], ex
  mov AX, %110F
  sys 2
  stop