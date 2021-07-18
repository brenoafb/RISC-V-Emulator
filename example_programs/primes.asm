.data
primes:	.word 1, 3, 5, 7, 11, 13, 17, 19
size:	.word 8
msg:	.asciz "The first prime numbers are: "
space:	.ascii " "

.text
	la t0, primes
	la t1, size
	lw t1, 0(t1)
	li a7, 4
	la a0, msg
	ecall

loop:	beq t1, zero, exit
	li a7, 1
	lw a0, 0(t0)
	ecall
	li a7, 4
	la a0, space
	ecall
	addi t0, t0, 4
	addi t1, t1, -1
	j loop
exit:	li a7, 10
	ecall
