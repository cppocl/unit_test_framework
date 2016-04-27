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

#ifndef OCL_GUARD_TEST_TESTMINMAX_HPP
#define OCL_GUARD_TEST_TESTMINMAX_HPP

#include <limits.h>

namespace ocl
{

template<typename Type>
struct TestMinMax;

template<>
struct TestMinMax<signed char>
{
    typedef signed char type;

    static type const min_value = static_cast<type>(CHAR_MIN);
    static type const max_value = static_cast<type>(CHAR_MAX);
};

template<>
struct TestMinMax<unsigned char>
{
    typedef unsigned char type;

    static type const min_value = static_cast<type>(0);
    static type const max_value = static_cast<type>(CHAR_MAX);
};

template<>
struct TestMinMax<signed short>
{
    typedef signed short type;

    static type const min_value = static_cast<type>(SHRT_MIN);
    static type const max_value = static_cast<type>(SHRT_MAX);
};

template<>
struct TestMinMax<unsigned short>
{
    typedef unsigned short type;

    static type const min_value = static_cast<type>(0);
    static type const max_value = static_cast<type>(USHRT_MAX);
};

template<>
struct TestMinMax<signed int>
{
    typedef signed int type;

    static type const min_value = static_cast<type>(INT_MIN);
    static type const max_value = static_cast<type>(INT_MAX);
};

template<>
struct TestMinMax<unsigned int>
{
    typedef unsigned int type;

    static type const min_value = static_cast<type>(0);
    static type const max_value = static_cast<type>(UINT_MAX);
};

template<>
struct TestMinMax<signed long>
{
    typedef signed long type;

    static type const min_value = static_cast<type>(LONG_MIN);
    static type const max_value = static_cast<type>(LONG_MAX);
};

template<>
struct TestMinMax<unsigned long>
{
    typedef unsigned long type;

    static type const min_value = static_cast<type>(0);
    static type const max_value = static_cast<type>(ULONG_MAX);
};

} // namespace ocl

#endif // OCL_GUARD_TEST_TESTMINMAX_HPP
