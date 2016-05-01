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

TEST_FAILURE_INDENT(4);

int main(int /*argc*/, char * /*argv*/[])
{
    TEST_OVERRIDE_LOG(ocl::TestStdioFileFunctor, new ocl::TestStdioFileFunctor("unit_test_log.txt"));
    TEST_OVERRIDE_FILE_LOG("unit_test_log.txt");
    TEST_OVERRIDE_STDIO_FILE_LOG("unit_test_log.txt");

    bool test_has_failed = TEST_HAS_FAILED;

    return test_has_failed ? 1 : 0;
}
