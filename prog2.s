	# Aleksander Sankar
	# This program is translated into MIPS code from C. It follows
	# the procedure of prog2.c. First, it reads in one value from
	# the console. Then it calls the function  count_digits and
	# prints the result of it. count_digits uses a local variable
	# and negates the argument if it is negative. It then calculates
	# how many times the argument can be divided by 10 until the
	# integer result is zero. It stores this number in num_digits
	# and then returns it.
	
	.text

main:	li      $sp, 0x7ffffffc #set up stack pointer

	li	$v0, 5		#system call to read in int	 
	syscall			#equivalent of scanf
	move	$t0, $v0	#read value for n

	sw      $t0, ($sp)	#store argument for the function
   	sub     $sp, $sp, 4

	jal	F		#call the function F

	add     $sp, $sp, 4     #pop argument from stack

	sw      $v0, value      #store return value, which is in
							#$v0, into value

	
	lw      $a0, value      # printf("%d", value)
	li      $v0, 1
	syscall

	li      $v0  11          # printf("%c", '\n')
	li      $a0, 10
	syscall
	
	li      $v0, 10          # quit program
	syscall

F:	sub   	$sp, $sp, 16     #set new stack pointer
	sw      $ra, 16($sp)     #save return address in stack
	sw      $fp, 12($sp)     #save old frame pointer in stack
	add     $fp, $sp, 16     #set new frame pointer
	
	lw      $t0, 4($fp)      #argument in caller's frame

	li	$t1, 0		#num_digits is $t1
				#it is 0 initially
	sw	$t1, 8($sp)	#local variable

	li	$t2, 10		#for use in division
	sw	$t2, 4($sp)


	bge	$t0, 0, L1	
	neg	$t0, $t0	#negate number if negative

L1:	div	$t0, $t2	#do while begins		
	mflo	$t0		#store division result
	addi	$t1, $t1, 1	#increment num_digits
	bgt	$t0, 0, L1	#while loop jump

	move	$v0, $t1	#store num_digits and return

	lw      $ra, 16($sp)    #load return address from stack
	lw      $fp, 12($sp)    #restore old frame pointer from stack
	add     $sp, $sp, 16    #reset stack pointer
	jr	$ra		#return

	.data

value:	 .word 0
