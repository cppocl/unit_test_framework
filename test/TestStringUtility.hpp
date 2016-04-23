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

#ifndef OCL_GUARD_TEST_TESTSTRINGUTILITY_HPP
#define OCL_GUARD_TEST_TESTSTRINGUTILITY_HPP

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include "TestMemoryUtility.hpp"
#include "TestConverterUtility.hpp"

namespace ocl
{

/*
    String utility class providing features such as:
        allocation, de-allocation, filling, copying, appending 

    Functions that don't do pointer checking start with Unsafe in the name.
*/

struct TestStringUtility
{
    // NOTE: size_type is not used as it causes
    // possible loss of data warnings for some compilers.
    typedef unsigned int size_type;

    template<typename IntType>
    static char const* GetMinSignedIntAsString(IntType size_of_int) throw()
    {
        switch (size_of_int)
        {
            case 1: return "-128";
            case 2: return "-32768";
            case 4: return "-2147483648";
            case 8: return "-9223372036854775808";
        }

        return "";
    }

    template<typename IntType>
    static char const* GetMaxSignedIntAsString(IntType size_of_int) throw()
    {
        switch (size_of_int)
        {
            case 1: return "127";
            case 2: return "32767";
            case 4: return "2147483647";
            case 8: return "9223372036854775807";
        }

        return "";
    }

    template<typename IntType>
    static char const* GetMaxUnsignedIntAsString(IntType size_of_int) throw()
    {
        switch (size_of_int)
        {
            case 1: return "255";
            case 2: return "65535";
            case 4: return "4294967295";
            case 8: return "18446744073709551615";
        }

        return "";
    }

    template<typename IntType>
    static size_type GetMinSignedIntCharCount(IntType size_of_int) throw()
    {
        switch (size_of_int)
        {
            case 1: return 4U;
            case 2: return 6U;
            case 4: return 11U;
            case 8: return 20U;
        }

        return 0U;
    }

    template<typename IntType>
    static size_type GetMaxSignedIntCharCount(IntType size_of_int) throw()
    {
        switch (size_of_int)
        {
            case 1: return 3U;
            case 2: return 5U;
            case 4: return 10U;
            case 8: return 19U;
        }

        return 0U;
    }

    template<typename IntType>
    static size_type GetMaxUnsignedIntCharCount(IntType size_of_int) throw()
    {
        switch (size_of_int)
        {
            case 1: return 3U;
            case 2: return 5U;
            case 4: return 10U;
            case 8: return 20U;
        }

        return 0U;
    }

    template<typename T>
    static char* GetString(T value)
    {
        size_type len = 0U;
        return TestConverterUtility<T>::GetString(value, len);
    }

    template<typename Type, typename SizeType>
    static char* GetString(Type value, SizeType& length)
    {
        return TestConverterUtility<Type, SizeType>::GetString(value, length);
    }

    static void UnsafeFill(char* str, char ch, size_type len)
    {
        char* end = str + len;
        for (; str < end; ++str)
            *str = ch;
        *str = '\0';
    }

    static size_type SafeLength(char const* str)
    {
        size_type len = str != NULL ? static_cast<size_type>(::strlen(str)) : 0U;
        return len;
    }

    static bool UnsafeFind(char const* str, char ch, size_type& pos, size_type start = 0)
    {
        char const* pch = str + start;
        for (; (*pch != '\0') && (*pch != ch); ++pch)
            ;
        if ((ch != '\0') && (*pch == '\0'))
            return false;
        pos = static_cast<size_type>(pch - str);
        return true;
    }

    static void Allocate(char*& dest, size_type dest_len)
    {
        dest = TestMemoryUtility<char>::Allocate(dest_len + 1);
    }

    static void FastFree(char* str)
    {
        TestMemoryUtility<char>::FastFree(str);
    }

    static void SafeFree(char*& str)
    {
        TestMemoryUtility<char>::SafeFree(str);
    }

    static void SafeAllocateCopy(char*& dest,
                                 size_type& dest_len,
                                 char const* src,
                                 size_type src_len)
    {
        if ((src != NULL) && (src_len > 0))
        {
            dest = TestMemoryUtility<char>::UnsafeAllocateCopy(src, src_len + 1);
            if (dest != NULL)
            {
                dest_len = src_len;

                // If src is longer than src_len, add the terminating '\0'.
                if (*(src + src_len) != '\0')
                    *(dest + dest_len) = '\0';
            }
            else
                dest_len = static_cast<size_type>(0);
        }
        else
        {
            dest = NULL;
            dest_len = 0;
        }
    }

    static void SafeAllocateCopy(char*& dest, size_type& dest_len, char const* src)
    {
        if (src == NULL)
        {
            dest = NULL;
            dest_len = 0;
        }
        else
            UnsafeAllocateCopy(dest, dest_len, src);
    }

    static void UnsafeAllocateCopy(char*& dest, size_type& dest_len, char const* src)
    {
        size_type src_len = static_cast<size_type>(::strlen(src));
        dest = TestMemoryUtility<char>::UnsafeAllocateCopy(src, src_len + 1);
        dest_len = (dest != NULL) ? src_len : static_cast<size_type>(0);
    }

    static void SafeReallocCopy(char*& dest,
                                size_type& dest_len,
                                char const* src,
                                size_type src_len)
    {
        FastFree(dest);
        SafeAllocateCopy(dest, dest_len, src, src_len);
    }

    static void SafeReallocCopy(char*& dest, size_type& dest_len, char const* src)
    {
        FastFree(dest);
        SafeAllocateCopy(dest, dest_len, src);
    }

    /// Allocate enough space in dest to copy str1 and append str2.
    /// @note It is safe for dest and str1 or str2 to be the same string.
    /// Also str1 or str2 can be NULL.
    static void SafeAllocAppend(char*& dest,
                                size_type& dest_len,
                                char const* str1,
                                size_type str1_len,
                                char const* str2,
                                size_type str2_len)
    {
        size_type new_len = str1_len + str2_len;
        if (new_len > 0)
        {
            char* new_dest = NULL;
            Allocate(new_dest, new_len);
            if (new_dest != NULL)
            {
                if ((str1 != NULL) && (str1_len > 0))
                    ::memcpy(new_dest, str1, str1_len);
                else
                {
                    new_len -= str1_len;
                    str1_len = 0;
                }
                if ((str2 != NULL) && (str2_len > 0))
                    ::memcpy(new_dest + str1_len, str2, str2_len);
                else
                    new_len -= str2_len;
                *(new_dest + new_len) = '\0';
                dest_len = new_len;
            }
            else
                dest_len = 0;
            FastFree(dest);
            dest = new_dest;
        }
        else
        {
            SafeFree(dest);
            dest_len = 0;
        }
    }

    /// Same as AllocAppend, except dest is first freed
    /// before being set with str1 and str2.
    static void SafeReallocAppend(char*& dest,
                                  size_type& dest_len,
                                  char const* str1,
                                  size_type str1_len,
                                  char const* str2,
                                  size_type str2_len)
    {
        char* new_dest = NULL;
        size_type new_dest_len = 0;
        SafeAllocAppend(new_dest, new_dest_len, str1, str1_len, str2, str2_len);
        FastFree(dest);
        dest = new_dest;
        dest_len = new_dest_len;
    }
};

} // namespace ocl

#endif // OCL_GUARD_TEST_TESTSTRINGUTILITY_HPP
