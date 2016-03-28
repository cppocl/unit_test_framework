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

*******************************************************************************

class: Array

description: Sample template array class implementation file.
*/

template<typename Type, typename SizeType>
Array<Type, SizeType>::Array() throw()
    : m_size(0)
    , m_array(NULL)
{
}

template<typename Type, typename SizeType>
Array<Type, SizeType>::Array(SizeType initial_size)
    : m_size(initial_size)
{
    privateAlloc(m_array, initial_size);
}

template<typename Type, typename SizeType>
Array<Type, SizeType>::Array(simple_array_type const& array_value)
    : m_size(0)
    , m_array(NULL)
{
    Copy(array_value);
}

template<typename Type, typename SizeType>
Array<Type, SizeType>::~Array()
{
    privateFree(m_array);
}

template<typename Type, typename SizeType>
typename Array<Type, SizeType>::simple_array_type&
Array<Type, SizeType>::operator =(simple_array_type const& rhs)
{
    Copy(rhs);
    return *this;
}

template<typename Type, typename SizeType>
Type const& Array<Type, SizeType>::operator[](SizeType index) const
{
    static std::string const msg("operator[](index) const");
    privateIndexCheck(index, msg);
    return m_array[index];
}

template<typename Type, typename SizeType>
Type& Array<Type, SizeType>::operator[](SizeType index)
{
    static std::string const msg("operator[](index)");
    privateIndexCheck(index, msg);
    return m_array[index];
}

template<typename Type, typename SizeType>
typename Array<Type, SizeType>::simple_array_type&
Array<Type, SizeType>::operator+=(Type const& value)
{
    Append(value);
    return *this;
}

template<typename Type, typename SizeType>
typename Array<Type, SizeType>::simple_array_type&
Array<Type, SizeType>::operator+=(simple_array_type const& array_value)
{
    Append(array_value);
    return *this;
}

template<typename Type, typename SizeType>
bool Array<Type, SizeType>::operator==(simple_array_type const& rhs) const throw()
{
    if (m_array == NULL)
        return rhs.m_array == NULL ? true : false;
    if (rhs.m_array == NULL)
        return false;
    Type const* dest = m_array;
    Type const* src  = rhs.m_array;
    Type const* src_end = (m_size < rhs.m_size) ? src + m_size : src + rhs.m_size;
    while (src < src_end)
        if (*dest++ != *src++)
            return false;
    return true;
}

template<typename Type, typename SizeType>
bool Array<Type, SizeType>::operator!=(simple_array_type const& rhs) const throw()
{
    return !(*this == rhs);
}

template<typename Type, typename SizeType>
bool Array<Type, SizeType>::IsEmpty() const throw()
{
    return m_size == 0;
}

template<typename Type, typename SizeType>
SizeType Array<Type, SizeType>::GetSize() const throw()
{
    return m_size;
}

template<typename Type, typename SizeType>
void Array<Type, SizeType>::Clear()
{
    privateFree(m_array);
    m_array = NULL;
    m_size = 0;
}

template<typename Type, typename SizeType>
Type const* Array<Type, SizeType>::Ptr() const throw()
{
    return m_array;
}

template<typename Type, typename SizeType>
Type* Array<Type, SizeType>::Ptr() throw()
{
    return m_array;
}

template<typename Type, typename SizeType>
void Array<Type, SizeType>::Copy(simple_array_type const& array_value)
{
    privateRealloc(m_array, array_value.m_size);
    privateCopy(m_array, array_value.m_size, array_value.m_array, array_value.m_size);
    m_size = array_value.m_size;
}

template<typename Type, typename SizeType>
void Array<Type, SizeType>::Move(simple_array_type& array_value)
{
    Clear();
    m_array = array_value.m_array;
    m_size = array_value.m_size;
    array_value.m_array = NULL;
    array_value.m_size = 0;
}

template<typename Type, typename SizeType>
void Array<Type, SizeType>::Append(Type const& value)
{
    Type* new_array = NULL;
    SizeType new_size = m_size + 1;
    privateRealloc(new_array, new_size);
    privateCopy(new_array, new_size, m_array, m_size);
    new_array[m_size] = value;
    privateFree(m_array);
    m_array = new_array;
    m_size = new_size;
}

template<typename Type, typename SizeType>
void Array<Type, SizeType>::Append(simple_array_type const& array_value)
{
    if (!array_value.IsEmpty())
    {
        Type* new_array = NULL;
        SizeType value_size = array_value.GetSize();
        SizeType new_size = m_size + value_size;
        privateRealloc(new_array, new_size);
        privateCopy(new_array, new_size, m_array, m_size);
        privateCopy(new_array + m_size, value_size, array_value.Ptr(), value_size);
        privateFree(m_array);
        m_array = new_array;
        m_size = new_size;
    }
}

