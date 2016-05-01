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

#ifndef OCL_GUARD_TEST_TESTCLASSSHAREDDATA_HPP
#define OCL_GUARD_TEST_TESTCLASSSHAREDDATA_HPP

#include "StdioTestLog.hpp"

namespace ocl
{

/**
 * All shared data for every test which is derived from TestClass
 * is in TestClassSharedData class.
 */
template<typename SizeType>
class TestClassSharedData
{
friend class TestClass;

public:
    TestClassSharedData()
        : m_logger(new StdioTestLog)
        , m_max_member_function_length(0)
        , m_failure_indent(0)
        , m_constructions(0)
        , m_logged_line(0)
        , m_total_checks(0)
        , m_total_not_tested(0)
        , m_total_failed_tests(0)
        , m_total_functions_tested(0)
        , m_total_timed_functions(0)
        , m_total_tests(0)

        // Padded to be lined up for output as columns.
        , m_success_message("SUCCESS - ")
        , m_failed_message (" FAILED - ")
        , m_not_run_message("NOT RUN - ")
        , m_timed_message  ("  TIMED - ")
    {
    }

    ~TestClassSharedData()
    {
        delete m_logger;
    }

    void SetLogger(TestLog* logger)
    {
        delete m_logger;
        m_logger = logger;
    }

private:
    TestClassSharedData(TestClassSharedData const&);
    TestClassSharedData& operator=(TestClassSharedData const&);

// Data only available to TestClass.
private:
    // Default logger when one is not provided.
    TestLog* m_logger;

    // Track longest member function for cleaner output.
    SizeType m_max_member_function_length;

    // Number of spaces to indent error information.
    SizeType m_failure_indent;

    // Count all constructions and delete m_logger of last destructor.
    SizeType m_constructions;

    // Currently logged line number for the run test.
    SizeType m_logged_line;

    // Counts for all tests.
    SizeType m_total_checks;
    SizeType m_total_not_tested;
    SizeType m_total_failed_tests;
    SizeType m_total_functions_tested;
    SizeType m_total_timed_functions;
    SizeType m_total_tests;

    // message for start of each test.
    const char* m_success_message;
    const char* m_failed_message;
    const char* m_not_run_message;
    const char* m_timed_message;
};

} // namespace ocl

#endif // OCL_GUARD_TEST_TESTCLASSSHAREDDATA_HPP
