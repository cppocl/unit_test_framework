/*
Copyright 2016 Colin Girling

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************

description: Sample tests for template array class.
*/

#include "../test/Test.hpp"
#include "Array.hpp"

typedef ocl::Array<char> simple_string;

TEST_MEMBER_FUNCTION(Array, Array, char_const_ptr_int_ref)
{
    TEST_OVERRIDE_ARGS("char const*, int&");

    {
        simple_string str;
        CHECK_TRUE(str.IsEmpty());
        CHECK_FALSE(str.GetSize() > 0);
    }

    {
        simple_string str(1);
        CHECK_FALSE(str.IsEmpty());
        CHECK_EQUAL(str.GetSize(), 1U);
    }

    {
        simple_string str_other;
        str_other.Append('a');

        simple_string str(str_other);
        CHECK_FALSE(str.IsEmpty());
        CHECK_EQUAL(str.GetSize(), 1U);
    }
}

TEST_MEMBER_FUNCTION(Array, Array, Array_const_ref)
{
    TEST_OVERRIDE_FUNCTION_NAME_ARGS("operator =", "Array const&");

    simple_string str;
    simple_string str_other;

    str = str_other;
    CHECK_TRUE(str.IsEmpty());
    CHECK_EQUAL(str.Ptr(), static_cast<char const*>(NULL));
    CHECK_TRUE(str == str_other);
    CHECK_TRUE(!(str != str_other));

    str_other.Append('a');
    str = str_other;
    CHECK_TRUE(str == str_other);
    CHECK_TRUE(!(str != str_other));
}

TEST_MEMBER_FUNCTION(Array, subscript, size_t)
{
    TEST_OVERRIDE_FUNCTION_NAME("operator []");

    simple_string str;

    str.Append('a');
    CHECK_TRUE(str[0] == 'a');

    str.Append('\0');
    CHECK_TRUE(str[1] == '\0');

    str.DeleteAt(0);
    CHECK_EXCEPTION(str[1], std::out_of_range, true);

    str[0] = 'b';
    CHECK_TRUE(str[0] == 'b');
}

TEST_MEMBER_FUNCTION(Array, operator_plus_equal, Array_const_ref)
{
    TEST_OVERRIDE_FUNCTION_NAME_ARGS("operator +=", "Array const&");

    simple_string str, str_other;

    str += 'a';
    CHECK_FALSE(str.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 1U);
    CHECK_TRUE(str[0] == 'a');

    str += 'b';
    CHECK_FALSE(str.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 2U);
    CHECK_TRUE(str[0] == 'a');
    CHECK_TRUE(str[1] == 'b');

    str_other += str;
    CHECK_FALSE(str_other.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 2U);
    CHECK_TRUE(str_other[0] == 'a');
    CHECK_TRUE(str_other[1] == 'b');

    str_other.Clear();
    str_other += str;
    CHECK_FALSE(str_other.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 2U);
    CHECK_TRUE(str_other[0] == 'a');
    CHECK_TRUE(str_other[1] == 'b');
}

TEST_MEMBER_FUNCTION(Array, operator_equality, Array_const_ref)
{
    TEST_OVERRIDE_FUNCTION_NAME_ARGS("operator +=", "Array const&");

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
}

TEST_CONST_MEMBER_FUNCTION(Array, IsEmpty, NA)
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
}

TEST_CONST_MEMBER_FUNCTION(Array, GetSize, NA)
{
    simple_string str;
    CHECK_EQUAL(str.GetSize(), 0U);

    str.Append('a');
    CHECK_EQUAL(str.GetSize(), 1U);

    simple_string str_other(str);
    CHECK_EQUAL(str_other.GetSize(), 1U);

    str.Append(str_other);
    CHECK_EQUAL(str.GetSize(), 2U);
}

TEST_MEMBER_FUNCTION(Array, Clear, NA)
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
}

TEST_MEMBER_FUNCTION(Array, Ptr, NA)
{
    // Test const and non-const versions.
    simple_string str;
    simple_string const& ref_str(str);
    CHECK_EQUAL(str.Ptr(), static_cast<char const*>(NULL));
    CHECK_EQUAL(ref_str.Ptr(), static_cast<char const*>(NULL));

    str.Append('a');
    CHECK_NOT_EQUAL(str.Ptr(), static_cast<char const*>(NULL));
    CHECK_NOT_EQUAL(ref_str.Ptr(), static_cast<char const*>(NULL));
    CHECK_EQUAL(*str.Ptr(), 'a');
    CHECK_EQUAL(*ref_str.Ptr(), 'a');
}

TEST_MEMBER_FUNCTION(Array, Copy, Array_const_ref)
{
    TEST_OVERRIDE_ARGS("Array const&");

    simple_string str, str_other;
    str.Append('a');
    str.Copy(str_other);
    CHECK_TRUE(str.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 0U);

    str_other.Append('b');
    str.Copy(str_other);
    CHECK_EQUAL(str[0], 'b');
    CHECK_FALSE(str.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 1U);
}

