#ifndef OCL_SIMPLEARRAY_HPP
#define OCL_SIMPLEARRAY_HPP

#include <cstddef>
#include <stdexcept>

/**
 * Functions are capable of throwing a std::bad_alloc exception when changing
 * the array contents.
 */
namespace ocl
{

template<typename Type, typename SizeType = size_t>
class Array
{
public:
    typedef Type type;
    typedef SizeType size_type;
    typedef Array<Type, SizeType> simple_array_type;

public:
    Array() throw();
    Array(SizeType initial_size);
    Array(simple_array_type const& array_value);

    /// Non-virtual destructor, as Array is not designed as a base class.
    ~Array();

public:
    simple_array_type& operator =(simple_array_type const& rhs);

    /// Note these subscript functions can throw std::out_of_range
    /// when the index is invalid.
    Type const& operator[](SizeType index) const;
    Type& operator[](SizeType index);

    /// Same as Append function.
    simple_array_type& operator+=(Type const& value);
    simple_array_type& operator+=(simple_array_type const& array_value);

    bool operator==(simple_array_type const& rhs) const throw();
    bool operator!=(simple_array_type const& rhs) const throw();

public:
    /// Return true when the array is empty, i.e. size is zero.
    bool IsEmpty() const throw();

    /// Get the number of elements in the array.
    SizeType GetSize() const throw();

    /// Free the internal memory and set to null.
    /// Ptr will return null when this is empty.
    void Clear();

    /// Get the internal pointer storing the actual data.
    Type const* Ptr() const throw();
    Type* Ptr() throw();

    /// Copy array_value, replacing any existing contents.
    void Copy(simple_array_type const& array_value);

    /// Replace this object with array_value, freeing any existing contents.
    void Move(simple_array_type& array_value);

    void Append(Type const& value);
    void Append(simple_array_type const& array_value);

    /// Deletes an element at index, unless the index is invalid
    /// then std::out_of_range is thrown.
    void DeleteAt(SizeType index);

    /// Inserts an element at index, unless the index is invalid
    /// then std::out_of_range is thrown.
    void InsertAt(SizeType insert_pos, Type const& value);
    void InsertAt(SizeType insert_pos, simple_array_type const& value);

private:
    // Check that index is valid or throw std::out_of_range.
    void privateIndexCheck(SizeType index,
                           const std::string& arg,
                           bool is_insert = false);

// copy/insert helpers.
private:
    // Copy contents from src to dest.
    static void privateCopy(Type* dest,
                            SizeType dest_size,
                            Type const* src,
                            SizeType src_size) throw();

    // Insert value into src and store result in dest.
    // NOTE: dest_size must be greater or equal than src_size + value_size.
    static void privateInsert(Type* dest,
                              SizeType dest_size,
                              Type const* src,
                              SizeType src_size,
                              SizeType insert_pos,
                              Type const* value,
                              SizeType value_size) throw();

// Allocate and de-allocate memory helpers.
private:
    // Safety net for ensuring consistent allocation.
    static void privateAlloc(Type*& ptr, SizeType size);

    // Safety net for ensuring consistent delete.
    static void privateFree(Type* ptr);

    // Delete and allocate ptr with a new size.
    static void privateRealloc(Type*& ptr, SizeType size);

private:
    SizeType m_size;
    Type* m_array;
};

#include "Array.inl"

} // namespace ocl

#endif // OCL_SIMPLEARRAY_HPP

