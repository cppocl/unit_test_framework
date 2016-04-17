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
#include "../TestStringUtility.hpp"
#include <cstddef>
#include <limits.h>

TEST_MEMBER_FUNCTION(TestStringUtility, GetMinSignedIntAsString, unsigned_int)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("unsigned int");

    char const* str = TestStringUtility::GetMinSignedIntAsString(1U);
    CHECK_ZERO(StrCmp(str, "-128"));

    str = TestStringUtility::GetMinSignedIntAsString(2U);
    CHECK_ZERO(StrCmp(str, "-32768"));

    str = TestStringUtility::GetMinSignedIntAsString(4U);
    CHECK_ZERO(StrCmp(str, "-2147483648"));

    str = TestStringUtility::GetMinSignedIntAsString(8U);
    CHECK_ZERO(StrCmp(str, "-9223372036854775808"));
}

TEST_MEMBER_FUNCTION(TestStringUtility, GetMaxSignedIntAsString, unsigned_int)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("unsigned int");

    char const* str = TestStringUtility::GetMaxSignedIntAsString(1U);
    CHECK_ZERO(StrCmp(str, "127"));

    str = TestStringUtility::GetMaxSignedIntAsString(2U);
    CHECK_ZERO(StrCmp(str, "32767"));

    str = TestStringUtility::GetMaxSignedIntAsString(4U);
    CHECK_ZERO(StrCmp(str, "2147483647"));

    str = TestStringUtility::GetMaxSignedIntAsString(8U);
    CHECK_ZERO(StrCmp(str, "9223372036854775807"));
}

TEST_MEMBER_FUNCTION(TestStringUtility, GetMinSignedIntCharCount, unsigned_int)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("unsigned int");

    CHECK_EQUAL(TestStringUtility::GetMinSignedIntCharCount(1U), 4U);
    CHECK_EQUAL(TestStringUtility::GetMinSignedIntCharCount(2U), 6U);
    CHECK_EQUAL(TestStringUtility::GetMinSignedIntCharCount(4U), 11U);
    CHECK_EQUAL(TestStringUtility::GetMinSignedIntCharCount(8U), 20U);
}

TEST_MEMBER_FUNCTION(TestStringUtility, GetMaxSignedIntCharCount, unsigned_int)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("unsigned int");

    CHECK_EQUAL(TestStringUtility::GetMaxSignedIntCharCount(1U), 3U);
    CHECK_EQUAL(TestStringUtility::GetMaxSignedIntCharCount(2U), 5U);
    CHECK_EQUAL(TestStringUtility::GetMaxSignedIntCharCount(4U), 10U);
    CHECK_EQUAL(TestStringUtility::GetMaxSignedIntCharCount(8U), 19U);
}

TEST_MEMBER_FUNCTION(TestStringUtility, GetMaxUnsignedIntCharCount, unsigned_int)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("unsigned int");

    CHECK_EQUAL(TestStringUtility::GetMaxUnsignedIntCharCount(1U), 3U);
    CHECK_EQUAL(TestStringUtility::GetMaxUnsignedIntCharCount(2U), 5U);
    CHECK_EQUAL(TestStringUtility::GetMaxUnsignedIntCharCount(4U), 10U);
    CHECK_EQUAL(TestStringUtility::GetMaxUnsignedIntCharCount(8U), 20U);
}

TEST_MEMBER_FUNCTION(TestStringUtility, GetMaxUnsignedIntAsString, unsigned_int)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("unsigned char");

    char const* str = TestStringUtility::GetMaxUnsignedIntAsString(1U);
    CHECK_ZERO(StrCmp(str, "255"));

    str = TestStringUtility::GetMaxUnsignedIntAsString(2U);
    CHECK_ZERO(StrCmp(str, "65535"));

    str = TestStringUtility::GetMaxUnsignedIntAsString(4U);
    CHECK_ZERO(StrCmp(str, "4294967295"));

    str = TestStringUtility::GetMaxUnsignedIntAsString(8U);
    CHECK_ZERO(StrCmp(str, "18446744073709551615"));
}


TEST_MEMBER_FUNCTION(TestStringUtility, GetString, bool)
{
    using ocl::TestStringUtility;

    size_t len = 0;
    char* str = TestStringUtility::GetString(true, len);
    CHECK_ZERO(StrCmp(str, "true"));
    CHECK_EQUAL(len, 4U);
    TestStringUtility::FastFree(str);

    str = TestStringUtility::GetString(false, len);
    CHECK_ZERO(StrCmp(str, "false"));
    CHECK_EQUAL(len, 5U);
    TestStringUtility::FastFree(str);
}

