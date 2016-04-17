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

#ifndef OCL_GUARD_TEST_TESTCLASS_HPP
#define OCL_GUARD_TEST_TESTCLASS_HPP

#include "TestLog.hpp"
#include "TestTime.hpp"
#include "TestString.hpp"
#include "TestCompare.hpp"
#include "TestTypeInfo.hpp"
#include "TestClassSharedData.hpp"

#include <cstring>
#include <climits>

namespace ocl
{

class TestClass
{
public:
    typedef TestString::size_type size_type;

public:
    TestClass(TestString const& class_name,
              TestString const& function_name,
              TestString const& args,
              bool is_const = false,
              bool is_timed = false,
              unsigned long secs = 0,
              unsigned long nanosecs = 0)
        : m_logger(NULL)
        , m_class_name(class_name)
        , m_function_name(function_name)
        , m_args(args == "NA" ? "" : args)
        , m_is_const(is_const)
        , m_is_timed(is_timed)
        , m_start_time(is_timed)
        , m_current_time(m_start_time)
        , m_timed_function_calls(0)
        , m_sample_time(secs, nanosecs)
        , m_check_count(0)
        , m_failure_check_count(0)
        , m_previous_member_function_length(0)
        , m_test_number(0)
        , m_recorded(true)
    {
        if (is_timed && !m_sample_time.IsSet())
            m_sample_time.SetSeconds(1);

        privateConstruct();
    }

    virtual ~TestClass()
    {
        if (m_recorded)
        {
            if (GetSharedData().m_constructions > 0)
                --GetSharedData().m_constructions;
            else if (GetSharedData().m_constructions == 0)
                LogWriteLine("*** Error with destruction ***");

            privateLogFunction();

            if (IsLast())
                privateLogSummary();
        }
    }

// Functions for managing the function or member function test.
public:
    TestString const& GetClassName() const throw()
    {
        return m_class_name;
    }

    void SetClassName(TestString const& class_name)
    {
        m_class_name = class_name;
        privateUpdateMaxFunctionLength();
    }

    TestString const& GetFunctionName() const throw()
    {
        return m_function_name;
    }

    void SetFunctionName(TestString const& function_name)
    {
        m_function_name = function_name;
        privateUpdateMaxFunctionLength();
    }

    TestString const& GetArgs() const throw()
    {
        return m_args;
    }

    void SetArgs(TestString const& args)
    {
        m_args = args;
        privateUpdateMaxFunctionLength();
    }

    /// return string as class name::function name([args]) [const]
    /// , function name([args]) or general test name.
    void GetTestName(TestString& str) const
    {
        TestString const& function_name = GetFunctionName();

        str = GetClassName();

        // If function_name is empty then this is a general test.
        if (!function_name.IsEmpty())
        {
            if (!str.IsEmpty())
                str += "::";
            str += function_name;
            str += "(";
            str += GetArgs();
            str += ")";
            if (m_is_const)
                str += " const";
        }
    }

// General helper functions.
public:
    /// Underlying function for TEST_FAILURE_INDENT macro.
    void SetFailureIndent(size_type indent)
    {
        m_recorded = false;
        GetSharedData().m_failure_indent = indent;

        // Don't want to count constructors for the indentation object,
        // as this is not needed in the unit test statistics.
        --GetSharedData().m_constructions;
        --GetSharedData().m_total_tests;
    }

    /// Get status for tests and failures.
    bool HasTests() const throw()
    {
        return m_check_count > 0;
    }

    bool HasFailed() const throw()
    {
        return m_failure_check_count > 0;
    }

    bool IsGeneralTest() const throw()
    {
        return GetFunctionName().IsEmpty();
    }

    bool IsLast() const throw()
    {
        return GetSharedData().m_constructions == 0;
    }

    void LogWrite(TestString const& msg)
    {
        if (m_logger != NULL)
            m_logger->Write(msg);
    }

    void LogWriteLine(TestString const& msg)
    {
        if (m_logger != NULL)
            m_logger->WriteLine(msg);
    }

    TestTime const& GetStartTime() const throw()
    {
        return m_start_time;
    }

    TestTime const& GetSampleTime() const throw()
    {
        return m_sample_time;
    }

    TestTime const& GetCurrentTime() const throw()
    {
        return m_current_time;
    }

// Check functions
#include "TestClassCheckFunctions.inl"

// Unit test helper functions
#include "TestClassHelperFunctions.inl"

// Shared data helper functions
#include "TestClassSharedFunctions.inl"

protected:
    // TEST_FUNCTION and TEST_MEMBER_FUNCTION create their own constructor,
    // so access to the TestClass constructor does not require direct access.
    TestClass()
        : m_logger(NULL)
        , m_is_const(false)
        , m_is_timed(false)
        , m_start_time(false)
        , m_current_time(false)
        , m_timed_function_calls(0)
        , m_sample_time(false)
        , m_check_count(0)
        , m_failure_check_count(0)
        , m_previous_member_function_length(0)
        , m_test_number(0)
        , m_recorded(true)
    {
        // NOTE: This is only called for setting up indent for failure messages.
        privateConstruct();
    }

