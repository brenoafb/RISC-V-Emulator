.data

glyph: .byte
0,0,0xff,0xff,0,0,
0,0xff,0,0,0xff,0,
0,0xff,0,0,0xff,0,
0,0xff,0,0,0xff,0,
0,0xff,0,0,0xff,0,
0,0xff,0,0,0xff,0,

glyph_width: .word 6
glyph_height: .word 6
width: .word 128
height: .word 128


.text
loop:
add x0 x0 x0
j loop

# .text
#   li t1 0x3f
#   li t2 0x2000
#   li t3 0x6000
#   li t4 0xff
# loop:
#   bge t2 t3 reset
#   sb t1 (t2)
#   addi t2 t2 1
#   j loop
#   j loop
# reset:
# #  xor t1 t1 t4
#   not t1 t1
#   li t2 0x2000
#   j loop