TEST_MEMBER_FUNCTION(TestStringUtility, GetString, char)
{
    using ocl::TestStringUtility;

    size_t len = 0;
    char* str = TestStringUtility::GetString('a', len);
    CHECK_ZERO(StrCmp(str, "a"));
    CHECK_EQUAL(len, 1U);
    TestStringUtility::FastFree(str);
}

TEST_MEMBER_FUNCTION(TestStringUtility, GetString, signed_char)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("signed char");
    typedef signed char type;
    unsigned int size_of_type = sizeof(type);

    type value = CHAR_MIN;
    char* str = TestStringUtility::GetString(value);
    CHECK_ZERO(StrCmp(str, TestStringUtility::GetMinSignedIntAsString(size_of_type)));
    CHECK_EQUAL(StrLen(str), TestStringUtility::GetMinSignedIntCharCount(size_of_type));
    TestStringUtility::FastFree(str);

    value = CHAR_MAX;
    str = TestStringUtility::GetString(value);
    CHECK_ZERO(StrCmp(str, TestStringUtility::GetMaxSignedIntAsString(size_of_type)));
    CHECK_EQUAL(StrLen(str), TestStringUtility::GetMaxSignedIntCharCount(size_of_type));
    TestStringUtility::FastFree(str);
}

TEST_MEMBER_FUNCTION(TestStringUtility, GetString, unsigned_char)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("unsigned char");
    typedef unsigned char type;
    unsigned int size_of_type = sizeof(type);

    type value = 0;
    char* str = TestStringUtility::GetString(value);
    CHECK_ZERO(StrCmp(str, "0"));
    CHECK_EQUAL(StrLen(str), 1U);
    TestStringUtility::FastFree(str);

    value = UCHAR_MAX;
    str = TestStringUtility::GetString(value);
    CHECK_ZERO(StrCmp(str, TestStringUtility::GetMaxUnsignedIntAsString(size_of_type)));
    CHECK_EQUAL(StrLen(str), TestStringUtility::GetMaxUnsignedIntCharCount(size_of_type));
    TestStringUtility::FastFree(str);
}

TEST_MEMBER_FUNCTION(TestStringUtility, GetString, signed_short)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("signed short");
    typedef signed short type;
    unsigned int size_of_type = sizeof(type);

    type value = SHRT_MIN;
    char* str = TestStringUtility::GetString(value);
    CHECK_ZERO(StrCmp(str, TestStringUtility::GetMinSignedIntAsString(size_of_type)));
    CHECK_EQUAL(StrLen(str), TestStringUtility::GetMinSignedIntCharCount(size_of_type));
    TestStringUtility::FastFree(str);

    value = SHRT_MAX;
    str = TestStringUtility::GetString(value);
    CHECK_ZERO(StrCmp(str, TestStringUtility::GetMaxSignedIntAsString(size_of_type)));
    CHECK_EQUAL(StrLen(str), TestStringUtility::GetMaxSignedIntCharCount(size_of_type));
    TestStringUtility::FastFree(str);
}

TEST_MEMBER_FUNCTION(TestStringUtility, GetString, unsigned_short)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("unsigned short");
    typedef unsigned short type;
    unsigned int size_of_type = sizeof(type);

    type value = 0;
    char* str = TestStringUtility::GetString(value);
    CHECK_ZERO(StrCmp(str, "0"));
    CHECK_EQUAL(StrLen(str), 1U);
    TestStringUtility::FastFree(str);

    value = USHRT_MAX;
    str = TestStringUtility::GetString(value);
    CHECK_ZERO(StrCmp(str, TestStringUtility::GetMaxUnsignedIntAsString(size_of_type)));
    CHECK_EQUAL(StrLen(str), TestStringUtility::GetMaxUnsignedIntCharCount(size_of_type));
    TestStringUtility::FastFree(str);
}

