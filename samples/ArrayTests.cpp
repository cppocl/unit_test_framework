#include "Array.hpp"
#include <cstring>
#include <cassert>
#include <iostream>

const int NO_ERROR = 0;
const int GENERAL_ERROR = 1;
const int EXCEPTION_ERROR = 2;

typedef ocl::Array<char> simple_string;

// Test a function that can throw an exception and see if we were expecting it.
#define EXCEPTION_CHECK(func, exception_type, expect_exception) \
{ \
    bool found_exception = false; \
    try \
    { \
        func; \
    } \
    catch (exception_type&) \
    { \
        found_exception = true; \
    } \
    if ((expect_exception && !found_exception) || (!expect_exception && found_exception)) \
        return Error(); \
}

#define EXCEPTION_RANGE_CHECK(func, want_exception) \
    EXCEPTION_CHECK(func, std::out_of_range, want_exception)

#define CHECK_TRUE(exp) if (!(exp)) return Error()
#define CHECK_FALSE(exp) if (exp) return Error()
#define CHECK_EQUAL(exp, value) if (exp != value) return Error()
#define CHECK_NOT_EQUAL(exp, value) if (exp == value) return Error()

namespace
{

int Error()
{
    // Put break point here to see what failed...
    std::cerr << "Error detected!" << std::endl;
    return GENERAL_ERROR;
}

int TestConstruction()
{
    {
        simple_string str;
        CHECK_TRUE(str.IsEmpty());
        CHECK_FALSE(str.GetSize() > 0);
    }

    {
        simple_string str(1);
        CHECK_FALSE(str.IsEmpty());
        CHECK_EQUAL(str.GetSize(), 1);
    }

    {
        simple_string str_other;
        str_other.Append('a');

        simple_string str(str_other);
        CHECK_FALSE(str.IsEmpty());
        CHECK_EQUAL(str.GetSize(), 1);
    }

    return NO_ERROR;
}

int TestAssignment()
{
    simple_string str, str_other;

    str = str_other;
    CHECK_TRUE(str.IsEmpty());
    CHECK_EQUAL(str.Ptr(), NULL);
    CHECK_TRUE(str == str_other);
    CHECK_TRUE(!(str != str_other));

    str_other.Append('a');
    str = str_other;
    CHECK_TRUE(str == str_other);
    CHECK_TRUE(!(str != str_other));

    return NO_ERROR;
}

int TestSubscript()
{
    simple_string str;
    EXCEPTION_RANGE_CHECK(str[0], true)

    str.Append('a');
    CHECK_TRUE(str[0] == 'a');

    str.Append('\0');
    CHECK_TRUE(str[1] == '\0');

    str.DeleteAt(0);
    EXCEPTION_RANGE_CHECK(str[1], true)

    str[0] = 'b';
    CHECK_TRUE(str[0] == 'b');

    return NO_ERROR;
}

int TestPlusEqual()
{
    simple_string str, str_other;

    str += 'a';
    CHECK_FALSE(str.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 1);
    CHECK_TRUE(str[0] == 'a');

    str += 'b';
    CHECK_FALSE(str.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 2);
    CHECK_TRUE(str[0] == 'a');
    CHECK_TRUE(str[1] == 'b');

    str_other += str;
    CHECK_FALSE(str_other.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 2);
    CHECK_TRUE(str_other[0] == 'a');
    CHECK_TRUE(str_other[1] == 'b');

    str_other.Clear();
    str_other += str;
    CHECK_FALSE(str_other.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 2);
    CHECK_TRUE(str_other[0] == 'a');
    CHECK_TRUE(str_other[1] == 'b');

    return NO_ERROR;
}

int TestEquality()
{
    simple_string str1, str2;
    CHECK_TRUE(str1 == str2);
    CHECK_TRUE(!(str1 != str2));

    str1 += 'a';
    CHECK_FALSE(str1 == str2);
    CHECK_FALSE(str2 == str1);

    str2 += 'a';
    CHECK_TRUE(str1 == str2);
    CHECK_TRUE(str2 == str1);
    CHECK_TRUE(!(str1 != str2));
    CHECK_TRUE(!(str2 != str1));

    str1 += 'b';
    str2 += 'b';
    CHECK_TRUE(str1 == str2);
    CHECK_TRUE(str2 == str1);
    CHECK_TRUE(!(str1 != str2));
    CHECK_TRUE(!(str2 != str1));

    str1 += 'c';
    str2 += 'd';
    CHECK_TRUE(!(str1 == str2));
    CHECK_TRUE(!(str2 == str1));
    CHECK_TRUE(str1 != str2);
    CHECK_TRUE(str2 != str1);

    return NO_ERROR;
}

int TestIsEmpty()
{
    simple_string str;
    CHECK_TRUE(str.IsEmpty());

    str.Append('a');
    CHECK_FALSE(str.IsEmpty());

    simple_string str_other(str);
    CHECK_FALSE(str_other.IsEmpty());

    str.DeleteAt(0);
    CHECK_TRUE(str.IsEmpty());

    str += str_other;
    CHECK_FALSE(str.IsEmpty());

    return NO_ERROR;
}

int TestGetSize()
{
    simple_string str;
    CHECK_EQUAL(str.GetSize(), 0);

    str.Append('a');
    CHECK_EQUAL(str.GetSize(), 1);

    simple_string str_other(str);
    CHECK_EQUAL(str_other.GetSize(), 1);

    str.Append(str_other);
    CHECK_EQUAL(str.GetSize(), 2);

    return NO_ERROR;
}

int TestClear()
{
    simple_string str;
    str.Clear();
    CHECK_TRUE(str.IsEmpty());

    str.Append('a');
    CHECK_FALSE(str.IsEmpty());

    simple_string str_other('a');
    CHECK_FALSE(str_other.IsEmpty());

    str.Clear();
    CHECK_TRUE(str.IsEmpty());

    str = str_other;
    str.Clear();
    CHECK_TRUE(str.IsEmpty());
    CHECK_FALSE(str_other.IsEmpty());

    return NO_ERROR;
}

int TestPtr()
{
    // Test const and non-const versions.
    simple_string str;
    simple_string const& ref_str(str);
    CHECK_EQUAL(str.Ptr(), NULL);
    CHECK_EQUAL(ref_str.Ptr(), NULL);

    str.Append('a');
    CHECK_NOT_EQUAL(str.Ptr(), NULL);
    CHECK_NOT_EQUAL(ref_str.Ptr(), NULL);
    CHECK_EQUAL(*str.Ptr(), 'a');
    CHECK_EQUAL(*ref_str.Ptr(), 'a');

    return NO_ERROR;
}

int TestCopy()
{
    simple_string str, str_other;
    str.Append('a');
    str.Copy(str_other);
    CHECK_TRUE(str.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 0);

    str_other.Append('b');
    str.Copy(str_other);
    CHECK_EQUAL(str[0], 'b');
    CHECK_FALSE(str.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 1);

    return NO_ERROR;
}

int TestMove()
{
    simple_string str, str_other;
    str.Move(str_other);
    CHECK_TRUE(str.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 0);
    CHECK_TRUE(str_other.IsEmpty());
    CHECK_EQUAL(str_other.GetSize(), 0);

    str_other.Append('a');
    char const* ptr = str_other.Ptr();
    str.Move(str_other);
    CHECK_FALSE(str.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 1);
    CHECK_EQUAL(str[0], 'a');
    CHECK_EQUAL(str.Ptr(), ptr);
    CHECK_TRUE(str_other.IsEmpty());
    CHECK_EQUAL(str_other.GetSize(), 0);
    CHECK_EQUAL(str_other.Ptr(), NULL);

    return NO_ERROR;
}

int TestAppend()
{
    simple_string str, str_other;

    str.Append('a');
    CHECK_FALSE(str.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 1);
    CHECK_EQUAL(str[0], 'a');

    str.Append('b');
    CHECK_FALSE(str.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 2);
    CHECK_EQUAL(str[0], 'a');
    CHECK_EQUAL(str[1], 'b');

    str_other.Append(str);
    CHECK_FALSE(str_other.IsEmpty());
    CHECK_EQUAL(str_other.GetSize(), 2);
    CHECK_EQUAL(str_other[0], 'a');
    CHECK_EQUAL(str_other[1], 'b');

    str_other.Clear();
    str_other.Append(str);
    CHECK_FALSE(str_other.IsEmpty());
    CHECK_EQUAL(str_other.GetSize(), 2);
    CHECK_EQUAL(str_other[0], 'a');
    CHECK_EQUAL(str_other[1], 'b');

    return NO_ERROR;
}

int TestDeleteAt()
{
    simple_string str;
    EXCEPTION_RANGE_CHECK(str.DeleteAt(0), true)

    str.Append('a');
    EXCEPTION_RANGE_CHECK(str.DeleteAt(1), true)

    EXCEPTION_RANGE_CHECK(str.DeleteAt(0), false)
    CHECK_TRUE(str.IsEmpty());
    CHECK_EQUAL(str.Ptr(), NULL);

    EXCEPTION_RANGE_CHECK(str[0], true)

    str.Append('a');
    str.Append('b');
    EXCEPTION_RANGE_CHECK(str.DeleteAt(0), false)
    CHECK_FALSE(str.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 1);
    CHECK_EQUAL(str[0], 'b');

    str.Clear();
    str.Append('a');
    str.Append('b');
    EXCEPTION_RANGE_CHECK(str.DeleteAt(1), false)
    CHECK_FALSE(str.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 1);
    CHECK_EQUAL(str[0], 'a');

    str.Clear();
    str.Append('a');
    str.Append('b');
    str.Append('c');
    EXCEPTION_RANGE_CHECK(str.DeleteAt(1), false)
    CHECK_FALSE(str.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 2);
    CHECK_EQUAL(str[0], 'a');
    CHECK_EQUAL(str[1], 'c');

    return NO_ERROR;
}

int TestInsertAt()
{
    {
        simple_string str;
        EXCEPTION_RANGE_CHECK(str.InsertAt(1, 'a'), true)
        CHECK_TRUE(str.IsEmpty());
        CHECK_EQUAL(str.GetSize(), 0);
    }

    {
        simple_string str;
        EXCEPTION_RANGE_CHECK(str.InsertAt(0, 'a'), false)
        CHECK_FALSE(str.IsEmpty());
        CHECK_EQUAL(str.GetSize(), 1);
        CHECK_EQUAL(str[0], 'a');
    }

    {
        simple_string str, str_other;
        str_other.Append('a');
        EXCEPTION_RANGE_CHECK(str.InsertAt(0, str_other), false)
        CHECK_FALSE(str.IsEmpty());
        CHECK_EQUAL(str.GetSize(), 1);
        CHECK_EQUAL(str[0], 'a');

        str_other[0] = 'b';
        EXCEPTION_RANGE_CHECK(str.InsertAt(0, str_other), false)
        CHECK_FALSE(str.IsEmpty());
        CHECK_EQUAL(str.GetSize(), 2);
        CHECK_EQUAL(str[0], 'b');
        CHECK_EQUAL(str[1], 'a');

        str_other[0] = 'c';
        EXCEPTION_RANGE_CHECK(str.InsertAt(2, str_other), false)
        CHECK_FALSE(str.IsEmpty());
        CHECK_EQUAL(str.GetSize(), 3);
        CHECK_EQUAL(str[0], 'b');
        CHECK_EQUAL(str[1], 'a');
        CHECK_EQUAL(str[2], 'c');
    }

    return NO_ERROR;
}

int PerformTests()
{
    int ret;
    if ((ret = TestConstruction()) != NO_ERROR)
        return ret;

    if ((ret = TestAssignment()) != NO_ERROR)
        return ret;

    if ((ret = TestSubscript()) != NO_ERROR)
        return ret;

    if ((ret = TestPlusEqual()) != NO_ERROR)
        return ret;

    if ((ret = TestEquality()) != NO_ERROR)
        return ret;

    if ((ret = TestIsEmpty()) != NO_ERROR)
        return ret;

    if ((ret = TestGetSize()) != NO_ERROR)
        return ret;

    if ((ret = TestClear()) != NO_ERROR)
        return ret;

    if ((ret = TestPtr()) != NO_ERROR)
        return ret;

    if ((ret = TestCopy()) != NO_ERROR)
        return ret;

    if ((ret = TestMove()) != NO_ERROR)
        return ret;

    if ((ret = TestAppend()) != NO_ERROR)
        return ret;

    if ((ret = TestDeleteAt()) != NO_ERROR)
        return ret;

    if ((ret = TestInsertAt()) != NO_ERROR)
        return ret;

    std::cout << "Unit testing completed without errors" << std::endl;
    return NO_ERROR;
}

} // unanmed namespace

int main(int /*argv*/, char* /*argv*/[])
{
    return PerformTests();
}
