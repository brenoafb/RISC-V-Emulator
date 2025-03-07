.data

glyph: .byte
0   ,0   ,0xff,0xff,0   ,0,
0   ,0xff,0   ,0   ,0xff,0,
0   ,0xff,0   ,0   ,0xff,0,
0   ,0xff,0xff,0xff,0xff,0,
0   ,0xff,0   ,0   ,0xff,0
0   ,0xff,0   ,0   ,0xff,0,


glyph_width: .word 6
glyph_height: .word 6
width: .word 128
height: .word 128


.text
li a0 0x3000
li a1 0

call draw_glyph_line
call next_line
addi a1 zero 1

call draw_glyph_line
call next_line
addi a1 zero 1

call draw_glyph_line
call next_line
addi a1 zero 1

call draw_glyph_line
call next_line
addi a1 zero 1

call draw_glyph_line
call next_line
addi a1 zero 1

loop:
j loop

# draw a glyph in the position starting at a0
# draw_glyph:

# draw the a1'th line at the position pointed to
# by a0
draw_glyph_line:
addi t0 a0 0
la t1 glyph

lw t2 glyph_width
mul t2 t2 a1              # t2: starting position of glyph line
add t1 t1 t2              # t1: starting position of line
lw t2 glyph_width         # t2: glyph width
draw_glyph_line_loop:
beqz t2 draw_glyph_line_end
lb t3 0(t1)
sb t3 4(t0)
addi t0 t0 1
addi t1 t1 1
addi t2 t2 -1
j draw_glyph_line_loop
draw_glyph_line_end:
ret

# puts in a0 the address of the same
# position in the next line
next_line:
# lw t1 width
# lw t2 glyph_height
# mul t1 t1 t2
# add a0 a0 t1
lw t0 width
add a0 a0 t0
ret

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
