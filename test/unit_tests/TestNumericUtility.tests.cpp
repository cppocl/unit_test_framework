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
#include "../TestNumericUtility.hpp"

namespace
{

unsigned int GetExpectedIntChars(unsigned int size_of_type)
{
    switch (size_of_type)
    {
        case 1: return 4U;
        case 2: return 6U;
        case 4: return 11U;
        case 8: return 20U;
        default:
            return 0U;
    }
}

}

TEST_MEMBER_FUNCTION(TestNumericUtility, GetMaxNumberOfCharsForInt, NA)
{
    using ocl::TestNumericUtility;

    // expected characters in a converted signed integer type.

    static unsigned int const sizeof_char  = sizeof(signed char);
    static unsigned int const sizeof_short = sizeof(signed short);
    static unsigned int const sizeof_int   = sizeof(signed int);
    static unsigned int const sizeof_long  = sizeof(signed long);

    unsigned int char_count = TestNumericUtility<signed char, unsigned int>::GetMaxNumberOfCharsForInt();
    CHECK_EQUAL(char_count, GetExpectedIntChars(sizeof_char));

    unsigned int short_count = TestNumericUtility<signed short, unsigned int>::GetMaxNumberOfCharsForInt();
    CHECK_EQUAL(short_count, GetExpectedIntChars(sizeof_short));

    unsigned int int_count = TestNumericUtility<signed int, unsigned int>::GetMaxNumberOfCharsForInt();
    CHECK_EQUAL(int_count, GetExpectedIntChars(sizeof_int));

    unsigned int long_count = TestNumericUtility<signed long, unsigned int>::GetMaxNumberOfCharsForInt();
    CHECK_EQUAL(long_count, GetExpectedIntChars(sizeof_long));
}
