.text
  li t1 0xffffffff
  li t2 0x2000
  li t3 0x6000
loop:
  bge t2 t3 reset
  sw t1 (t2)
  addi t2 t2 4
#  add a0 zero t2
#  li a7 1
#  ecall
switch_color:
# not t1 t1
  j loop

reset:
  not t1 t1
  li t2 0x2000
  j loop
