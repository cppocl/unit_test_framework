#include "Test.hpp"

// Example functions for demonstrating how to write unit tests
// for functions using TEST_FUNCTION.

bool FuncOneArg(int i)
{
    return i != 0;
}

bool FuncTwoArgs(int i1, int i2)
{
    return i1 + i2 != 0;
}

// Example class for demonstrating how to write unit tests
// for member functions and const member functions using 
// TEST_MEMBER_FUNCTION and TEST_CONST_MEMBER_FUNCTION.
//
// There are two versions for testing member functions,
// so that the output can display overloaded const and non-const versions.

class Example
{
public:
    Example(int value = 0)
        : m_value(value)
    {
    }

    Example(Example const& other)
        : m_value(other.m_value)
    {
    }

    Example& operator =(Example const& other)
    {
        m_value = other.m_value;
        return *this;
    }

    bool operator ==(Example const& other) const
    {
        return m_value == other.m_value;
    }

    int GetValue() const
    {
        return m_value;
    }

    void SetValue(int value)
    {
        m_value = value;
    }

    void Inc()
    {
        ++m_value;
    }

private:
    int m_value;
};


// Implement unit tests for functions and member functions.

TEST_FUNCTION(FuncOneArg, int)
{
    CHECK_FALSE(FuncOneArg(0));
    CHECK_TRUE(FuncOneArg(1));
    CHECK_TRUE(FuncOneArg(-1));
}

// Because the arguments within TEST_FUNCTION can only use characters that
// would be valid for a variable name, the ARGS part of TEST_FUNCTION uses
// underscore to represent comma separated function arguments.
TEST_FUNCTION(FuncOneArg, int_int)
{
    TEST_OVERRIDE_ARGS("int,int");
}

// Because we cannot use == in function name for macro, is_equal is used
// and TEST_OVERRIDE_FUNCTION_NAME is used to update output in logging.
TEST_CONST_MEMBER_FUNCTION(Example, operator_is_equal, Example_const_ref)
{
    TEST_OVERRIDE_FUNCTION_NAME("operator ==");
    TEST_OVERRIDE_ARGS("Example const&");

    /*
     * Could also use TEST_OVERRIDE_FUNCTION_NAME_ARGS, but for this example
     * to show how to set name and args separately, the above was used.
     * Could also do this: TEST_OVERRIDE_FUNCTION_NAME("operator ==", "Example const&");
     */

    Example example1, example2;
    CHECK_TRUE(example1 == example2);

    example1.SetValue(1);
    CHECK_FALSE(example1 == example2);

    example2.SetValue(1);
    CHECK_TRUE(example1 == example2);
}

// Test the constructor for Example class.
// When args is set to NA the framework understand the function has no arguments.
// NA is an abbreviation for Not Applicable.
TEST_CONST_MEMBER_FUNCTION(Example, GetValue, NA)
{
    Example example;

    CHECK_ZERO(example.GetValue());
    example.SetValue(1);
    CHECK_EQUAL(example.GetValue(), 1);
}

TEST_MEMBER_FUNCTION(Example, Inc, NA)
{
    Example example;
    CHECK_ZERO(example.GetValue());
    example.Inc();
    CHECK_EQUAL(example.GetValue(), 1);
}