    // Log check function used by all check macros.
    void LogCheck(TestString const& expression,
                  TestString const& filename,
                  size_type line_number,
                  bool failed)
    {
        privateSetFilename(filename);

        if (failed)
        {
            ++m_failure_check_count;
            privateRecordFailed(expression, filename, line_number);
        }

        ++m_check_count;
        ++GetSharedData().m_total_checks;
    }

private:
    // Log the whole line for a function or member function.
    void privateLogFunction()
    {
        privateLogFunctionLineNumber();

        privateLogTestStatus();

        // Output function ot memeber function name.
        LogWrite(m_member_function);

        // Output summary after function name.
        if (m_is_timed)
            privateLogTime();
        else
            privateLogNumberOfRunTests();

        // Output any error lines and expressions.
        if (HasFailed())
            privateLogFailures();
    }

    // Log the current tested function with a line number.
    void privateLogFunctionLineNumber()
    {
        TestString line_number_str;
        size_type total_tests = static_cast<size_type>(GetSharedData().m_total_tests);
        size_type pad_size = TestNumericUtility<size_type, size_type>::GetNumberOfCharsForInt(total_tests);
        line_number_str.Append("(");
        ++GetSharedData().m_logged_line;
        line_number_str.Append(static_cast<size_type>(GetSharedData().m_logged_line), pad_size);
        line_number_str.Append(") ");
        LogWrite(line_number_str);
    }

    // Output SUCCESS, FAILED, NOT RUN or TIMED message.
    void privateLogTestStatus()
    {
        // Output the first part of the log line.
        if (HasTests())
        {
            if (!IsGeneralTest())
                ++GetSharedData().m_total_functions_tested;
            if (HasFailed())
                LogWrite(GetSharedData().m_failed_message);
            else
                LogWrite(GetSharedData().m_success_message);
        }
        else if (m_is_timed)
        {
            ++GetSharedData().m_total_timed_functions;
            LogWrite(GetSharedData().m_timed_message);
        }
        else
        {
            ++GetSharedData().m_total_not_tested;
            LogWrite(GetSharedData().m_not_run_message);
        }
    }

    // The three variations that can be logged for a test.
    void privateLogTime()
    {
        TestTime diff_time;
        m_current_time.GetDiffTime(m_start_time, diff_time);

        TestString msg(" ");
        msg.Append(privateGetMemberFunctionPadding());

        msg.Append(" (elapsed) ");
        msg.Append(diff_time.GetSeconds());
        msg.Append(".");
        msg.Append(diff_time.GetMilliseconds(), TestTime::MILLISECONDS_CHARS);

        msg.Append(". calls ");
        msg.Append(static_cast<unsigned long>(m_timed_function_calls));

        if (m_timed_function_calls > 0)
        {
            diff_time /= m_timed_function_calls;
            msg.Append(". call time ");
            msg.Append(diff_time.GetSeconds());
            msg.Append(".");
            msg.Append(diff_time.GetNanoseconds(), TestTime::NANOSECONDS_CHARS);
        }
        else
            msg.Append(". cannot calculate call time!");

        LogWriteLine(msg);
    }

    // Log the part that follows the function name.
    void privateLogNumberOfRunTests()
    {
        TestString msg(" ");
        msg.Append(privateGetMemberFunctionPadding());

        if (m_check_count > 0)
        {
            msg.Append(static_cast<unsigned long>(m_check_count));
            if (m_check_count > 1)
                msg.Append(" TESTS");
            else
                msg.Append(" TEST");
            if (m_failure_check_count > 0)
            {
                msg.Append(" (");
                msg.Append(static_cast<unsigned long>(m_failure_check_count));
                msg.Append(" FAILED)");
            }
        }
        else
            msg.Append("** NO TESTS **");
        LogWriteLine(msg);
    }

    // Log the output for a failed check macro.
    void privateLogFailures()
    {
        static TestString const indent(' ', GetSharedData().m_failure_indent);

        TestString msg(indent);
        msg.Append("FILE: ");
        msg.Append(m_filename);
        LogWriteLine(msg);
        msg.Clear();

        // Extract each line from the string until there are no more failed checks.
        while (!m_check_failures.IsEmpty())
        {
            size_type pos = 0;
            if (m_check_failures.Find('\n', pos))
            {
                m_check_failures.GetSubString(msg, 0, pos + 1, true);
                msg.Prepend(indent);
                LogWrite(msg);
            }
            else
            {
                m_check_failures.Prepend(indent);
                LogWrite(m_check_failures);
                m_check_failures.Clear();
            }
        }
    }

