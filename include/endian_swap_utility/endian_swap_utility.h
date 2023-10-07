#ifndef __ENDIAN_SWAP_UTILITY_H__
#define __ENDIAN_SWAP_UTILITY_H__
#include <boost/hana.hpp>
#include <vector>
#include <algorithm>
#include <numeric>

namespace endian_swap_utility
{
template <typename T>
auto get_member_types(T object)
{
  namespace hana = boost::hana;
  auto members = hana::transform(hana::accessors<T>(), hana::second);
  return hana::transform(members, [&object](auto member) {
    using NonReferenceType = std::remove_reference_t<decltype(member(object))>;
    return hana::type_c<NonReferenceType>;
  });
}

template <typename T>
void generate_swap_mapping(std::vector<int>& transform_matrix, size_t& offset)
{
  namespace hana = boost::hana;
  auto types = get_member_types(T{});
  hana::for_each(types, [&transform_matrix, &offset](auto t) {
    using type = typename decltype(t)::type;
    if constexpr (std::is_fundamental_v<type>)
    {
      auto it = transform_matrix.begin() + offset;
      std::reverse(it, it + sizeof(type));
      offset += sizeof(type);
    }
    else if constexpr (std::is_array_v<type>)
    {
      constexpr size_t num_of_elems = std::extent_v<type>;
      // get array element type
      using element_type = std::remove_all_extents_t<type>;
      if constexpr (std::is_fundamental_v<element_type>)
      {
        // get the num of elems of array
        for (size_t i = 0; i < num_of_elems; ++i)
        {
          auto it = transform_matrix.begin() + offset;
          std::reverse(it, it + sizeof(element_type));
          offset += sizeof(element_type);
        }
      }
      else
      {
        // parsing array of struct
        for (size_t i = 0; i < num_of_elems; ++i)
        {
          generate_swap_mapping<element_type>(transform_matrix, offset);
        }
      }
    }
    else if constexpr (std::is_class_v<type>)
    {
      // parsing struct
      generate_swap_mapping<type>(transform_matrix, offset);
    }
    else
    {
      static_assert(std::is_fundamental_v<type> || std::is_array_v<type> || std::is_class_v<type>,
                    "not supported type ");
    }
  });
}

// specialize for fundamental type
template <typename T>
typename std::enable_if<std::is_fundamental<T>::value, T>::type swap_endian(const T& value)
{
  if constexpr (sizeof(T) == 1)
  {
    return value;
  }
  T result;
  auto value_ptr = reinterpret_cast<const char*>(&value);
  auto result_ptr = reinterpret_cast<char*>(&result);
  for (size_t i = 0; i < sizeof(T); ++i)
  {
    result_ptr[i] = value_ptr[sizeof(T) - i - 1];
  }
  return result;
}

// specialize for array of fundamental type c-style array
template <typename T>
typename std::enable_if<std::is_array<T>::value && std::is_fundamental<std::remove_extent_t<T>>::value,
                        std::array<std::remove_extent_t<T>, std::extent_v<T>>>::type
swap_endian(const T& value)
{
  using element_type = std::remove_extent_t<T>;
  constexpr size_t num_of_elems = std::extent_v<T>;
  std::array<element_type, num_of_elems> result;

  if constexpr (sizeof(element_type) == 1)
  {
    for (size_t i = 0; i < num_of_elems; ++i)
    {
      result[i] = value[i];
    }
    return result;
  }

  for (size_t i = 0; i < num_of_elems; ++i)
  {
    result[i] = swap_endian(value[i]);
  }
  return result;
}

template <typename T>
typename std::enable_if<!std::is_fundamental<T>::value, T>::type swap_endian(const T& value)
{
  // generate transform matrix
  std::vector<int> transform_matrix(sizeof(T));
  std::iota(transform_matrix.begin(), transform_matrix.end(), 0);
  size_t offset = 0;
  generate_swap_mapping<T>(transform_matrix, offset);
  T result;

  auto value_ptr = reinterpret_cast<const char*>(&value);
  auto result_ptr = reinterpret_cast<char*>(&result);
  for (size_t i = 0; i < sizeof(T); ++i)
  {
    result_ptr[i] = value_ptr[transform_matrix[i]];
  }

  return result;
}

}  // namespace endian_swap_utility
#endif  // __ENDIAN_SWAP_UTILITY_H__
