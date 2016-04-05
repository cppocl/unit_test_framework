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
    TestClass(TestString const& class_name,
              TestString const& function_name,
              TestString const& args,
              bool is_const = false,
              bool is_timed = false,
              unsigned long secs = 0,
              unsigned long nanosecs = 0);

    virtual ~TestClass();

// Functions for managing the function or member function test.
public:
    TestString const& GetClassName() const;
    void SetClassName(TestString const& class_name);

    TestString const& GetFunctionName() const;
    void SetFunctionName(TestString const& function_name);

    TestString const& GetArgs() const;
    void SetArgs(TestString const& args);

    /// return string as class name::function name([args]) [const]
    /// , function name([args]) or general test name.
    void GetTestName(TestString& str) const;

// General helper functions.
public:
    /// Underlying function for TEST_FAILURE_INDENT macro.
    void SetFailureIndent(size_t indent);

    /// Get status for tests and failures.
    bool HasTests() const;
    bool HasFailed() const;

    bool IsGeneralTest() const;

    bool IsLast() const;

    void LogWrite(TestString const& msg);

    void LogWriteLine(TestString const& msg);

    TestTime const& GetStartTime() const throw();
    TestTime const& GetSampleTime() const throw();
    TestTime const& GetCurrentTime() const throw();

// Check functions
public:
    void CheckTrue(TestString const& expression,
                   TestString const& filename,
                   size_t line_number,
                   bool value);

    void CheckFalse(TestString const& expression,
                    TestString const& filename,
                    size_t line_number,
                    bool value);

    void CheckNull(TestString const& expression,
                   TestString const& filename,
                   size_t line_number,
                   void const* ptr);

    void CheckNotNull(TestString const& expression,
                      TestString const& filename,
                      size_t line_number,
                      void const* ptr);

    template<typename T1, typename T2>
    void CheckEqual(TestString const& expression,
                    TestString const& filename,
                    size_t line_number,
                    T1 const& value1,
                    T2 const& value2)
    {
        bool is_equal = value1 == value2;
        LogCheck(expression, filename, line_number, !is_equal);
    }

    template<typename T1, typename T2>
    void CheckNotEqual(TestString const& expression,
                       TestString const& filename,
                       size_t line_number,
                       T1 const& value1,
                       T2 const& value2)
    {
        bool is_not_equal = value1 != value2;
        LogCheck(expression, filename, line_number, !is_not_equal);
    }

    template<typename T1, typename T2>
    void CheckGreater(TestString const& expression,
                      TestString const& filename,
                      size_t line_number,
                      T1 const& value1,
                      T2 const& value2)
    {
        bool is_greater = value1 > value2;
        LogCheck(expression, filename, line_number, !is_greater);
    }

    template<typename T1, typename T2>
    void CheckGreaterEqual(TestString const& expression,
                           TestString const& filename,
                           size_t line_number,
                           T1 const& value1,
                           T2 const& value2)
    {
        bool is_greater_equal = value1>= value2;
        LogCheck(expression, filename, line_number, !is_greater_equal);
    }

    template<typename T1, typename T2>
    void CheckLess(TestString const& expression,
                   TestString const& filename,
                   size_t line_number,
                   T1 const& value1,
                   T2 const& value2)
    {
        bool is_less = value1 < value2;
        LogCheck(expression, filename, line_number, !is_less);
    }

    template<typename T1, typename T2>
    void CheckLessEqual(TestString const& expression,
                        TestString const& filename,
                        size_t line_number,
                        T1 const& value1,
                        T2 const& value2)
    {
        bool is_less_equal = value1 <= value2;
        LogCheck(expression, filename, line_number, !is_less_equal);
    }

    template<typename T>
    void CheckZero(TestString const& expression,
                   TestString const& filename,
                   size_t line_number,
                   T value)
    {
        bool is_equal = value == static_cast<T>(0);
        LogCheck(expression, filename, line_number, !is_equal);
    }

    template<typename T>
    void CheckNotZero(TestString const& expression,
                      TestString const& filename,
                      size_t line_number,
                      T value)
    {
        bool is_not_equal = value != static_cast<T>(0);
        LogCheck(expression, filename, line_number, !is_not_equal);
    }

    template<typename T1, typename T2>
    void CheckCompare(TestString const& expression,
                      TestString const& filename,
                      size_t line_number,
                      T1 const& value1,
                      T2 const& value2)
    {
        int compare = TestCompare<T1, T2>::Compare(value1, value2);
        LogCheck(expression, filename, line_number, compare == 0);
    }

    template<typename T1, typename T2>
    void CheckNotCompare(TestString const& expression,
                         TestString const& filename,
                         size_t line_number,
                         T1 const& value1,
                         T2 const& value2)
    {
        int compare = TestCompare<T1, T2>::Compare(value1, value2);
        LogCheck(expression, filename, line_number, compare != 0);
    }

    /// While the current time has not reached the start time + sample time,
    /// keep returning false.
    /// @note This function also refreshes the current time.
    bool CheckTime();

// Unit test helper functions
#include "TestClassHelperFunctions.inl"

protected:
    // TEST_FUNCTION and TEST_MEMBER_FUNCTION create their own constructor,
    // so access to the TestClass constructor does not require direct access.
    TestClass();

    // Log check function used by all check macros.
    void LogCheck(TestString const& expression,
                  TestString const& filename,
                  size_t line_number,
                  bool failed);

private:
    // Log the whole line for a function or member function.
    void privateLogFunction();

    // Log the current tested function with a line number.
    void privateLogFunctionLineNumber();

    // Output SUCCESS, FAILED, NOT RUN or TIMED message.
    void privateLogTestStatus();

    // The three variations that can be logged for a test.
    void privateLogTime();

    // Log the part that follows the function name.
    void privateLogNumberOfRunTests();

    // Log the output for a failed check macro.
    void privateLogFailures();

    // Log the number of checks for a tested function, e.g. 5 TESTS or 1 TEST.
    void privateLogCount(TestString const& msg, size_t count);

    // Output the final summary report.
    void privateLogSummary();

    // Every function or member function test calls this construct function,
    // which sets up the initial logger and keeps track of number of tests.
    void privateConstruct();

    // Pad the member function with spaces to line up right column.
    TestString privateGetMemberFunctionPadding();

    void privateUpdateMaxFunctionLength();

    void privateSetFilename(TestString const& filename);

    void privateRecordFailed(TestString const& expression,
                             TestString const& filename,
                             size_t line_number);


private:
    ocl::TestClassSharedData& GetSharedData() const;

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
    size_t m_timed_function_calls;

    // The amount of time to run the performance test.
    TestTime m_sample_time;

    // Store class name, function name, args and const in single string.
    // This can be used to calculate the longest string.
    TestString m_member_function;

    // total checks for this function.
    size_t m_check_count;

    // test failures for this function.
    size_t m_failure_check_count;
    TestString m_check_failures;

    // Store m_max_member_function_length before update in this class.
    // This can change as SetFunctionName or SetArgs is called.
    size_t m_previous_member_function_length;

    // Debug test number helper for identifying when a test crashes.
    size_t m_test_number;

    // When this test is used to set the indentation for failure messages,
    // then this test does not need to be recorded.
    bool m_recorded;

private:
    TestClass(TestClass const&);
    TestClass& operator =(TestClass const&);
};

} // namespace ocl

#endif // OCL_GUARD_TEST_TESTCLASS_HPP