    // Log the number of checks for a tested function, e.g. 5 TESTS or 1 TEST.
    void privateLogCount(TestString const& msg, size_type count)
    {
        TestString count_msg(msg);
        count_msg += " = ";
        count_msg.Append(static_cast<unsigned long>(count));
        if (m_logger != NULL)
            m_logger->WriteLine(count_msg);
    }

    // Output the final summary report.
    void privateLogSummary()
    {
        if (m_logger != NULL)
        {
            m_logger->WriteLine("");
            privateLogCount("Total checks", GetSharedData().m_total_checks);
            if (GetSharedData().m_total_not_tested > 0)
                privateLogCount("Total not tested", GetSharedData().m_total_not_tested);
            if (HasSharedFailure())
                privateLogCount("Total failed tests", GetSharedData().m_total_failed_tests);
            privateLogCount("Total functions tested", GetSharedData().m_total_functions_tested);
            if (GetSharedData().m_total_timed_functions > 0)
                privateLogCount("Total functions timed", GetSharedData().m_total_timed_functions);
            privateLogCount("Total tests", GetSharedData().m_total_tests);
        }
    }

    // Every function or member function test calls this construct function,
    // which sets up the initial logger and keeps track of number of tests.
    void privateConstruct()
    {
        if (GetSharedData().m_stdio_logger != NULL)
            m_logger = GetSharedData().m_stdio_logger;
        else
            LogWriteLine("Error setting logger!");

        // NOTE: SetFailureIndent will decrement m_constructions and m_total_tests later.
        ++GetSharedData().m_constructions;
        ++GetSharedData().m_total_tests;

        m_test_number = GetSharedData().m_constructions;

        // Calculate longest function output so results can be padded to create aligned columns.
        privateUpdateMaxFunctionLength();
    }

    // Pad the member function with spaces to line up right column.
    TestString privateGetMemberFunctionPadding()
    {
        TestString padding;
        size_type len = m_member_function.GetLength();
        if (len < GetSharedData().m_max_member_function_length)
            padding.Append(TestString(' ', GetSharedData().m_max_member_function_length - len));
        return padding;
    }

    void privateUpdateMaxFunctionLength()
    {
        GetTestName(m_member_function);
        if (m_previous_member_function_length > 0)
        {
            size_type new_len = m_member_function.GetLength();

            // a call to SetFunctionName or SetArgs has changed the length,
            // so re-calculate the max length.
            if (new_len > m_previous_member_function_length)
                GetSharedData().m_max_member_function_length = new_len;
            else
                GetSharedData().m_max_member_function_length = m_previous_member_function_length;
        }
        else
        {
            // first time to be set, so update the static max length.
            m_previous_member_function_length = GetSharedData().m_max_member_function_length;
            if (m_member_function.GetLength() > GetSharedData().m_max_member_function_length)
                GetSharedData().m_max_member_function_length = m_member_function.GetLength();
        }
    }

    void privateSetFilename(TestString const& filename)
    {
        if (m_filename.IsEmpty())
            m_filename = filename;
    }

    void privateRecordFailed(TestString const& expression,
                             TestString const& filename,
                             size_type line_number)
    {
        ++GetSharedData().m_total_failed_tests;
        m_check_failures.Append("LINE: ");
        m_check_failures.Append(static_cast<unsigned long>(line_number));
        m_check_failures.Append("\nFILENAME: ");
        m_check_failures.Append(filename);
        m_check_failures.Append("\nEXPRESSION: ");
        m_check_failures.Append(expression);
        m_check_failures.Append("\n");
    }

private:
    static TestClassSharedData<size_type>& GetSharedData()
    {
        // Ensure shared data is available for first use.
        static TestClassSharedData<size_type> shared_data;
        return shared_data;
    }

// Data for this test.
private:
    TestLog* m_logger;
    TestString m_filename;
    TestString m_class_name;
    TestString m_function_name;
    TestString m_args;
    bool m_is_const;
    bool m_is_timed;

    // When this test is timed, record the start time.
    TestTime m_start_time;

    // Timer used to check progress in the sample time period.
    TestTime m_current_time;
    size_type m_timed_function_calls;

    // The amount of time to run the performance test.
    TestTime m_sample_time;

    // Store class name, function name, args and const in single string.
    // This can be used to calculate the longest string.
    TestString m_member_function;

    // total checks for this function.
    size_type m_check_count;

    // test failures for this function.
    size_type m_failure_check_count;
    TestString m_check_failures;

    // Store m_max_member_function_length before update in this class.
    // This can change as SetFunctionName or SetArgs is called.
    size_type m_previous_member_function_length;

    // Debug test number helper for identifying when a test crashes.
    size_type m_test_number;

    // When this test is used to set the indentation for failure messages,
    // then this test does not need to be recorded.
    bool m_recorded;

private:
    TestClass(TestClass const&);
    TestClass& operator =(TestClass const&);
};

} // namespace ocl

#endif // OCL_GUARD_TEST_TESTCLASS_HPP
