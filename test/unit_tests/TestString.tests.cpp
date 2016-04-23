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

    {
        TestString str('a', 1U);
        CHECK_FALSE(str.IsEmpty());
        CHECK_ZERO(StrCmp(str.Ptr(), "a"));
    }

    {
        TestString str('b', 2U);
        CHECK_FALSE(str.IsEmpty());
        CHECK_ZERO(StrCmp(str.Ptr(), "bb"));
    }
}

TEST_MEMBER_FUNCTION(TestString, TestString, TestString_const_ref)
{
    using ocl::TestString;

    TEST_OVERRIDE_ARGS("TestString const&");

    {
        TestString str_src;
        TestString str(str_src);
        CHECK_TRUE(str.IsEmpty());
    }

    {
        TestString str_src("Hello");
        TestString str(str_src);
        CHECK_FALSE(str.IsEmpty());
        CHECK_ZERO(StrCmp(str.Ptr(), str_src.Ptr()));
    }
}

TEST_MEMBER_FUNCTION(TestString, operator_assignment, char_const_ptr)
{
    using ocl::TestString;

    TEST_OVERRIDE_FUNCTION_NAME_ARGS("operator =", "char const*");

    char const* hello_str = "Hello";

    TestString str;
    str = "";
    CHECK_TRUE(str.IsEmpty());
    CHECK_ZERO(str.GetLength());
    str = hello_str;
    CHECK_FALSE(str.IsEmpty());
    CHECK_ZERO(StrCmp(str.Ptr(), hello_str));
    CHECK_EQUAL(str.GetLength(), StrLen(hello_str));
}

TEST_MEMBER_FUNCTION(TestString, operator_assignment, TestString_const_ref)
{
    using ocl::TestString;

    TEST_OVERRIDE_FUNCTION_NAME_ARGS("operator =", "TestString const&");

    TestString empty_string;
    TestString hello_string("Hello");

    TestString str;
    str = empty_string;
    CHECK_TRUE(str.IsEmpty());
    CHECK_ZERO(str.GetLength());

    str = hello_string;
    CHECK_FALSE(str.IsEmpty());
    CHECK_ZERO(StrCmp(str.Ptr(), hello_string.Ptr()));
    CHECK_EQUAL(str.GetLength(), hello_string.GetLength());

    // Ensure assigning to nothing will clear any existing string.
    str = empty_string;
    CHECK_TRUE(str.IsEmpty());
    CHECK_ZERO(str.GetLength());
}

TEST_MEMBER_FUNCTION(TestString, operator_plus_equal, char_const_ptr)
{
    using ocl::TestString;

    TEST_OVERRIDE_FUNCTION_NAME_ARGS("operator +=", "char const* str");

    TestString str;
    str += static_cast<char const*>(NULL);
    CHECK_TRUE(str.IsEmpty());
    CHECK_EQUAL(str.GetLength(), 0U);

    str += "";
    CHECK_TRUE(str.IsEmpty());
    CHECK_EQUAL(str.GetLength(), 0U);

    str += "H";
    CHECK_ZERO(StrCmp(str.Ptr(), "H"));
    CHECK_EQUAL(str.GetLength(), 1U);

    str += "ello";
    CHECK_ZERO(StrCmp(str.Ptr(), "Hello"));
    CHECK_EQUAL(str.GetLength(), 5U);
}

TEST_MEMBER_FUNCTION(TestString, operator_plus_equal, TestString_const_ref)
{
    using ocl::TestString;

    TEST_OVERRIDE_FUNCTION_NAME_ARGS("operator +=", "TestString const&");

    TestString empty_string;
    TestString str;

    str += empty_string;
    CHECK_TRUE(str.IsEmpty());
    CHECK_EQUAL(str.GetLength(), 0U);

    TestString h_str("H");
    str += h_str;
    CHECK_ZERO(StrCmp(str.Ptr(), h_str.Ptr()));
    CHECK_EQUAL(str.GetLength(), 1U);

    TestString ello_str("ello");
    str += ello_str;
    CHECK_ZERO(StrCmp(str.Ptr(), "Hello"));
    CHECK_EQUAL(str.GetLength(), 5U);
}

TEST_CONST_MEMBER_FUNCTION(TestString, operator_subscript, size_type)
{
    using ocl::TestString;

    TEST_OVERRIDE_FUNCTION_NAME("operator []");

    TestString const str("Hello");

    CHECK_EQUAL(str[0U], 'H');
    CHECK_EQUAL(str[4U], 'o');
}

TEST_MEMBER_FUNCTION(TestString, operator_subscript, size_type)
{
    using ocl::TestString;

    TEST_OVERRIDE_FUNCTION_NAME("operator []");

    TestString str("Hello");

    char& ch0 = str[0U];
    CHECK_EQUAL(ch0, 'H');
    ch0 = 'h';
    CHECK_EQUAL(str[0U], 'h');

    char& ch4 = str[4U];
    CHECK_EQUAL(ch4, 'o');
    ch4 = 'Z';
    CHECK_EQUAL(str[4U], 'Z');
}

TEST_CONST_MEMBER_FUNCTION(TestString, char_const_ptr, NA)
{
    using ocl::TestString;

    TEST_OVERRIDE_FUNCTION_NAME("char const*");

    char const* hello_str = "Hello";

    TestString str;
    char const* ptr = static_cast<char const*>(str);
    CHECK_NOT_NULL(ptr);
    CHECK_EQUAL(*ptr, '\0');

    str = hello_str;
    ptr = static_cast<char const*>(str);
    CHECK_NOT_NULL(ptr);
    CHECK_ZERO(StrCmp(ptr, hello_str));
    CHECK_EQUAL(StrLen(ptr), 5U);
}

TEST_CONST_MEMBER_FUNCTION(TestString, operator_is_equal, char_const_ptr)
{
    using ocl::TestString;

    TEST_OVERRIDE_FUNCTION_NAME_ARGS("operator ==", "char const*");

    TestString str;
    CHECK_TRUE(str == "");
    CHECK_TRUE(str == static_cast<char const*>(NULL));

    str = "A";
    CHECK_TRUE(str == "A");
    CHECK_FALSE(str == "a");
}

TEST_MEMBER_FUNCTION(TestString, operator_is_equal, TestString_const_ref)
{
    using ocl::TestString;

    TEST_OVERRIDE_FUNCTION_NAME_ARGS("operator ==", "TestString const&");

    TestString empty_string;
    TestString a_str("a");
    TestString A_str("A");

    TestString str;
    CHECK_TRUE(str == empty_string);

    str = A_str;
    CHECK_TRUE(str == A_str);
    CHECK_FALSE(str == a_str);
}

TEST_CONST_MEMBER_FUNCTION(TestString, Ptr, NA)
{
    using ocl::TestString;

    char const* hello_string = "Hello";

    TestString str;
    CHECK_NOT_NULL(str.Ptr());
    CHECK_ZERO(StrCmp(str.Ptr(), ""));

    str = hello_string;

    // Make sure assignment of hello_string does not just reference the same pointer.
    CHECK_NOT_EQUAL(str.Ptr(), hello_string);

    CHECK_ZERO(StrCmp(str.Ptr(), hello_string));
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
