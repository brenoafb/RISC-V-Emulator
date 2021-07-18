.data

hello: .asciz "Hello from the program!\n"
goodbye: .asciz "Done writing glyph\n"

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

la a0 hello
li a7 4       # print string
ecall

li a0 0x4000
li a1 0

addi sp sp 4
lw a0 -4(sp)
call draw_glyph
lw a0 -4(sp)
addi sp sp -4

j loop

lw t0 glyph_width # go to next char space
add a0 a0 t0

li a1 0
addi sp sp 4
lw a0 -4(sp)
call draw_glyph
lw a0 -4(sp)
addi sp sp -4

la a0 goodbye
li a7 4       # print string
ecall

j loop

loop:
j loop

# draw a glyph in the position starting at a0
draw_glyph:
li a1 0
lw a2 glyph_height
draw_glyph_loop:
beqz a2 draw_glyph_end

addi sp sp 4
sw ra -4(sp)
call draw_glyph_line
lw ra -4(sp)
addi sp sp 4


lw t0 width 			# go to next line
add a0 a0 t0
addi a1 a1 1                    # next glyph line
addi a2 a2 -1                   # decrement counter
j draw_glyph_loop
draw_glyph_end:
ret

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
lw t1 width
lw t2 glyph_height
mul t1 t1 t2
add a0 a0 t1
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
