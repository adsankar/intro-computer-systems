	# Aleksander Sankar
	# I pledge on my honor that I have not given or received any
	# unauthorized assistance on this assignment.
	#
	# This program is translated into MIPS code from C. It follows
	# the procedure of prog3.c. First, it reads in values from
	# the console and adds them to the array until a value read is
	# zero. Then it reads in element from the console and increments
	# a counter until another zero is seen. It searches for each of
	# these and if the number is found, print it.
	# If not found print -1.

	
	.data
arr:	.space	400		#space for the array is 400
				#because each int takes 4 bytes
arrsz:	.word	100		#array size is 100
	
	.text

main:	lw	$s0, arrsz	#store size of array in $s0
	la	$s1, arr	#address of array is in $s1
	li 	$t1, 0		#set max to 0
	li	$t2, 0		#set element to 0
	li 	$t4, 0		#set pos to 0
	li 	$t5, 0		#a temporary variable

	li	$v0, 5		#system call to read in int	 
	syscall			#equivalent of scanf
	move	$t0, $v0	#read value for n



WL1:	beq	$t0, 0, END1	#begin while loop

	bge	$t1, 100, IF1	#if max < 100

	sw	$t0, arr($t4)	#arr[max] = n

	addi	$t1, $t1, 1
	mul	$t4, $t1, 4	#increment and move pointer
	
IF1:	li	$v0, 5		#system call to read in int	 
	syscall			#equivalent of scanf
	move	$t0, $v0	#read value for n (again)

	j	WL1		#jump to top of loop

END1:	li	$v0, 5		#system call to read in int	 
	syscall			#equivalent of scanf
	move	$t2, $v0	#read value for element

WL2:	beq	$t2, 0, END2	#while element != 0

	li	$t3, 0		#set idx to 0
	li	$t4, 0		#set pointer to 0


WL3:	lw	$t5, arr($t4)	#$t5 is the element at that index
	bge	$t3, $t1, END3	#while idx < max
	beq	$t5, $t2, END3	#while arr[idx] != element
	addi	$t3, $t3, 1	#increment idx
	mul	$t4, $t3, 4	#move pointer

	j	WL3

END3:	bge	$t3, $t1, IF2	#if idx < max
	li	$v0, 1		#print out idx
	move	$a0, $t3
	syscall

	j	NEXT
	

IF2:	li	$v0, 1		#else print out -1
	li	$t6, -1
	move	$a0, $t6
	syscall
	
NEXT:	li      $v0  11          # printf("%c", '\n')
	li      $a0, 10
	syscall

	li	$v0, 5		#system call to read in int	 
	syscall			#equivalent of scanf
	move	$t2, $v0	#read value for element (again)

	j	WL2		#back to top of while loop

	
END2:	li      $v0, 10          # quit program
	syscall


