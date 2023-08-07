#ifndef __ENDIAN_SWAP_UTILITY_EXAMPLE_H__
#define __ENDIAN_SWAP_UTILITY_EXAMPLE_H__

#include <stdint.h>
#include <boost/hana.hpp>
/*
 * This example shows how to use endian_swap_utility.hpp
 * to swap the endian of a struct with nested struct and array.
 * Restriction:
 *    Structs are assumed to be packed with no padding(#pragma pack(1) or __attribute__((packed))
 *    Structs (include nested_structs) do not have any pointer members.
 *    Nested struct arrays are assumed to be fixed size.
 */

#if defined(_MSC_VER)
#pragma pack(push, 1)
#endif
struct NestedStruct
{
  uint32_t a;
  uint16_t b;
  uint8_t c;
}
#if defined(__GNUC__) || defined(__clang__)
__attribute__((packed))
#endif
;

/*
 * Using boost::hana::accessors to get all member types of a struct.
 */

struct ExampleStruct
{
  uint32_t a;
  uint16_t b;
  uint8_t c;
  NestedStruct nested_struct;
  uint32_t d;
  uint16_t e;
  uint8_t f;
  NestedStruct nested_struct_array[2];
}
#if defined(__GNUC__) || defined(__clang__)
__attribute__((packed))
#endif
;

#if defined(_MSC_VER)
#pragma pack(pop)
#endif


BOOST_HANA_ADAPT_STRUCT(NestedStruct, a, b, c);
BOOST_HANA_ADAPT_STRUCT(ExampleStruct, a, b, c, nested_struct, d, e, f, nested_struct_array);

#endif  // __ENDIAN_SWAP_UTILITY_EXAMPLE_H__