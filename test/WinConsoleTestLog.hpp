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

#ifndef OCL_GUARD_UNIT_TEST_FRAMEWORK_TEST_WINCONSOLETESTLOG_HPP
#define OCL_GUARD_UNIT_TEST_FRAMEWORK_TEST_WINCONSOLETESTLOG_HPP

#ifdef _MSC_VER

#include <Windows.h>
#include "TestLog.hpp"
#include <iostream>

namespace ocl
{

class WinConsoleTestLog : public TestLog
{
public:
    static bool IsDebugging()
    {
        BOOL debugging_status = FALSE;
        bool is_debugging;
        if (::CheckRemoteDebuggerPresent(GetCurrentProcess(), &debugging_status) != FALSE)
            is_debugging = debugging_status != FALSE;
        else
            is_debugging = false;
        return is_debugging;
    }

    virtual void Write(TestString const& str)
    {
        static bool is_debugging = IsDebugging();

        TestString::size_type len = str.GetLength();
        bool eol = (len > 0) && (str[len-1] == '\n');
        if (eol)
        {
            if (len > 1)
            {
                TestString sub_str = str.GetSubString(0, len - 1);
                if (is_debugging)
                    ::OutputDebugStringA(sub_str.Ptr());
                else
                    std::cout << sub_str.Ptr();

            }
            WriteEOL();
        }
        else if (is_debugging)
            ::OutputDebugStringA(str.Ptr());
        else
            std::cout << str.Ptr();
    }

    virtual void WriteEOL()
    {
        static bool is_debugging = IsDebugging();

        if (is_debugging)
            ::OutputDebugStringA("\n");
        else
            std::cout << std::endl;
    }
};

} // namespace ocl

#endif // defined _MSC_VER

#endif // OCL_GUARD_UNIT_TEST_FRAMEWORK_TEST_WINCONSOLETESTLOG_HPP
