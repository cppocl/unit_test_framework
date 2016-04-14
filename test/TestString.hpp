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

#ifndef OCL_GUARD_TEST_TESTSTRING_HPP
#define OCL_GUARD_TEST_TESTSTRING_HPP

#include "TestMemoryUtility.hpp"
#include "TestStringUtility.hpp"
#include <cstddef>
#include <cstdlib>
#include <cstring>

namespace ocl
{

/*
 * String class for test harness, with an interface complete enough for general usage.
 */

class TestString
{
public:
    TestString(char const* str = NULL)
        : m_length(0)
        , m_string(NULL)
    {
        if (str != NULL)
            TestStringUtility::UnsafeAllocateCopy(m_string, m_length, str);
    }

    TestString(char ch, size_t len)
        : m_length(len)
        , m_string(NULL)
    {
        if (len > 0)
        {
            TestStringUtility::Allocate(m_string, len);
            if (m_string != NULL)
                TestStringUtility::UnsafeFill(m_string, ch, len);
            else
                m_length = 0;
        }
    }

    TestString(TestString const& str)
        : m_length(0)
        , m_string(NULL)
    {
        TestStringUtility::SafeAllocateCopy(m_string, m_length, str.m_string, str.m_length);
    }

    ~TestString()
    {
        TestStringUtility::FastFree(m_string);
    }

    TestString& operator =(char const* str)
    {
        TestStringUtility::SafeReallocCopy(m_string, m_length, str);
        return *this;
    }

    TestString& operator =(TestString const& str)
    {
        TestStringUtility::SafeReallocCopy(m_string,
                                           m_length,
                                           str.m_string,
                                           str.m_length);

        return *this;
    }

    TestString& operator +=(char const* str)
    {
        Append(str);
        return *this;
    }

    TestString& operator +=(TestString const& str)
    {
        Append(str);
        return *this;
    }

    char operator [](size_t index) const
    {
        return ((m_string != NULL) && (index < m_length)) ? m_string[index] : '\0';
    }

    char& operator [](size_t index)
    {
        static char spare = '\0';
        return ((m_string != NULL) && (index < m_length)) ? m_string[index] : spare;
    }

    operator char const*() const throw()
    {
        return Ptr();
    }

    bool operator ==(char const* str) const
    {
        if (m_string == NULL)
            return str == NULL;
        return ::strcmp(m_string, str) == 0;
    }

    bool operator ==(TestString const& str) const
    {
        if (m_string == NULL)
            return str.m_string == NULL;
        return ::strcmp(m_string, str.Ptr()) == 0;
    }

public:
    char const* Ptr() const throw()
    {
        return m_string != NULL ? m_string : "";
    }

    size_t GetLength() const throw()
    {
        return m_length;
    }

    bool IsEmpty() const throw()
    {
        return m_string == NULL;
    }

    void Clear()
    {
        TestStringUtility::FastFree(m_string);
        m_string = NULL;
        m_length = 0;
    }

    bool Find(char ch, size_t& pos, size_t start = 0) const
    {
        if (m_string != NULL)
            return TestStringUtility::UnsafeFind(m_string, ch, pos, start);
        return false;
    }

    /// Extract a partial string, and optionally remove the partial string
    /// from this string.
    void GetSubString(TestString& sub_str,
                      size_t start,
                      size_t count,
                      bool remove = false)
    {
        sub_str.Clear();
        TestStringUtility::SafeReallocCopy(sub_str.m_string,
                                           sub_str.m_length,
                                           m_string + start,
                                           count);
        if (remove)
        {
            if (sub_str.m_length < m_length)
            {
                size_t chars_remaining = m_length - sub_str.m_length;
                ::memmove(m_string, m_string + sub_str.m_length, chars_remaining);
                *(m_string + chars_remaining) = '\0';
                m_length = chars_remaining;
            }
            else
                Clear();
        }
    }

    /// Replace the current sting with str.
    void Assign(char const* str)
    {
        TestStringUtility::SafeReallocCopy(m_string, m_length, str);
    }

