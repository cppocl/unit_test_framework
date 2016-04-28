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
#include "../TestMinMax.hpp"
#include <limits.h>

TEST(TestMinMax_class_constants)
{
    using ocl::TestMinMax;

    CHECK_EQUAL(TestMinMax<signed char>::min_value, CHAR_MIN);
    CHECK_EQUAL(TestMinMax<signed char>::max_value, CHAR_MAX);

    CHECK_EQUAL(TestMinMax<unsigned char>::min_value, 0U);
    CHECK_EQUAL(TestMinMax<unsigned char>::max_value, UCHAR_MAX);

    CHECK_EQUAL(TestMinMax<signed short>::min_value, SHRT_MIN);
    CHECK_EQUAL(TestMinMax<signed short>::max_value, SHRT_MAX);

    CHECK_EQUAL(TestMinMax<unsigned short>::min_value, 0U);
    CHECK_EQUAL(TestMinMax<unsigned short>::max_value, USHRT_MAX);

    CHECK_EQUAL(TestMinMax<signed int>::min_value, INT_MIN);
    CHECK_EQUAL(TestMinMax<signed int>::max_value, INT_MAX);

    CHECK_EQUAL(TestMinMax<unsigned int>::min_value, 0U);
    CHECK_EQUAL(TestMinMax<unsigned int>::max_value, UINT_MAX);

    CHECK_EQUAL(TestMinMax<signed long>::min_value, LONG_MIN);
    CHECK_EQUAL(TestMinMax<signed long>::max_value, LONG_MAX);

    CHECK_EQUAL(TestMinMax<unsigned long>::min_value, 0U);
    CHECK_EQUAL(TestMinMax<unsigned long>::max_value, ULONG_MAX);
}
