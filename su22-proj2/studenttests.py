from unittest import TestCase
from framework import AssemblyTest, print_coverage

"""
Coverage tests for project 2 is meant to make sure you understand
how to test RISC-V code based on function descriptions.
Before you attempt to write these tests, it might be helpful to read
unittests.py and framework.py.
Like project 1, you can see your coverage score by submitting to gradescope.
The coverage will be determined by how many lines of code your tests run,
so remember to test for the exceptions!
"""

"""
abs_loss
# =======================================================
# FUNCTION: Get the absolute difference of 2 int arrays,
#   store in the result array and compute the sum
# Arguments:
#   a0 (int*) is the pointer to the start of arr0
#   a1 (int*) is the pointer to the start of arr1
#   a2 (int)  is the length of the arrays
#   a3 (int*) is the pointer to the start of the result array

# Returns:
#   a0 (int)  is the sum of the absolute loss
# Exceptions:
# - If the length of the array is less than 1,
#   this function terminates the program with error code 36.
# =======================================================
"""

simple_array0 = [1,2,3,4,5,6,7,8,9]
simple_array1 = [1,3,5,3,5,7,5,7,9]

class TestAbsLoss(TestCase):
    def test_simple(self):
        # load the test for abs_loss.s
        t = AssemblyTest(self, "../coverage-src/abs_loss.s")

        # create array0 in the data section
        array0 = t.array(simple_array0)
        # load address of `array0` into register a0
        t.input_array("a0", array0)
        # create array1 in the data section
        array1 = t.array(simple_array1)
        # load address of `array1` into register a1
        t.input_array("a1", array1)
        # set a2 to the length of the array
        t.input_scalar("a2",9)
        # create a result array in the data section (fill values with -1)
        output_array = t.array([-1,-1,-1,-1,-1,-1,-1,-1,-1])
        # load address of `array2` into register a3
        t.input_array("a3",output_array)
        # call the `abs_loss` function
        t.call("abs_loss")
        # check that the result array contains the correct output
        result = [0,1,2,1,0,1,2,1,0]
        t.check_array(output_array, result)
        # check that the register a0 contains the correct output
        t.check_scalar("a0", 8)
        # generate the `assembly/TestAbsLoss_test_simple.s` file and run it through venus
        t.execute()

    # Add other test cases if neccesary
    def test_exception(self):
        t = AssemblyTest(self, "../coverage-src/abs_loss.s")
        t.input_array("a0", t.array([]))
        t.input_array("a1", t.array([]))
        t.input_scalar("a2",0)
        output_array = t.array([])
        t.input_array("a3",output_array)
        t.call("abs_loss")
        t.check_scalar("a0", 36)
        # generate the `assembly/TestAbsLoss_test_simple.s` file and run it through venus
        t.execute()

    @classmethod
    def tearDownClass(cls):
        print_coverage("abs_loss.s", verbose=False)


"""
squared_loss
# =======================================================
# FUNCTION: Get the squared difference of 2 int arrays,
#   store in the result array and compute the sum
# Arguments:
#   a0 (int*) is the pointer to the start of arr0
#   a1 (int*) is the pointer to the start of arr1
#   a2 (int)  is the length of the arrays
#   a3 (int*) is the pointer to the start of the result array

# Returns:
#   a0 (int)  is the sum of the squared loss
# Exceptions:
# - If the length of the array is less than 1,
#   this function terminates the program with error code 36.
# =======================================================
"""


class TestSquaredLoss(TestCase):
    def test_simple(self):
        # load the test for squared_loss.s
        t = AssemblyTest(self, "../coverage-src/squared_loss.s")

        # create input arrays in the data section
        array0 = t.array(simple_array0)
        array1 = t.array(simple_array1)
        # load array addresses into argument registers
        t.input_array("a0",array0)
        t.input_array("a1",array1)
        # load array length into argument register
        t.input_scalar("a2",9)
        # create a result array in the data section (fill values with -1)
        output_array = t.array([-1,-1,-1,-1,-1,-1,-1,-1,-1])
        # load result array address into argument register
        t.input_array("a3",output_array)
        # call the `squared_loss` function
        t.call("squared_loss")
        # check that the result array contains the correct output
        result = [0,1,4,1,0,1,4,1,0]
        t.check_array(output_array, result)
        # check that the register a0 contains the correct output
        t.check_scalar("a0", 12)
        # generate the `assembly/TestSquaredLoss_test_simple.s` file and run it through venus
        t.execute()

    # Add other test cases if neccesary

    def test_exception(self):
        t = AssemblyTest(self, "../coverage-src/squared_loss.s")
        t.input_array("a0",t.array([]))
        t.input_array("a1",t.array([]))
        t.input_scalar("a2",0)
        output_array = t.array([])
        t.input_array("a3",output_array)
        t.call("squared_loss")
        t.check_scalar("a0", 36)
        t.execute()


    @classmethod
    def tearDownClass(cls):
        print_coverage("squared_loss.s", verbose=False)