TEST_MEMBER_FUNCTION(TestStringUtility, GetString, signed_int)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("signed int");
    typedef signed int type;
    unsigned int size_of_type = sizeof(type);

    type value = INT_MIN;
    char* str = TestStringUtility::GetString(value);
    CHECK_ZERO(StrCmp(str, TestStringUtility::GetMinSignedIntAsString(size_of_type)));
    CHECK_EQUAL(StrLen(str), TestStringUtility::GetMinSignedIntCharCount(size_of_type));
    TestStringUtility::FastFree(str);

    value = INT_MAX;
    str = TestStringUtility::GetString(value);
    CHECK_ZERO(StrCmp(str, TestStringUtility::GetMaxSignedIntAsString(size_of_type)));
    CHECK_EQUAL(StrLen(str), TestStringUtility::GetMaxSignedIntCharCount(size_of_type));
    TestStringUtility::FastFree(str);
}

TEST_MEMBER_FUNCTION(TestStringUtility, GetString, unsigned_int)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("unsigned int");
    typedef unsigned int type;
    unsigned int size_of_type = sizeof(type);

    type value = 0;
    char* str = TestStringUtility::GetString(value);
    CHECK_ZERO(StrCmp(str, "0"));
    CHECK_EQUAL(StrLen(str), 1U);
    TestStringUtility::FastFree(str);

    value = UINT_MAX;
    str = TestStringUtility::GetString(value);
    CHECK_ZERO(StrCmp(str, TestStringUtility::GetMaxUnsignedIntAsString(size_of_type)));
    CHECK_EQUAL(StrLen(str), TestStringUtility::GetMaxUnsignedIntCharCount(size_of_type));
    TestStringUtility::FastFree(str);
}

TEST_MEMBER_FUNCTION(TestStringUtility, GetString, signed_long)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("signed long");
    typedef signed long type;
    unsigned int size_of_type = sizeof(type);

    type value = LONG_MIN;
    char* str = TestStringUtility::GetString(value);
    CHECK_ZERO(StrCmp(str, TestStringUtility::GetMinSignedIntAsString(size_of_type)));
    CHECK_EQUAL(StrLen(str), TestStringUtility::GetMinSignedIntCharCount(size_of_type));
    TestStringUtility::FastFree(str);

    value = LONG_MAX;
    str = TestStringUtility::GetString(value);
    CHECK_ZERO(StrCmp(str, TestStringUtility::GetMaxSignedIntAsString(size_of_type)));
    CHECK_EQUAL(StrLen(str), TestStringUtility::GetMaxSignedIntCharCount(size_of_type));
    TestStringUtility::FastFree(str);
}

TEST_MEMBER_FUNCTION(TestStringUtility, GetString, unsigned_long)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("unsigned long");
    typedef unsigned long type;
    unsigned int size_of_type = sizeof(type);

    type value = 0U;
    char* str = TestStringUtility::GetString(value);
    CHECK_ZERO(StrCmp(str, "0"));
    CHECK_EQUAL(StrLen(str), 1U);
    TestStringUtility::FastFree(str);

    value = ULONG_MAX;
    str = TestStringUtility::GetString(value);
    CHECK_ZERO(StrCmp(str, TestStringUtility::GetMaxUnsignedIntAsString(size_of_type)));
    CHECK_EQUAL(StrLen(str), TestStringUtility::GetMaxUnsignedIntCharCount(size_of_type));
    TestStringUtility::FastFree(str);
}

TEST_MEMBER_FUNCTION(TestStringUtility, GetString, signed_char_size_t_ref)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("signed char,size_t&");
    typedef signed char type;
    unsigned int size_of_type = sizeof(type);

    size_t len = 0;
    type value = CHAR_MIN;
    char* str = TestStringUtility::GetString(value, len);
    CHECK_ZERO(StrCmp(str, TestStringUtility::GetMinSignedIntAsString(size_of_type)));
    CHECK_EQUAL(len, TestStringUtility::GetMinSignedIntCharCount(size_of_type));
    TestStringUtility::FastFree(str);

    value = CHAR_MAX;
    str = TestStringUtility::GetString(value, len);
    CHECK_ZERO(StrCmp(str, TestStringUtility::GetMaxSignedIntAsString(size_of_type)));
    CHECK_EQUAL(len, TestStringUtility::GetMaxSignedIntCharCount(size_of_type));
    TestStringUtility::FastFree(str);
}

