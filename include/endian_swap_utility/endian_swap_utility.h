#ifndef __ENDIAN_SWAP_UTILITY_H__
#define __ENDIAN_SWAP_UTILITY_H__
#include <boost/hana.hpp>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cstring>

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
    if constexpr (std::is_fundamental_v<T>)
    {
      auto it = transform_matrix.begin() + offset;
      std::reverse(it, it + sizeof(T));
      offset += sizeof(T);
    }
    else if constexpr (std::is_array_v<T>)
    {
      constexpr size_t num_of_elems = std::extent_v<T>;
      // get array element T
      using element_type = std::remove_all_extents_t<T>;
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
    else if constexpr (std::is_class_v<T>)
    {
      // parsing struct
      namespace hana = boost::hana;
      auto types = get_member_types(T{});
      hana::for_each(types, [&transform_matrix, &offset](auto t) {
        using member_t = typename decltype(t)::type;
        generate_swap_mapping<member_t>(transform_matrix, offset);
      });
    }
    else
    {
      static_assert(std::is_fundamental_v<T> || std::is_array_v<T> || std::is_class_v<T>,
                    "not supported type ");
    }
  
}

template <typename T>
T swap_endian(const T& value)
{
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

// c-style array
template <typename T, int N>
T* swap_endian_arr(const T (&value)[N])
{
  std::vector<int> transform_matrix(sizeof(T) * N);
  std::iota(transform_matrix.begin(), transform_matrix.end(), 0);
  size_t offset = 0;
  for(auto i = 0; i < N; i++)
  generate_swap_mapping<T>(transform_matrix, offset);
  T *result = reinterpret_cast<T *>(malloc(sizeof(T) * N));
  memset(result, 0, sizeof(T) * N);

  auto value_ptr = reinterpret_cast<const char*>(value);
  auto result_ptr = reinterpret_cast<char*>(result);
  for (size_t i = 0; i < N * sizeof(T); ++i)
  {
    result_ptr[i] = value_ptr[transform_matrix[i]];
  }

  return result;
}

}  // namespace endian_swap_utility
#endif  // __ENDIAN_SWAP_UTILITY_H__
