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

#include "TestClassSharedData.hpp"

namespace ocl
{

TestClassSharedData::TestClassSharedData()
    : m_max_member_function_length(0)
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
    m_stdio_logger = new StdioTestLog;
}

TestClassSharedData::~TestClassSharedData()
{
    delete m_stdio_logger;
}

} // namespace ocl
