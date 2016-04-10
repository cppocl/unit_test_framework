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
#include "../TestTime.hpp"

#if 1

TEST_MEMBER_FUNCTION(TestTime, TestTime, bool)
{
    using ocl::TestTime;

    {
        TestTime tt;
        bool is_set = tt.IsSet();
        CHECK_FALSE(is_set);
    }

    {
        TestTime tt(false);
        bool is_set = tt.IsSet();
        CHECK_FALSE(is_set);
    }

    {
        TestTime tt(true);
        bool is_set = tt.IsSet();
        CHECK_TRUE(is_set);
    }
}

TEST_MEMBER_FUNCTION(TestTime, TestTime, unsigned_long_unsigned_long)
{
    TEST_OVERRIDE_ARGS("unsigned long,unsigned long");

    using ocl::TestTime;

    {
        TestTime tt(0, 0);
        CHECK_FALSE(tt.IsSet());
    }

    {
        TestTime tt(1, 0);
        CHECK_TRUE(tt.IsSet());
    }

    {
        TestTime tt(0, 1);
        CHECK_TRUE(tt.IsSet());
    }

    {
        TestTime tt(1, 1);
        CHECK_TRUE(tt.IsSet());
    }
}

TEST_CONST_MEMBER_FUNCTION(TestTime, operator_equal, TestTime_const_ref)
{
    TEST_OVERRIDE_FUNCTION_NAME_ARGS("operator ==", "TestTime const&");

    using ocl::TestTime;

    TestTime tt1(0, 0);
    TestTime tt2(0, 0);

    CHECK_EQUAL(tt1, tt2);

    tt1 += TestTime(0, 1);
    CHECK_FALSE(tt1 == tt2);

    tt2 += TestTime(0, 1);

    CHECK_EQUAL(tt1, tt2);
}

TEST_CONST_MEMBER_FUNCTION(TestTime, operator_not_equal, TestTime_const_ref)
{
    TEST_OVERRIDE_FUNCTION_NAME_ARGS("operator !=", "TestTime const&");

    using ocl::TestTime;

    TestTime tt1(0, 0);
    TestTime tt2(0, 0);

    CHECK_FALSE(tt1 != tt2);

    tt1 += TestTime(0, 1);
    CHECK_NOT_EQUAL(tt1, tt2);

    tt2 += TestTime(0, 1);
    CHECK_FALSE(tt1 != tt2);
}

TEST_CONST_MEMBER_FUNCTION(TestTime, operator_less, TestTime_const_ref)
{
    TEST_OVERRIDE_FUNCTION_NAME_ARGS("operator <", "TestTime const&");

    using ocl::TestTime;

    TestTime tt1(0, 0);
    TestTime tt2(0, 0);
    tt2 += TestTime(0, 1);

    CHECK_LESS(tt1, tt2);

    tt1 += TestTime(0, 1);
    CHECK_FALSE(tt1 < tt2);

    tt1 += TestTime(0, 1);
    CHECK_FALSE(tt1 < tt2);
}

TEST_CONST_MEMBER_FUNCTION(TestTime, operator_less_equal, TestTime_const_ref)
{
    TEST_OVERRIDE_FUNCTION_NAME_ARGS("operator <=", "TestTime const&");

    using ocl::TestTime;

    TestTime tt1(0, 0);
    TestTime tt2(0, 0);
    tt2 += TestTime(0, 1);

    CHECK_LESS_EQUAL(tt1, tt2);

    tt1 += TestTime(0, 1);
    CHECK_LESS_EQUAL(tt1, tt2);

    tt1 += TestTime(0, 1);
    CHECK_FALSE(tt1 <= tt2);
}

TEST_CONST_MEMBER_FUNCTION(TestTime, operator_more, TestTime_const_ref)
{
    TEST_OVERRIDE_FUNCTION_NAME_ARGS("operator >", "TestTime const&");

    using ocl::TestTime;

    TestTime tt1(0, 0);
    TestTime tt2(0, 0);
    tt1 += TestTime(0, 1);

    CHECK_GREATER(tt1, tt2);

    tt2 += TestTime(0, 1);
    CHECK_FALSE(tt1 > tt2);

    tt2 += TestTime(0, 1);
    CHECK_FALSE(tt1 > tt2);
}

TEST_MEMBER_FUNCTION(TestTime, operator_plus_eq, TestTime_const_ref)
{
    TEST_OVERRIDE_FUNCTION_NAME_ARGS("operator +=", "TestTime const&");

    using ocl::TestTime;

    TestTime tt(0, 0);
    tt += TestTime(0, 1);

    CHECK_EQUAL(tt.GetNanoseconds(), 1U);

    tt += TestTime(1, 0);
    CHECK_EQUAL(tt.GetSeconds(), 1U);
    CHECK_EQUAL(tt.GetNanoseconds(), 1U);

    tt += TestTime(1, 1);
    CHECK_EQUAL(tt.GetSeconds(), 2U);
    CHECK_EQUAL(tt.GetNanoseconds(), 2U);
}

