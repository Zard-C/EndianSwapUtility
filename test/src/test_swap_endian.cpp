#include <boost/hana.hpp>
#include <endian_swap_utility/endian_swap_utility.h>
#include <gtest/gtest.h>
#include <stdint.h>

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

struct FloatAndDoubleStruct
{
  float a;
  double b;
}
#if defined(__GNUC__) || defined(__clang__)
__attribute__((packed))
#endif
;

struct NestedFloadAndDoubleArrayStruct
{
  float a[2];
  double b[2];
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
BOOST_HANA_ADAPT_STRUCT(FloatAndDoubleStruct, a, b);
BOOST_HANA_ADAPT_STRUCT(NestedFloadAndDoubleArrayStruct, a, b);

static float little_to_big_endian(float f)
{
  static_assert(sizeof(float) == sizeof(uint32_t));
  uint32_t i = *(uint32_t*)&f;
  i = ((i & 0x0000ffff) << 16) | ((i & 0xffff0000) >> 16);
  i = ((i & 0x00ff00ff) << 8) | ((i & 0xff00ff00) >> 8);
  return *(float*)&i;
}

static double little_to_big_endian(double d)
{
  static_assert(sizeof(double) == sizeof(uint64_t));
  uint64_t i = *(uint64_t*)&d;
  i = ((i & 0x00000000ffffffff) << 32) | ((i & 0xffffffff00000000) >> 32);
  i = ((i & 0x0000ffff0000ffff) << 16) | ((i & 0xffff0000ffff0000) >> 16);
  i = ((i & 0x00ff00ff00ff00ff) << 8) | ((i & 0xff00ff00ff00ff00) >> 8);
  return *(double*)&i;
}

TEST(EndianSwapUtility, TestSwapEndianNestedStruct)
{
  ExampleStruct example_struct;
  example_struct.a = 0x12345678;
  example_struct.b = 0x1234;
  example_struct.c = 0x12;
  example_struct.nested_struct.a = 0x12345678;
  example_struct.nested_struct.b = 0x1234;
  example_struct.nested_struct.c = 0x12;
  example_struct.d = 0x12345678;
  example_struct.e = 0x1234;
  example_struct.f = 0x12;
  example_struct.nested_struct_array[0].a = 0x12345678;
  example_struct.nested_struct_array[0].b = 0x1234;
  example_struct.nested_struct_array[0].c = 0x12;
  example_struct.nested_struct_array[1].a = 0x12345678;
  example_struct.nested_struct_array[1].b = 0x1234;
  example_struct.nested_struct_array[1].c = 0x12;

  auto swapped_example_struct = endian_swap_utility::swap_endian(example_struct);

  EXPECT_EQ(swapped_example_struct.a, 0x78563412);
  EXPECT_EQ(swapped_example_struct.b, 0x3412);
  EXPECT_EQ(swapped_example_struct.c, 0x12);
  EXPECT_EQ(swapped_example_struct.nested_struct.a, 0x78563412);
  EXPECT_EQ(swapped_example_struct.nested_struct.b, 0x3412);
  EXPECT_EQ(swapped_example_struct.nested_struct.c, 0x12);
  EXPECT_EQ(swapped_example_struct.d, 0x78563412);
  EXPECT_EQ(swapped_example_struct.e, 0x3412);
  EXPECT_EQ(swapped_example_struct.f, 0x12);
  EXPECT_EQ(swapped_example_struct.nested_struct_array[0].a, 0x78563412);
  EXPECT_EQ(swapped_example_struct.nested_struct_array[0].b, 0x3412);
  EXPECT_EQ(swapped_example_struct.nested_struct_array[0].c, 0x12);
  EXPECT_EQ(swapped_example_struct.nested_struct_array[1].a, 0x78563412);
  EXPECT_EQ(swapped_example_struct.nested_struct_array[1].b, 0x3412);
  EXPECT_EQ(swapped_example_struct.nested_struct_array[1].c, 0x12);
}

TEST(EndianSwapUtility, TestSwapEndianFloatAndDouble)
{
  FloatAndDoubleStruct float_and_double_struct;
  float_and_double_struct.a = 1.0f;
  float_and_double_struct.b = 2.0;

  auto swapped_float_and_double_struct = endian_swap_utility::swap_endian(float_and_double_struct);

  EXPECT_EQ(swapped_float_and_double_struct.a, little_to_big_endian(1.0f));
  EXPECT_EQ(swapped_float_and_double_struct.b, little_to_big_endian(2.0));
}

TEST(EndianSwapUtility, TestSwapEndianNestedFloatAndDoubleArray)
{
  NestedFloadAndDoubleArrayStruct nested_float_and_double_array_struct;
  nested_float_and_double_array_struct.a[0] = 1.0f;
  nested_float_and_double_array_struct.a[1] = 2.0f;
  nested_float_and_double_array_struct.b[0] = 3.0;
  nested_float_and_double_array_struct.b[1] = 4.0;

  auto swapped_nested_float_and_double_array_struct =
      endian_swap_utility::swap_endian(nested_float_and_double_array_struct);

  EXPECT_EQ(swapped_nested_float_and_double_array_struct.a[0], little_to_big_endian(1.0f));
  EXPECT_EQ(swapped_nested_float_and_double_array_struct.a[1], little_to_big_endian(2.0f));
  EXPECT_EQ(swapped_nested_float_and_double_array_struct.b[0], little_to_big_endian(3.0));
  EXPECT_EQ(swapped_nested_float_and_double_array_struct.b[1], little_to_big_endian(4.0));
}