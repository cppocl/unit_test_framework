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

class TestClassSharedData
{
friend class TestClass;

public:
    TestClassSharedData();
    ~TestClassSharedData();

// Data only available to TestClass.
private:
    // Track longest member function for cleaner output.
    size_t m_max_member_function_length;

    // Number of spaces to indent error information.
    size_t m_failure_indent;

    // Default logger when one is not provided.
    StdioTestLog* m_stdio_logger;

    // Count all constructions and delete m_stdio_logger of last destructor.
    size_t m_constructions;

    // Currently logged line number.
    size_t m_logged_line;

    // Counts for all tests.
    size_t m_total_checks;
    size_t m_total_not_tested;
    size_t m_total_failed_tests;
    size_t m_total_functions_tested;
    size_t m_total_timed_functions;
    size_t m_total_tests;

    // message for start of each test.
    const char* m_success_message;
    const char* m_failed_message;
    const char* m_not_run_message;
    const char* m_timed_message;
};

} // namespace ocl

#endif // OCL_GUARD_TEST_TESTCLASSSHAREDDATA_HPP
