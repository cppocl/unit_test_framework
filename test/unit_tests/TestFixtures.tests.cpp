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

namespace
{
    class LocalFixtureTestClass
    {
    public:
        LocalFixtureTestClass() : m_value(0)
        {
        }

        int GetValue() const
        {
            return m_value;
        }

        void SetValue(int value)
        {
            m_value = value;
        }

    private:
        int m_value;
    };

    LocalFixtureTestClass g_local_test_class;
}

// Extra setup and teardown for verifying that multiple fixtures
// access the correct classes.
TEST_SETUP(LocalFixtureTestClass)
{
    g_local_test_class.SetValue(2);
}

TEST_TEARDOWN(LocalFixtureTestClass)
{
    g_local_test_class.SetValue(-2);
}

TEST_MEMBER_FUNCTION(LocalFixtureTestClass, GetValue, NA)
{
    LocalFixtureTestClass local_test_class;
    CHECK_EQUAL(g_local_test_class.GetValue(), 2);
    CHECK_EQUAL(local_test_class.GetValue(), 0);
    local_test_class = g_local_test_class;
    CHECK_EQUAL(local_test_class.GetValue(), 2);
}

TEST_MEMBER_FUNCTION(LocalFixtureTestClass, SetValue, int)
{
    LocalFixtureTestClass local_test_class;
    CHECK_EQUAL(g_local_test_class.GetValue(), 2);
    CHECK_EQUAL(local_test_class.GetValue(), 0);
    local_test_class.SetValue(4);
    CHECK_EQUAL(local_test_class.GetValue(), 4);
}
