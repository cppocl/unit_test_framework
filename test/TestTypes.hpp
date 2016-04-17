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

#ifndef OCL_GUARD_TEST_TESTTYPES_HPP
#define OCL_GUARD_TEST_TESTTYPES_HPP

#if !defined(_MSC_VER)
// Want to use uint64_t for all non-Winsows platforms
#include <stdint.h>
#endif

namespace ocl
{
#if defined(_MSC_VER)
    // For simplicity, always use Microsoft in-built 64-bit type,
    // as various Microsoft versions have different support.
    typedef signed __int64   ocl_int64;
    typedef unsigned __int64 ocl_uint64;
#else
    typedef int64_t  ocl_int64;
    typedef uint64_t ocl_uint64;
#endif
}

#endif // OCL_GUARD_TEST_TESTTYPES_HPP
