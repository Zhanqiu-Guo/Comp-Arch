.globl argmax

.text
# =================================================================
# FUNCTION: Given a int array, return the index of the largest
#   element. If there are multiple, return the one
#   with the smallest index.
# Arguments:
#   a0 (int*) is the pointer to the start of the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   a0 (int)  is the first index of the largest element
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# =================================================================
argmax:
	# Prologue
	li t0 1
	blt a1 t0 Exception
	li t0 0
	li t1 4
	lw t2 0(a0)
	li t3 0

loop_start:
	beq t0 a1 loop_end
    mul t4 t0 t1
    add t4 t4 a0
    lw t5 0(t4)
    blt t5 t2 loop_continue
    mv t2 t5
	mv t3 t0

loop_continue:
	addi t0 t0 1
    j loop_start

loop_end:
	# Epilogue
	mv a0 t3
	ret

Exception:
	li a0 36
	j exit