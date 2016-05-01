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

#ifndef OCL_GUARD_TEST_TESTCONVERTERUTILITY_HPP
#define OCL_GUARD_TEST_TESTCONVERTERUTILITY_HPP

#include "TestMemoryUtility.hpp"
#include "TestNumericUtility.hpp"
#include <cstring>
#include <cstddef>

namespace ocl
{

/*
 * Helper class for converting primitive types to char* which can then be output.
 */

template<typename ValueType, typename SizeType = unsigned int>
struct TestConverterUtility
{
    typedef SizeType size_type;

    static char* GetString(ValueType, SizeType& length) { length = 0; return NULL; }
};

template<typename SizeType>
struct TestConverterUtility<bool, SizeType>
{
    typedef SizeType size_type;
    typedef bool type;

    /// bool will be returned as a string of "true" or "false".
    static char* GetString(type value, SizeType& length)
    {
        char const* str = value ? "true" : "false";
        length = static_cast<SizeType>(::strlen(str));
        return TestMemoryUtility<char, SizeType>::UnsafeAllocateCopy(str, length + 1);
    }
};

template<typename SizeType>
struct TestConverterUtility<char, SizeType>
{
    typedef SizeType size_type;
    typedef char type;

    static char* GetString(type value, SizeType& length)
    {
        char str[2] = {value, '\0'};
        length = 1;
        return TestMemoryUtility<char, SizeType>::UnsafeAllocateCopy(str, 2);
    }
};

template<typename SizeType>
struct TestConverterUtility<signed char, SizeType>
{
    typedef SizeType size_type;
    typedef signed char type;

    static char* GetString(type value, SizeType& length)
    {
        return TestNumericUtility<signed int, SizeType>::GetString(value, "%d", length);
    }
};

template<typename SizeType>
struct TestConverterUtility<unsigned char, SizeType>
{
    typedef SizeType size_type;
    typedef unsigned short type;

    static char* GetString(type value, SizeType& length)
    {
        return TestNumericUtility<unsigned int, SizeType>::GetString(value, "%u", length);
    }
};

template<typename SizeType>
struct TestConverterUtility<signed short, SizeType>
{
    typedef SizeType size_type;
    typedef signed short type;

    static char* GetString(type value, SizeType& length)
    {
        return TestNumericUtility<signed int, SizeType>::GetString(value, "%d", length);
    }
};

template<typename SizeType>
struct TestConverterUtility<unsigned short, SizeType>
{
    typedef SizeType size_type;
    typedef unsigned short type;

    static char* GetString(type value, SizeType& length)
    {
        return TestNumericUtility<unsigned int, SizeType>::GetString(value, "%u", length);
    }
};

template<typename SizeType>
struct TestConverterUtility<signed int, SizeType>
{
    typedef SizeType size_type;
    typedef signed int type;

    static char* GetString(type value, SizeType& length)
    {
        return TestNumericUtility<type, SizeType>::GetString(value, "%d", length);
    }
};

template<typename SizeType>
struct TestConverterUtility<unsigned int, SizeType>
{
    typedef SizeType size_type;
    typedef unsigned int type;

    static char* GetString(type value, SizeType& length)
    {
        return TestNumericUtility<type, SizeType>::GetString(value, "%u", length);
    }
};

template<typename SizeType>
struct TestConverterUtility<signed long, SizeType>
{
    typedef SizeType size_type;
    typedef signed long type;

    static char* GetString(type value, SizeType& length)
    {
        return TestNumericUtility<type, SizeType>::GetString(value, "%ld", length);
    }
};

template<typename SizeType>
struct TestConverterUtility<unsigned long, SizeType>
{
    typedef SizeType size_type;
    typedef unsigned long type;

    static char* GetString(type value, SizeType& length)
    {
        return TestNumericUtility<type, SizeType>::GetString(value, "%lu", length);
    }
};

} // namespace ocl

#endif // OCL_GUARD_TEST_TESTCONVERTERUTILITY_HPP