TEST_MEMBER_FUNCTION(TestStringUtility, GetString, unsigned_char_size_t_ref)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("unsigned char,size_t&");
    typedef unsigned char type;
    unsigned int size_of_type = sizeof(type);

    size_t len = 0;
    type value = 0;
    char* str = TestStringUtility::GetString(value, len);
    CHECK_ZERO(StrCmp(str, "0"));
    CHECK_EQUAL(len, 1U);
    TestStringUtility::FastFree(str);

    value = UCHAR_MAX;
    str = TestStringUtility::GetString(value, len);
    CHECK_ZERO(StrCmp(str, TestStringUtility::GetMaxUnsignedIntAsString(size_of_type)));
    CHECK_EQUAL(len, TestStringUtility::GetMaxUnsignedIntCharCount(size_of_type));
    TestStringUtility::FastFree(str);
}

TEST_MEMBER_FUNCTION(TestStringUtility, GetString, signed_short_size_t_ref)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("signed short,size_t&");
    typedef signed short type;
    unsigned int size_of_type = sizeof(type);

    size_t len = 0;
    type value = SHRT_MIN;
    char* str = TestStringUtility::GetString(value, len);
    CHECK_ZERO(StrCmp(str, TestStringUtility::GetMinSignedIntAsString(size_of_type)));
    CHECK_EQUAL(len, TestStringUtility::GetMinSignedIntCharCount(size_of_type));
    TestStringUtility::FastFree(str);

    value = SHRT_MAX;
    str = TestStringUtility::GetString(value, len);
    CHECK_ZERO(StrCmp(str, TestStringUtility::GetMaxSignedIntAsString(size_of_type)));
    CHECK_EQUAL(len, TestStringUtility::GetMaxSignedIntCharCount(size_of_type));
    TestStringUtility::FastFree(str);
}

TEST_MEMBER_FUNCTION(TestStringUtility, GetString, unsigned_short_size_t_ref)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("unsigned short,size_t&");
    typedef unsigned short type;
    unsigned int size_of_type = sizeof(type);

    size_t len = 0;
    type value = 0;
    char* str = TestStringUtility::GetString(value, len);
    CHECK_ZERO(StrCmp(str, "0"));
    CHECK_EQUAL(len, 1U);
    TestStringUtility::FastFree(str);

    value = USHRT_MAX;
    str = TestStringUtility::GetString(value, len);
    CHECK_ZERO(StrCmp(str, TestStringUtility::GetMaxUnsignedIntAsString(size_of_type)));
    CHECK_EQUAL(len, TestStringUtility::GetMaxUnsignedIntCharCount(size_of_type));
    TestStringUtility::FastFree(str);
}

TEST_MEMBER_FUNCTION(TestStringUtility, GetString, signed_int_size_t_ref)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("signed int,size_t&");
    typedef signed int type;
    unsigned int size_of_type = sizeof(type);

    size_t len = 0;
    type value = INT_MIN;
    char* str = TestStringUtility::GetString(value, len);
    CHECK_ZERO(StrCmp(str, TestStringUtility::GetMinSignedIntAsString(size_of_type)));
    CHECK_EQUAL(len, TestStringUtility::GetMinSignedIntCharCount(size_of_type));
    TestStringUtility::FastFree(str);

    value = INT_MAX;
    str = TestStringUtility::GetString(value, len);
    CHECK_ZERO(StrCmp(str, TestStringUtility::GetMaxSignedIntAsString(size_of_type)));
    CHECK_EQUAL(len, TestStringUtility::GetMaxSignedIntCharCount(size_of_type));
    TestStringUtility::FastFree(str);
}

TEST_MEMBER_FUNCTION(TestStringUtility, GetString, unsigned_int_size_t_ref)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("unsigned int,size_t&");
    typedef unsigned int type;
    unsigned int size_of_type = sizeof(type);

    size_t len = 0;
    type value = 0;
    char* str = TestStringUtility::GetString(value, len);
    CHECK_ZERO(StrCmp(str, "0"));
    CHECK_EQUAL(len, 1U);
    TestStringUtility::FastFree(str);

    value = UINT_MAX;
    str = TestStringUtility::GetString(value, len);
    CHECK_ZERO(StrCmp(str, TestStringUtility::GetMaxUnsignedIntAsString(size_of_type)));
    CHECK_EQUAL(len, TestStringUtility::GetMaxUnsignedIntCharCount(size_of_type));
    TestStringUtility::FastFree(str);
}