template<typename Type, typename SizeType>
void Array<Type, SizeType>::DeleteAt(SizeType index)
{
    static std::string const msg("DeleteAt(index)");
    privateIndexCheck(index, msg);

    if (m_size > 1)
    {
        Type* new_array;
        SizeType new_size = m_size - 1;
        privateAlloc(new_array, new_size);
        if (index > 0)
        {
            privateCopy(new_array, new_size, m_array, index);
            if (index < new_size)
            {
                privateCopy(new_array + index,
                            new_size - index,
                            m_array + index + 1,
                            m_size - index - 1);
            }
        }
        else
            privateCopy(new_array, new_size, m_array + 1, new_size);
        privateFree(m_array);
        m_array = new_array;
        m_size = new_size;
    }
    else
        Clear();
}

template<typename Type, typename SizeType>
void Array<Type, SizeType>::InsertAt(SizeType insert_pos, Type const& value)
{
    static std::string const msg("InsertAt(index, value)");
    privateIndexCheck(insert_pos, msg, true);

    Type* new_array;
    SizeType new_size = m_size + 1;
    privateAlloc(new_array, new_size);

    privateInsert(new_array, new_size, m_array, m_size, insert_pos, &value, 1);

    privateFree(m_array);
    m_array = new_array;
    m_size = new_size;
}

template<typename Type, typename SizeType>
void Array<Type, SizeType>::InsertAt(SizeType insert_pos,
                                     simple_array_type const& array_value)
{
    static std::string const msg("InsertAt(index, array_value)");
    privateIndexCheck(insert_pos, msg, true);

    Type* new_array;
    SizeType new_size = m_size + array_value.m_size;
    privateAlloc(new_array, new_size);

    privateInsert(new_array,
                  new_size,
                  m_array,
                  m_size,
                  insert_pos,
                  array_value.m_array,
                  array_value.m_size);

    privateFree(m_array);
    m_array = new_array;
    m_size = new_size;
}

template<typename Type, typename SizeType>
void Array<Type, SizeType>::privateIndexCheck(SizeType index,
                                              const std::string& arg,
                                              bool is_insert)
{
    if (is_insert)
    {
        // Insert position can be the same as m_size to indicate an append operation.
        if ((index < 0) || (index > m_size))
            throw std::out_of_range(arg);
    }
    else if ((index < 0) || (index >= m_size))
        throw std::out_of_range(arg);
}

template<typename Type, typename SizeType>
void Array<Type, SizeType>::privateCopy(Type* dest,
                                        SizeType dest_size,
                                        Type const* src,
                                        SizeType src_size) throw()
{
    // Copy up to src_size, but allocate more space if dest_size > src_size.
    // Ensure there are not too many src objects before copying.
    Type const* src_end = (dest_size < src_size) ? src + dest_size : src + src_size;
    while (src < src_end)
        *dest++ = *src++;
}

template<typename Type, typename SizeType>
void Array<Type, SizeType>::privateInsert(Type* dest,
                                          SizeType dest_size,
                                          Type const* src,
                                          SizeType src_size,
                                          SizeType insert_pos,
                                          Type const* value,
                                          SizeType value_size) throw()
{
    // This function expects the caller to ensure dest is big enough for src + value.

    if (src_size > 0)
    {
        if (insert_pos > 0)
        {
            // Copy everyting up to the insertion position.
            privateCopy(dest, dest_size, src, insert_pos);
            dest += insert_pos;
            dest_size -= insert_pos;
        }

        // Insert the value at the insert position.
        privateCopy(dest, dest_size, value, value_size);
        dest += value_size;
        dest_size -= value_size;

        // Copy the remaining values acroos.
        if (insert_pos < src_size)
            privateCopy(dest, dest_size, src + insert_pos, src_size - insert_pos);
    }
    else
        privateCopy(dest, dest_size, value, value_size);
}

template<typename Type, typename SizeType>
void Array<Type, SizeType>::privateAlloc(Type*& ptr, SizeType size)
{
    ptr = new Type[size];
}

template<typename Type, typename SizeType>
void Array<Type, SizeType>::privateFree(Type* ptr)
{
    delete [] ptr;
}

template<typename Type, typename SizeType>
void Array<Type, SizeType>::privateRealloc(Type*& ptr, SizeType size)
{
    privateFree(ptr);
    privateAlloc(ptr, size);
}