"""
zero_one_loss
# =======================================================
# FUNCTION: Generates a 0-1 classifer array inplace in the result array,
#  where result[i] = (arr0[i] == arr1[i])
# Arguments:
#   a0 (int*) is the pointer to the start of arr0
#   a1 (int*) is the pointer to the start of arr1
#   a2 (int)  is the length of the arrays
#   a3 (int*) is the pointer to the start of the result array

# Returns:
#   NONE
# Exceptions:
# - If the length of the array is less than 1,
#   this function terminates the program with error code 36.
# =======================================================
"""


class TestZeroOneLoss(TestCase):
    def test_simple(self):
        # load the test for zero_one_loss.s
        t = AssemblyTest(self, "../coverage-src/zero_one_loss.s")

        # create input arrays in the data section
        # TODO
        array0 = t.array(simple_array0)
        array1 = t.array(simple_array1)
        # load array addresses into argument registers
        # TODO
        t.input_array("a0",array0)
        t.input_array("a1",array1)
        # load array length into argument register
        # TODO
        t.input_scalar("a2",9)
        # create a result array in the data section (fill values with -1)
        # TODO
        output_array = t.array([-1,-1,-1,-1,-1,-1,-1,-1,-1])
        # load result array address into argument register
        # TODO
        t.input_array("a3",output_array)
        # call the `zero_one_loss` function
        # TODO
        t.call("zero_one_loss")
        # check that the result array contains the correct output
        # TODO
        result = [1,0,0,0,1,0,0,0,1]
        t.check_array(output_array, result)
        # generate the `assembly/TestZeroOneLoss_test_simple.s` file and run it through venus
        # TODO
        t.execute()

    # Add other test cases if neccesary

    def test_exception(self):
        # load the test for zero_one_loss.s
        t = AssemblyTest(self, "../coverage-src/zero_one_loss.s")

        t.input_array("a0",t.array([]))
        t.input_array("a1",t.array([]))
        t.input_scalar("a2",0)
        output_array = t.array([])
        t.input_array("a3",output_array)
        t.call("zero_one_loss")
        t.check_scalar("a0",36)
        t.execute()

    @classmethod
    def tearDownClass(cls):
        print_coverage("zero_one_loss.s", verbose=False)


"""
initialize_zero
# =======================================================
# FUNCTION: Initialize a zero array with the given length
# Arguments:
#   a0 (int) size of the array

# Returns:
#   a0 (int*)  is the pointer to the zero array
# Exceptions:
# - If the length of the array is less than 1,
#   this function terminates the program with error code 36.
# - If malloc fails, this function terminates the program with exit code 26.
# =======================================================
"""


class TestInitializeZero(TestCase):
    def test_simple(self):
        t = AssemblyTest(self, "../coverage-src/initialize_zero.s")
        # input the length of the desired array
        t.input_scalar("a0",3)
        # call the `initialize_zero` function
        t.call("initialize_zero")
        # check that the register a0 contains the correct array (hint: look at the check_array_pointer function in framework.py)
        result=[0,0,0]
        t.check_array_pointer("a0",result)
        t.execute()

    def test_exception1(self):
        t = AssemblyTest(self, "../coverage-src/initialize_zero.s")
        t.input_scalar("a0",0)
        # call the `initialize_zero` function
        t.call("initialize_zero")
        t.check_scalar("a0",36)
        t.execute()

    def test_exception2(self):
        t = AssemblyTest(self, "../coverage-src/initialize_zero.s")
        t.input_scalar("a0",1)
        # call the `initialize_zero` function
        t.call("initialize_zero")
        t.check_scalar("a0",26)
        t.execute(fail="malloc",code=26)



    # Add other test cases if neccesary
    @classmethod
    def tearDownClass(cls):
        print_coverage("initialize_zero.s", verbose=False)