TEST_MEMBER_FUNCTION(TestStringUtility, GetString, signed_long_size_t_ref)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("signed long,size_t&");
    typedef signed long type;
    unsigned int size_of_type = sizeof(type);

    size_t len = 0;
    type value = LONG_MIN;
    char* str = TestStringUtility::GetString(value, len);
    CHECK_ZERO(StrCmp(str, TestStringUtility::GetMinSignedIntAsString(size_of_type)));
    CHECK_EQUAL(len, TestStringUtility::GetMinSignedIntCharCount(size_of_type));
    TestStringUtility::FastFree(str);

    value = LONG_MAX;
    str = TestStringUtility::GetString(value, len);
    CHECK_ZERO(StrCmp(str, TestStringUtility::GetMaxSignedIntAsString(size_of_type)));
    CHECK_EQUAL(len, TestStringUtility::GetMaxSignedIntCharCount(size_of_type));
    TestStringUtility::FastFree(str);
}

TEST_MEMBER_FUNCTION(TestStringUtility, GetString, unsigned_long_size_t_ref)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("unsigned long,size_t&");
    typedef unsigned long type;
    unsigned int size_of_type = sizeof(type);

    size_t len = 0;
    type value = 0U;
    char* str = TestStringUtility::GetString(value, len);
    CHECK_ZERO(StrCmp(str, "0"));
    CHECK_EQUAL(len, 1U);
    TestStringUtility::FastFree(str);

    value = ULONG_MAX;
    str = TestStringUtility::GetString(value, len);
    CHECK_ZERO(StrCmp(str, TestStringUtility::GetMaxUnsignedIntAsString(size_of_type)));
    CHECK_EQUAL(len, TestStringUtility::GetMaxUnsignedIntCharCount(size_of_type));
    TestStringUtility::FastFree(str);
}

TEST_MEMBER_FUNCTION(TestStringUtility, UnsafeFill, char_ptr_char_size_t)
{
    using ocl::TestStringUtility;

    char str[10];
    TestStringUtility::UnsafeFill(str, 'a', 1);
    CHECK_ZERO(MemCmp(str, "a", 1));

    TestStringUtility::UnsafeFill(str, 'b', 2);
    CHECK_ZERO(MemCmp(str, "bb", 2));

    TEST_OVERRIDE_ARGS("char*,char,size_t");

}

TEST_MEMBER_FUNCTION(TestStringUtility, SafeLength, char_const_ptr)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("char const*");

    CHECK_ALL_EXCEPTIONS(TestStringUtility::SafeLength(NULL), false);
    CHECK_ZERO(TestStringUtility::SafeLength(NULL));

    CHECK_EQUAL(TestStringUtility::SafeLength("a"), 1U);
}

TEST_MEMBER_FUNCTION(TestStringUtility, UnsafeFind, char_const_ptr_char_size_t_size_t)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("char const*,char,size_t&,size_t");

}

TEST_MEMBER_FUNCTION(TestStringUtility, Allocate, char_ptr_ref_size_t)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("char*&,size_t");

}

TEST_MEMBER_FUNCTION(TestStringUtility, FastFree, char_ptr)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("char*");

}

TEST_MEMBER_FUNCTION(TestStringUtility, SafeFree, char_ptr_ref)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("char*&");

}

TEST_MEMBER_FUNCTION(TestStringUtility, SafeAllocateCopy, char_ptr_ref_size_t_ref_char_const_ptr_size_t)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("char*&,size_t&,char const*,size_t");

}

TEST_MEMBER_FUNCTION(TestStringUtility, SafeAllocateCopy, char_ptr_ref_size_t_ref_char_const_ptr)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("char*&,size_t&,char const*");

}

TEST_MEMBER_FUNCTION(TestStringUtility, UnsafeAllocateCopy, char_ptr_ref_size_t_ref_char_const_ptr)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("char*&,size_t&,char const*");

}

TEST_MEMBER_FUNCTION(TestStringUtility, SafeReallocCopy, char_ptr_ref_size_t_char_const_ptr)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("char*&,size_t&,char const*");

}

TEST_MEMBER_FUNCTION(TestStringUtility, SafeReallocCopy, char_ptr_ref_size_t_ref_char_const_ptr_size_t)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("char*&,size_t&,char const*,size_t");

}

TEST_MEMBER_FUNCTION(TestStringUtility, SafeReallocAppend, char_ptr_ref_size_t_ref_char_const_ptr_size_t_char_const_ptr_size_t)
{
    using ocl::TestStringUtility;

    TEST_OVERRIDE_ARGS("char*&,size_t&,char const*,size_t,char const*,size_t");

}
