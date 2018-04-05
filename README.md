# C++ Unit Test Framework

![](header_image.jpg)

## Features

This unit test framework provides a variety of features in a single library, including:
* Testing for success with CHECK_TRUE, CHECK_EQUAL, etc.
* Identify tests as functions, member functions and const member functions.
* Pass or fail a test based on the performance.
* Fixtures (setup and teardown).
* Fail on memory leaks (currently only Windows).
* Single header file include for all features.
* Override test output for function names and arguments.
* Provide custom logging (replace stdio output).
* Test for C++ exceptions.

## How to use

Copy the `test` folder into any location you wish to share the unit test code from, then include `Test.hpp` from the test folder into your source file with the tests.

## Code Examples

```cpp
#include "test/Test.hpp"

TEST(MyTest)
{
    int a = 1;
    CHECK_TRUE(a == 1);

    CHECK_EQUAL(a, 1);
}

class MyClass
{
public:
    int MultiplyBy2(int i)
    {
        return i * 2;
    }

    int Multiply(int i1, int i2)
    {
        return i1 * i2;
    }
};

TEST_MEMBER_FUNCTION(MyClass, MultiplyBy2, int)
{
    MyClass a;
    CHECK_EQUAL(a.MultiplyBy2(2), 4);
}

TEST_MEMBER_FUNCTION(MyClass, Multiply, int_int)
{
    TEST_OVERRIDE_ARGS("int, int");

    MyClass a;
    CHECK_EQUAL(a.Multiply(2, 3), 6);
}
```
