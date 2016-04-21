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
*/

#include "../Test.hpp"
#include "../TestString.hpp"

TEST_MEMBER_FUNCTION(TestString, TestString, char_const_ptr)
{
    using ocl::TestString;

    TEST_OVERRIDE_ARGS("char const*");

    {
        TestString empty_string;
        CHECK_TRUE(empty_string.IsEmpty());
    }

    {
        TestString empty_string(NULL);
        CHECK_TRUE(empty_string.IsEmpty());
    }

    {
        TestString str("Hello");
        CHECK_FALSE(str.IsEmpty());
        CHECK_EQUAL(str.GetLength(), 5U);
    }
}

TEST_MEMBER_FUNCTION(TestString, TestString, char_size_type)
{
    using ocl::TestString;

    TEST_OVERRIDE_ARGS("char,size_type");

}

TEST_MEMBER_FUNCTION(TestString, TestString, TestString_const_ref)
{
    using ocl::TestString;

    TEST_OVERRIDE_ARGS("TestString const&");

}

TEST_MEMBER_FUNCTION(TestString, operator_assignment, char_const_ptr)
{
    using ocl::TestString;

    TEST_OVERRIDE_FUNCTION_NAME_ARGS("operator =", "char const*");

}

TEST_MEMBER_FUNCTION(TestString, operator_assignment, TestString_const_ref)
{
    using ocl::TestString;

    TEST_OVERRIDE_FUNCTION_NAME_ARGS("operator =", "TestString const&");

}

TEST_MEMBER_FUNCTION(TestString, operator_plus_equal, TestString_const_ref)
{
    using ocl::TestString;

    TEST_OVERRIDE_FUNCTION_NAME_ARGS("operator +=", "TestString const&");

}

TEST_CONST_MEMBER_FUNCTION(TestString, operator_subscript, size_type)
{
    using ocl::TestString;

    TEST_OVERRIDE_FUNCTION_NAME("operator []");

}

TEST_MEMBER_FUNCTION(TestString, operator_subscript, size_type)
{
    using ocl::TestString;

    TEST_OVERRIDE_FUNCTION_NAME("operator []");

}

TEST_CONST_MEMBER_FUNCTION(TestString, char_const_ptr, NA)
{
    using ocl::TestString;

    TEST_OVERRIDE_FUNCTION_NAME("char const*");

}

TEST_CONST_MEMBER_FUNCTION(TestString, operator_is_equal, char_const_ptr)
{
    using ocl::TestString;

    TEST_OVERRIDE_FUNCTION_NAME_ARGS("operator ==", "char const*");

}

TEST_MEMBER_FUNCTION(TestString, operator_is_equal, TestString_const_ref)
{
    using ocl::TestString;

    TEST_OVERRIDE_FUNCTION_NAME_ARGS("operator ==", "TestString const&");

}

TEST_CONST_MEMBER_FUNCTION(TestString, Ptr, NA)
{
    using ocl::TestString;

}

TEST_CONST_MEMBER_FUNCTION(TestString, GetLength, NA)
{
    using ocl::TestString;

}

TEST_CONST_MEMBER_FUNCTION(TestString, IsEmpty, NA)
{
    using ocl::TestString;

}

TEST_MEMBER_FUNCTION(TestString, Clear, NA)
{
    using ocl::TestString;

}

TEST_CONST_MEMBER_FUNCTION(TestString, Find, char_size_type_ref_size_type)
{
    using ocl::TestString;

    TEST_OVERRIDE_ARGS("char,size_type&,size_type");

}

TEST_MEMBER_FUNCTION(TestString, GetSubString, TestString_ref_size_type_size_type_bool)
{
    using ocl::TestString;

    TEST_OVERRIDE_ARGS("TestString&,size_type,size_type,bool");

}

TEST_MEMBER_FUNCTION(TestString, Assign, char_const_ptr)
{
    using ocl::TestString;

    TEST_OVERRIDE_ARGS("char const*");

}

TEST_MEMBER_FUNCTION(TestString, Assign, TestString_const_ref)
{
    using ocl::TestString;

    TEST_OVERRIDE_ARGS("TestString const&");

}

TEST_MEMBER_FUNCTION(TestString, Move, char_ptr_ref_size_type)
{
    using ocl::TestString;

    TEST_OVERRIDE_ARGS("char*&,size_type");

}

TEST_MEMBER_FUNCTION(TestString, Prepend, char_const_ptr)
{
    using ocl::TestString;

    TEST_OVERRIDE_ARGS("char const*");

}

TEST_MEMBER_FUNCTION(TestString, Append, char_const_ptr)
{
    using ocl::TestString;

    TEST_OVERRIDE_ARGS("char const*");

}

TEST_MEMBER_FUNCTION(TestString, Append, char_const_ptr_size_type)
{
    using ocl::TestString;

    TEST_OVERRIDE_ARGS("char const*,size_type");

}

TEST_MEMBER_FUNCTION(TestString, Append, TestString_const_ptr)
{
    using ocl::TestString;

    TEST_OVERRIDE_ARGS("TestString const&");

}

TEST_MEMBER_FUNCTION(TestString, Append, bool)
{
    using ocl::TestString;

}

TEST_MEMBER_FUNCTION(TestString, Append, char)
{
    using ocl::TestString;

}

TEST_MEMBER_FUNCTION(TestString, Append, char_size_type)
{
    using ocl::TestString;

    TEST_OVERRIDE_ARGS("char,size_type");

}

TEST_MEMBER_FUNCTION(TestString, Append, char_size_type_char_const_ptr_size_type)
{
    using ocl::TestString;

    TEST_OVERRIDE_ARGS("char,size_type,char const*,size_type");

}

TEST_MEMBER_FUNCTION(TestString, Append, signed_char_size_type)
{
    using ocl::TestString;

    TEST_OVERRIDE_ARGS("signed char,size_type");

}

TEST_MEMBER_FUNCTION(TestString, Append, unsigned_char_size_type)
{
    using ocl::TestString;

    TEST_OVERRIDE_ARGS("unsigned char,size_type");

}

TEST_MEMBER_FUNCTION(TestString, Append, signed_short_size_type)
{
    using ocl::TestString;

    TEST_OVERRIDE_ARGS("signed short,size_type");

}

TEST_MEMBER_FUNCTION(TestString, Append, unsigned_short_size_type)
{
    using ocl::TestString;

    TEST_OVERRIDE_ARGS("unsigned short,size_type");

}

TEST_MEMBER_FUNCTION(TestString, Append, signed_int_size_type)
{
    using ocl::TestString;

    TEST_OVERRIDE_ARGS("signed int,size_type");

}

TEST_MEMBER_FUNCTION(TestString, Append, unsigned_int_size_type)
{
    using ocl::TestString;

    TEST_OVERRIDE_ARGS("unsigned int,size_type");

}

TEST_MEMBER_FUNCTION(TestString, Append, signed_long_size_type)
{
    using ocl::TestString;

    TEST_OVERRIDE_ARGS("signed long,size_type");

}

TEST_MEMBER_FUNCTION(TestString, Append, unsigned_long_size_type)
{
    using ocl::TestString;

    TEST_OVERRIDE_ARGS("unsigned long,size_type");

}