TEST_MEMBER_FUNCTION(Array, Copy, Array_ref)
{
    TEST_OVERRIDE_ARGS("Array &");

    simple_string str, str_other;
    str.Move(str_other);
    CHECK_TRUE(str.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 0U);
    CHECK_TRUE(str_other.IsEmpty());
    CHECK_EQUAL(str_other.GetSize(), 0U);

    str_other.Append('a');
    char const* ptr = str_other.Ptr();
    str.Move(str_other);
    CHECK_FALSE(str.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 1U);
    CHECK_EQUAL(str[0], 'a');
    CHECK_EQUAL(str.Ptr(), ptr);
    CHECK_TRUE(str_other.IsEmpty());
    CHECK_EQUAL(str_other.GetSize(), 0U);
    CHECK_EQUAL(str_other.Ptr(), static_cast<char const*>(NULL));
}

TEST_MEMBER_FUNCTION(Array, Append, Array_const_ref)
{
    TEST_OVERRIDE_ARGS("Array const&");

    simple_string str, str_other;

    str.Append('a');
    CHECK_FALSE(str.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 1U);
    CHECK_EQUAL(str[0], 'a');

    str.Append('b');
    CHECK_FALSE(str.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 2U);
    CHECK_EQUAL(str[0], 'a');
    CHECK_EQUAL(str[1], 'b');

    str_other.Append(str);
    CHECK_FALSE(str_other.IsEmpty());
    CHECK_EQUAL(str_other.GetSize(), 2U);
    CHECK_EQUAL(str_other[0], 'a');
    CHECK_EQUAL(str_other[1], 'b');

    str_other.Clear();
    str_other.Append(str);
    CHECK_FALSE(str_other.IsEmpty());
    CHECK_EQUAL(str_other.GetSize(), 2U);
    CHECK_EQUAL(str_other[0], 'a');
    CHECK_EQUAL(str_other[1], 'b');
}

TEST_MEMBER_FUNCTION(Array, DeleteAt, size_t)
{
    simple_string str;
    CHECK_EXCEPTION(str.DeleteAt(0), std::out_of_range, true)

    str.Append('a');
    CHECK_EXCEPTION(str.DeleteAt(1), std::out_of_range, true)

    CHECK_EXCEPTION(str.DeleteAt(0), std::out_of_range, false)
    CHECK_TRUE(str.IsEmpty());
    CHECK_EQUAL(str.Ptr(), static_cast<char const*>(NULL));

    CHECK_EXCEPTION(str[0], std::out_of_range, true)

    str.Append('a');
    str.Append('b');
    CHECK_EXCEPTION(str.DeleteAt(0), std::out_of_range, false)
    CHECK_FALSE(str.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 1U);
    CHECK_EQUAL(str[0], 'b');

    str.Clear();
    str.Append('a');
    str.Append('b');
    CHECK_EXCEPTION(str.DeleteAt(1), std::out_of_range, false)
    CHECK_FALSE(str.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 1U);
    CHECK_EQUAL(str[0], 'a');

    str.Clear();
    str.Append('a');
    str.Append('b');
    str.Append('c');
    CHECK_EXCEPTION(str.DeleteAt(1), std::out_of_range, false)
    CHECK_FALSE(str.IsEmpty());
    CHECK_EQUAL(str.GetSize(), 2U);
    CHECK_EQUAL(str[0], 'a');
    CHECK_EQUAL(str[1], 'c');
}

TEST_MEMBER_FUNCTION(Array, InsertAt, size_t_Array_const_ref)
{
    TEST_OVERRIDE_ARGS("Array const&");

    {
        simple_string str;
        CHECK_EXCEPTION(str.InsertAt(1, 'a'), std::out_of_range, true)
        CHECK_TRUE(str.IsEmpty());
        CHECK_EQUAL(str.GetSize(), 0U);
    }

    {
        simple_string str;
        CHECK_EXCEPTION(str.InsertAt(0, 'a'), std::out_of_range, false)
        CHECK_FALSE(str.IsEmpty());
        CHECK_EQUAL(str.GetSize(), 1U);
        CHECK_EQUAL(str[0], 'a');
    }

    {
        simple_string str, str_other;
        str_other.Append('a');
        CHECK_EXCEPTION(str.InsertAt(0, str_other), std::out_of_range, false)
        CHECK_FALSE(str.IsEmpty());
        CHECK_EQUAL(str.GetSize(), 1U);
        CHECK_EQUAL(str[0], 'a');

        str_other[0] = 'b';
        CHECK_EXCEPTION(str.InsertAt(0, str_other), std::out_of_range, false)
        CHECK_FALSE(str.IsEmpty());
        CHECK_EQUAL(str.GetSize(), 2U);
        CHECK_EQUAL(str[0], 'b');
        CHECK_EQUAL(str[1], 'a');

        str_other[0] = 'c';
        CHECK_EXCEPTION(str.InsertAt(2, str_other), std::out_of_range, false)
        CHECK_FALSE(str.IsEmpty());
        CHECK_EQUAL(str.GetSize(), 3U);
        CHECK_EQUAL(str[0], 'b');
        CHECK_EQUAL(str[1], 'a');
        CHECK_EQUAL(str[2], 'c');
    }
}

int main(int /*argv*/, char* /*argv*/[])
{
}