    /// Replace the current sting with str.
    void Assign(TestString const& str)
    {
        TestStringUtility::SafeReallocCopy(m_string, m_length, str.m_string);
    }

    /// Move str into this string.
    void Move(char*& str, size_t len)
    {
        TestStringUtility::FastFree(m_string);
        m_string = str;
        m_length = len;
        str = NULL;
    }

    void Prepend(char const* str)
    {
        TestStringUtility::SafeReallocAppend(m_string,
                                             m_length,
                                             str,
                                             privateSafeLength(str),
                                             m_string,
                                             m_length);
    }

    void Append(char const* str)
    {
        TestStringUtility::SafeReallocAppend(m_string,
                                             m_length,
                                             m_string,
                                             m_length,
                                             str,
                                             privateSafeLength(str));
    }

    void Append(char const* str, size_t count)
    {
        TestStringUtility::SafeReallocAppend(m_string,
                                             m_length,
                                             m_string,
                                             m_length,
                                             str,
                                             count);
    }

    void Append(TestString const& str)
    {
        TestStringUtility::SafeReallocAppend(m_string,
                                             m_length,
                                             m_string,
                                             m_length,
                                             str.m_string,
                                             str.m_length);
    }

    /// Append a boolean converted to a "true" ot "false" string.
    void Append(bool value)
    {
        char const* str = value ? "true" : "false";
        Append(str);
    }

    /// Append a single character.
    void Append(char value)
    {
        char str[2];
        str[0] = value;
        str[1] = '\0';
        Append(str);
    }

    /// Append number of characters.
    void Append(char value, size_t count)
    {
        if (count > 0)
        {
            char* str = NULL;
            TestStringUtility::Allocate(str, count);
            if (str != NULL)
            {
                char* start = str;
                for (char* str_end = str + count; str < str_end; ++str)
                    *str = value;
                Append(start, count);
                TestStringUtility::FastFree(start);
            }
            else
                Append(value);
        }
    }

    /// Append number of characters then a string value.
    void Append(char ch, size_t count, char const* value, size_t len)
    {
        if (count > 0)
        {
            char* str = NULL;
            TestStringUtility::Allocate(str, count + len);
            if (str != NULL)
            {
                char* start = str;
                for (char* str_end = str + count; str < str_end; ++str)
                    *str = ch;
                ::memcpy(str, value, len + 1);
                Append(start, count + len);
                TestStringUtility::FastFree(start);
            }
            else
                Append(ch);
        }
        else
            Append(value, len);
    }

    void Append(signed char value, size_t pad = 0)
    {
        privateAppendValue(value, pad);
    }

    void Append(unsigned char value, size_t pad = 0)
    {
        privateAppendValue(value, pad);
    }

    void Append(signed short value, size_t pad = 0)
    {
        privateAppendValue(value, pad);
    }

    void Append(unsigned short value, size_t pad = 0)
    {
        privateAppendValue(value, pad);
    }

    void Append(signed int value, size_t pad = 0)
    {
        privateAppendValue(value, pad);
    }

    void Append(unsigned int value, size_t pad = 0)
    {
        privateAppendValue(value, pad);
    }

    void Append(signed long value, size_t pad = 0)
    {
        privateAppendValue(value, pad);
    }

    void Append(unsigned long value, size_t pad = 0)
    {
        privateAppendValue(value, pad);
    }

private:
    template<typename T>
    void privateAppendValue(T value, size_t pad)
    {
        size_t length = 0;
        char* str = TestStringUtility::GetString(value, length);
        if (str != NULL)
        {
            if (IsEmpty() && (pad == 0))
                Move(str, length);
            else
            {
                if (pad > length)
                    Append('0', pad - length, str, length);
                else
                    Append(str, length);
                TestStringUtility::FastFree(str);
            }
        }
    }

    size_t privateSafeLength(char const* str)
    {
        return TestStringUtility::SafeLength(str);
    }

private:
    size_t m_length;
    char* m_string;
};

} // namespace ocl

#endif // OCL_GUARD_TEST_TESTSTRING_HPP
