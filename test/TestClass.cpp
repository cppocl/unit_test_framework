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

#include "TestClass.hpp"
#include <iostream>

using ocl::TestTime;
using ocl::TestClass;
using ocl::TestString;
using ocl::StdioTestLog;

TestClass::TestClass()
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

TestClass::TestClass(TestString const& class_name,
                     TestString const& function_name,
                     TestString const& args,
                     bool is_const,
                     bool is_timed,
                     unsigned long secs,
                     unsigned long nanosecs)
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

TestClass::~TestClass()
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

TestString const& TestClass::GetClassName() const
{
    return m_class_name;
}

void TestClass::SetClassName(TestString const& class_name)
{
    m_class_name = class_name;
    privateUpdateMaxFunctionLength();
}

TestString const& TestClass::GetFunctionName() const
{
    return m_function_name;
}

void TestClass::SetFunctionName(TestString const& function_name)
{
    m_function_name = function_name;
    privateUpdateMaxFunctionLength();
}

TestString const& TestClass::GetArgs() const
{
    return m_args;
}

void TestClass::SetArgs(TestString const& args)
{
    m_args = args;
    privateUpdateMaxFunctionLength();
}

void TestClass::GetTestName(TestString& str) const
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

void TestClass::SetFailureIndent(size_t indent)
{
    m_recorded = false;
    GetSharedData().m_failure_indent = indent;

    // Don't want to count constructors for the indentation object,
    // as this is not needed in the unit test statistics.
    --GetSharedData().m_constructions;
    --GetSharedData().m_total_tests;
}

bool TestClass::HasTests() const
{
    return m_check_count > 0;
}

bool TestClass::HasFailed() const
{
    return m_failure_check_count > 0;
}

bool TestClass::IsGeneralTest() const
{
    return GetFunctionName().IsEmpty();
}

bool TestClass::IsLast() const
{
    return GetSharedData().m_constructions == 0;
}

void TestClass::LogWrite(TestString const& msg)
{
    if (m_logger != NULL)
        m_logger->Write(msg);
}

void TestClass::LogWriteLine(TestString const& msg)
{
    if (m_logger != NULL)
        m_logger->WriteLine(msg);
}

TestTime const& TestClass::GetStartTime() const throw()
{
    return m_start_time;
}

TestTime const& TestClass::GetSampleTime() const throw()
{
    return m_sample_time;
}

TestTime const& TestClass::GetCurrentTime() const throw()
{
    return m_current_time;
}

bool TestClass::CheckTime()
{
    TestTime stop_time(m_start_time);
    stop_time += m_sample_time;

    m_current_time.Refresh();

    ++m_timed_function_calls;

    return m_current_time > stop_time;
}

void TestClass::CheckTrue(TestString const& expression,
                          TestString const& filename,
                          size_t line_number,
                          bool value)
{
    LogCheck(expression, filename, line_number, !value);
}

void TestClass::CheckFalse(TestString const& expression,
                           TestString const& filename,
                           size_t line_number,
                           bool value)
{
    LogCheck(expression, filename, line_number, value);
}

void TestClass::CheckNull(TestString const& expression,
                          TestString const& filename,
                          size_t line_number,
                          void const* ptr)
{
    LogCheck(expression, filename, line_number, ptr != NULL);
}

void TestClass::CheckNotNull(TestString const& expression,
                             TestString const& filename,
                             size_t line_number,
                             void const* ptr)
{
    LogCheck(expression, filename, line_number, ptr == NULL);
}

void TestClass::CheckException(TestString const& expression,
                               TestString const& filename,
                               size_t line_number,
                               bool found_exception,
                               bool expect_exception)
{
    bool error = (expect_exception && !found_exception) || (!expect_exception && found_exception);
    LogCheck(expression, filename, line_number, error);
}

void TestClass::privateLogFunction()
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

void TestClass::privateLogFunctionLineNumber()
{
    TestString line_number_str;
    unsigned long total_tests = static_cast<unsigned long>(GetSharedData().m_total_tests);
    size_t pad_size = TestNumericUtility<unsigned long>::GetNumberOfCharsForInt(total_tests);
    line_number_str.Append("(");
    ++GetSharedData().m_logged_line;
    line_number_str.Append(static_cast<unsigned long>(GetSharedData().m_logged_line), pad_size);
    line_number_str.Append(") ");
    LogWrite(line_number_str);
}

void TestClass::privateLogTestStatus()
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

void TestClass::privateLogTime()
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

void TestClass::privateLogNumberOfRunTests()
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

void TestClass::privateLogFailures()
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
        size_t pos = 0;
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

void TestClass::privateLogCount(TestString const& msg, size_t count)
{
    TestString count_msg(msg);
    count_msg += " = ";
    count_msg.Append(static_cast<unsigned long>(count));
    if (m_logger != NULL)
        m_logger->WriteLine(count_msg);
}

void TestClass::privateLogSummary()
{
    if (m_logger != NULL)
    {
        m_logger->WriteLine("");
        privateLogCount("Total checks", GetSharedData().m_total_checks);
        if (GetSharedData().m_total_not_tested > 0)
            privateLogCount("Total not tested", GetSharedData().m_total_not_tested);
        if (GetSharedData().m_total_failed_tests > 0)
            privateLogCount("Total failed tests", GetSharedData().m_total_failed_tests);
        privateLogCount("Total functions tested", GetSharedData().m_total_functions_tested);
        if (GetSharedData().m_total_timed_functions > 0)
            privateLogCount("Total functions timed", GetSharedData().m_total_timed_functions);
        privateLogCount("Total tests", GetSharedData().m_total_tests);
    }
}

void TestClass::LogCheck(TestString const& expression,
                         TestString const& filename,
                         size_t line_number,
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

void TestClass::privateConstruct()
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

TestString TestClass::privateGetMemberFunctionPadding()
{
    TestString padding;
    size_t len = m_member_function.GetLength();
    if (len < GetSharedData().m_max_member_function_length)
        padding.Append(TestString(' ', GetSharedData().m_max_member_function_length - len));
    return padding;
}

void TestClass::privateUpdateMaxFunctionLength()
{
    GetTestName(m_member_function);
    if (m_previous_member_function_length > 0)
    {
        size_t new_len = m_member_function.GetLength();

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

void TestClass::privateSetFilename(TestString const& filename)
{
    if (m_filename.IsEmpty())
        m_filename = filename;
}

void TestClass::privateRecordFailed(TestString const& expression,
                                    TestString const& filename,
                                    size_t line_number)
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

ocl::TestClassSharedData& TestClass::GetSharedData() const
{
    // Ensure shared data is available for first use.
    static TestClassSharedData shared_data;
    return shared_data;
}
