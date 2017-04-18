	#  Aleksander Sankar
	# This program is translated into MIPS code from C. It follows
	# the procedure of prog1.c. First, it reads in three values from the
	# console, m, n, and mod_value. It then performs
	# a while loops and computes m mod mod_value and checks if it is zero
	# (mod_value evenly divides it). If this condtion is true, a counter
	# variable is incremented. The check is then performed on the next
	# integer, until it that integer is greater than n. It finally prints
	# count.
	
	.text

main:	li 	$t4, 0		#count is in $t4
	li	$t3, 0		#temporary value to store mod result
	
	li	$v0, 5		#system call to read in int	 
	syscall			#equivalent of scanf
	move	$t0, $v0	#read value for m


	li	$v0, 5
	syscall
	move	$t1, $v0	#read value for n


	li	$v0, 5
	syscall
	move	$t2, $v0	#read value for mod_value

	
WL:	bgt	$t0, $t1, DONE	#begin while loop
				#while m <= n

	rem	$t3, $t0, $t2	#load mod result to $t3

	bne	$t3, 0, IF	#if m % mod_value = 0, increment counter
	addi	$t4, $t4, 1	#increment counter

	
IF:	addi	$t0, $t0, 1
	j	WL		#end of the while loop
	
DONE:	li	$v0, 1		#print out the count
	move	$a0, $t4
	syscall

	li      $v0  11          # printf("%c", '\n')
	li      $a0, 10
	syscall

	
	li      $v0, 10          # quit program
	syscall


	