TEST_MEMBER_FUNCTION(TestTime, operator_div_eq, TestTime_const_ref)
{
    TEST_OVERRIDE_FUNCTION_NAME_ARGS("operator /=", "TestTime const&");

    using ocl::TestTime;

    TestTime tt(0, 0);
    tt += TestTime(2, 0);

    tt /= 2;
    CHECK_EQUAL(tt.GetSeconds(), 1U);

    tt /= 2;
    CHECK_EQUAL(tt.GetSeconds(), 0U);
    CHECK_EQUAL(tt.GetMilliseconds(), 500U);
}

#endif

TEST_MEMBER_FUNCTION(TestTime, GetDiffTime, TestTime_const_ref_TestTime_ref)
{
    TEST_OVERRIDE_ARGS("TestTime const&, TestTime&");

    using ocl::TestTime;

    TestTime tm1(1, 0);
    TestTime tm1b(1, 0);
    TestTime tm2(2, 0);
    TestTime tm3(1, TestTime::MillisecondsToNanoseconds(500));
    TestTime tm4(2, TestTime::MillisecondsToNanoseconds(500));

    TestTime diff;

    diff.SetSeconds(10);
    diff.SetMilliseconds(10);
    tm1.GetDiffTime(tm2, diff);
    unsigned long seconds = diff.GetSeconds();
    unsigned long milliseconds = diff.GetMilliseconds();
    unsigned long nanoseconds = diff.GetNanoseconds();
    CHECK_EQUAL(seconds, 1U);
    CHECK_EQUAL(milliseconds, 0U);
    CHECK_EQUAL(nanoseconds, 0U);

    diff.SetSeconds(10);
    diff.SetMilliseconds(10);
    tm2.GetDiffTime(tm1, diff);
    seconds = diff.GetSeconds();
    milliseconds = diff.GetMilliseconds();
    nanoseconds = diff.GetNanoseconds();
    CHECK_EQUAL(seconds, 1U);
    CHECK_EQUAL(nanoseconds, 0U);
    CHECK_EQUAL(milliseconds, 0U);

    diff.SetSeconds(10);
    diff.SetMilliseconds(10);
    tm1.GetDiffTime(tm3, diff);
    seconds = diff.GetSeconds();
    milliseconds = diff.GetMilliseconds();
    nanoseconds = diff.GetNanoseconds();
    CHECK_EQUAL(seconds, 0U);
    CHECK_EQUAL(nanoseconds, TestTime::MillisecondsToNanoseconds(500));
    CHECK_EQUAL(milliseconds, 500U);

    diff.SetSeconds(10);
    diff.SetMilliseconds(10);
    tm3.GetDiffTime(tm1, diff);
    seconds = diff.GetSeconds();
    milliseconds = diff.GetMilliseconds();
    nanoseconds = diff.GetNanoseconds();
    CHECK_EQUAL(seconds, 0U);
    CHECK_EQUAL(nanoseconds, TestTime::MillisecondsToNanoseconds(500));
    CHECK_EQUAL(milliseconds, 500U);

    diff.SetSeconds(10);
    diff.SetMilliseconds(10);
    tm1.GetDiffTime(tm4, diff);
    seconds = diff.GetSeconds();
    milliseconds = diff.GetMilliseconds();
    nanoseconds = diff.GetNanoseconds();
    CHECK_EQUAL(seconds, 1U);
    CHECK_EQUAL(nanoseconds, TestTime::MillisecondsToNanoseconds(500));
    CHECK_EQUAL(milliseconds, 500U);

    diff.SetSeconds(10);
    diff.SetMilliseconds(10);
    tm4.GetDiffTime(tm1, diff);
    seconds = diff.GetSeconds();
    milliseconds = diff.GetMilliseconds();
    nanoseconds = diff.GetNanoseconds();
    CHECK_EQUAL(seconds, 1U);
    CHECK_EQUAL(nanoseconds, TestTime::MillisecondsToNanoseconds(500));
    CHECK_EQUAL(milliseconds, 500U);

    diff.SetSeconds(10);
    diff.SetMilliseconds(10);
    tm1.GetDiffTime(tm1b, diff);
    seconds = diff.GetSeconds();
    milliseconds = diff.GetMilliseconds();
    nanoseconds = diff.GetNanoseconds();
    CHECK_EQUAL(seconds, 0U);
    CHECK_EQUAL(nanoseconds, TestTime::MillisecondsToNanoseconds(0));
    CHECK_EQUAL(milliseconds, 0U);

    diff.SetSeconds(10);
    diff.SetMilliseconds(10);
    tm1b.GetDiffTime(tm1, diff);
    seconds = diff.GetSeconds();
    milliseconds = diff.GetMilliseconds();
    nanoseconds = diff.GetNanoseconds();
    CHECK_EQUAL(seconds, 0U);
    CHECK_EQUAL(nanoseconds, TestTime::MillisecondsToNanoseconds(0));
    CHECK_EQUAL(milliseconds, 0U);
}
