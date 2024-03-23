.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
#   a0 (int*) is the pointer to the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   None
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# ==============================================================================
relu:
    li t0 1
    blt a1 t0 Exception
    li t0 0
    li t1 4
    
loop_start:
    beq t0 a1 loop_end
    mul t2 t0 t1
    add t2 t2 a0
    lw t3 0(t2)
    bgt t3 x0 loop_continue
    li t3 0
    sw t3 0(t2)

loop_continue:
    addi t0 t0 1
    j loop_start

loop_end:
	ret

Exception: 
    li a0 36
    j exit




