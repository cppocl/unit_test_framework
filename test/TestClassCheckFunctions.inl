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

    void CheckTrue(TestString const& expression,
                   TestString const& filename,
                   size_type line_number,
                   bool value)
    {
        LogCheck(expression, filename, line_number, !value);
    }

    void CheckFalse(TestString const& expression,
                    TestString const& filename,
                    size_type line_number,
                    bool value)
    {
        LogCheck(expression, filename, line_number, value);
    }

    void CheckNull(TestString const& expression,
                   TestString const& filename,
                   size_type line_number,
                   void const* ptr)
    {
        LogCheck(expression, filename, line_number, ptr != NULL);
    }

    void CheckNotNull(TestString const& expression,
                      TestString const& filename,
                      size_type line_number,
                      void const* ptr)
    {
        LogCheck(expression, filename, line_number, ptr == NULL);
    }

    void CheckException(TestString const& expression,
                        TestString const& filename,
                        size_type line_number,
                        bool found_exception,
                        bool expect_exception)
    {
        bool error = (expect_exception && !found_exception) || (!expect_exception && found_exception);
        LogCheck(expression, filename, line_number, error);
    }

    template<typename CharType>
    void CheckStrCmp(TestString const& expression,
                    TestString const& filename,
                    size_type line_number,
                    CharType const* str1,
                    CharType const* str2)
    {
        bool error = StrCmp(str1, str2) != 0;
        LogCheck(expression, filename, line_number, error);
    }

    template<typename CharType>
    void CheckNotStrCmp(TestString const& expression,
                    TestString const& filename,
                    size_type line_number,
                    CharType const* str1,
                    CharType const* str2)
    {
        bool error = StrCmp(str1, str2) == 0;
        LogCheck(expression, filename, line_number, error);
    }

    template<typename T1, typename T2>
    void CheckEqual(TestString const& expression,
                    TestString const& filename,
                    size_type line_number,
                    T1 const& value1,
                    T2 const& value2)
    {
        bool is_equal = value1 == value2;
        LogCheck(expression, filename, line_number, !is_equal);
    }

    template<typename T1, typename T2>
    void CheckNotEqual(TestString const& expression,
                       TestString const& filename,
                       size_type line_number,
                       T1 const& value1,
                       T2 const& value2)
    {
        bool is_not_equal = value1 != value2;
        LogCheck(expression, filename, line_number, !is_not_equal);
    }

    template<typename T1, typename T2>
    void CheckGreater(TestString const& expression,
                      TestString const& filename,
                      size_type line_number,
                      T1 const& value1,
                      T2 const& value2)
    {
        bool is_greater = value1 > value2;
        LogCheck(expression, filename, line_number, !is_greater);
    }

    template<typename T1, typename T2>
    void CheckGreaterEqual(TestString const& expression,
                           TestString const& filename,
                           size_type line_number,
                           T1 const& value1,
                           T2 const& value2)
    {
        bool is_greater_equal = value1>= value2;
        LogCheck(expression, filename, line_number, !is_greater_equal);
    }

    template<typename T1, typename T2>
    void CheckLess(TestString const& expression,
                   TestString const& filename,
                   size_type line_number,
                   T1 const& value1,
                   T2 const& value2)
    {
        bool is_less = value1 < value2;
        LogCheck(expression, filename, line_number, !is_less);
    }

    template<typename T1, typename T2>
    void CheckLessEqual(TestString const& expression,
                        TestString const& filename,
                        size_type line_number,
                        T1 const& value1,
                        T2 const& value2)
    {
        bool is_less_equal = value1 <= value2;
        LogCheck(expression, filename, line_number, !is_less_equal);
    }

    template<typename T>
    void CheckZero(TestString const& expression,
                   TestString const& filename,
                   size_type line_number,
                   T value)
    {
        bool is_equal = value == static_cast<T>(0);
        LogCheck(expression, filename, line_number, !is_equal);
    }

    template<typename T>
    void CheckNotZero(TestString const& expression,
                      TestString const& filename,
                      size_type line_number,
                      T value)
    {
        bool is_not_equal = value != static_cast<T>(0);
        LogCheck(expression, filename, line_number, !is_not_equal);
    }

    /// Compare contents of char*, wchar_t* or primitive values.
    template<typename T1, typename T2>
    void CheckCompare(TestString const& expression,
                      TestString const& filename,
                      size_type line_number,
                      T1 const& value1,
                      T2 const& value2)
    {
        int compare = TestCompare<T1, T2>::Compare(value1, value2);
        LogCheck(expression, filename, line_number, compare == 0);
    }

    /// Compare contents of char*, wchar_t* or primitive values.
    template<typename T1, typename T2>
    void CheckNotCompare(TestString const& expression,
                         TestString const& filename,
                         size_type line_number,
                         T1 const& value1,
                         T2 const& value2)
    {
        int compare = TestCompare<T1, T2>::Compare(value1, value2);
        LogCheck(expression, filename, line_number, compare != 0);
    }

    /// While the current time has not reached the start time + sample time,
    /// keep returning false.
    /// @note This function also refreshes the current time.
    bool CheckTime()
    {
        TestTime stop_time(m_start_time);
        stop_time += m_sample_time;

        m_current_time.Refresh();

        ++m_timed_function_calls;

        return m_current_time > stop_time;
    }

    /// While the current time has not reached the start time + sample time,
    /// keep returning false, or return false when the performance has been
    /// detected as too slow.
    /// @note This function also refreshes the current time.
    bool CheckTime(size_type min_iterations, TestString const& filename, size_type line_number)
    {
        TestTime stop_time(m_start_time);
        stop_time += m_sample_time;

        m_current_time.Refresh();

        ++m_timed_function_calls;

        bool time_complete = m_current_time > stop_time;

        if (time_complete && (m_timed_function_calls < min_iterations))
        {
            ++m_failure_check_count;
            TestString error;
            error.Append(m_timed_function_calls);
            error.Append(" iterations less than expected ");
            error.Append(min_iterations);
            error.Append(" iterations");
            privateRecordFailed(error, filename, line_number);
            return true;
        }

        return time_complete;
    }

