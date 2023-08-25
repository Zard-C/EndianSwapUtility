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

// POD is a C-style struct
// A non-member function C++ struct is considered as POD.
// "class xx{public: ...};" equals "struct xx{...};"

class Podclass{
public:
  int a;
  float b;
  double c;
}
#if defined(__GNUC__) || defined(__clang__)
__attribute__((packed))
#endif
;

class NonPodclass{
public:
  NonPodclass(){}
  int a;
  float b;
  double c;
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
BOOST_HANA_ADAPT_STRUCT(Podclass, a, b, c);
BOOST_HANA_ADAPT_STRUCT(double);
BOOST_HANA_ADAPT_STRUCT(double[5]);




static float endian_swap(float f)
{
  static_assert(sizeof(float) == sizeof(uint32_t));
  uint32_t i = *(uint32_t*)&f;
  i = ((i & 0x0000ffff) << 16) | ((i & 0xffff0000) >> 16);
  i = ((i & 0x00ff00ff) << 8) | ((i & 0xff00ff00) >> 8);
  return *(float*)&i;
}

static double endian_swap(double d)
{
  static_assert(sizeof(double) == sizeof(uint64_t));
  uint64_t i = *(uint64_t*)&d;
  i = ((i & 0x00000000ffffffff) << 32) | ((i & 0xffffffff00000000) >> 32);
  i = ((i & 0x0000ffff0000ffff) << 16) | ((i & 0xffff0000ffff0000) >> 16);
  i = ((i & 0x00ff00ff00ff00ff) << 8) | ((i & 0xff00ff00ff00ff00) >> 8);
  return *(double*)&i;
}

template<typename T>T auto_swap(T t){
    for(uint8_t pivot = 0; pivot < sizeof(t)/2 && pivot < sizeof(t) - 1 - pivot; pivot ++){
        *((uint8_t *)&t + pivot) ^= *((uint8_t *)&t+sizeof(t)-1- pivot);
        *((uint8_t *)&t+sizeof(t)-1- pivot) ^= *((uint8_t *)&t + pivot);
        *((uint8_t *)&t + pivot) ^= *((uint8_t *)&t+sizeof(t)-1- pivot);
    }
    return t;
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

  EXPECT_EQ(swapped_float_and_double_struct.a, endian_swap(1.0f));
  EXPECT_EQ(swapped_float_and_double_struct.b, endian_swap(2.0));
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

  EXPECT_EQ(swapped_nested_float_and_double_array_struct.a[0], endian_swap(1.0f));
  EXPECT_EQ(swapped_nested_float_and_double_array_struct.a[1], endian_swap(2.0f));
  EXPECT_EQ(swapped_nested_float_and_double_array_struct.b[0], endian_swap(3.0));
  EXPECT_EQ(swapped_nested_float_and_double_array_struct.b[1], endian_swap(4.0));
}

TEST(EndianSwapUtility, TestPodClass)
{
  
  EXPECT_TRUE(std::is_trivial<Podclass>::value && std::is_standard_layout<Podclass>::value);
  EXPECT_FALSE(std::is_trivial<NonPodclass>::value && std::is_standard_layout<NonPodclass>::value);

  Podclass pc{1, 2.0f, 3.0};
  ASSERT_EQ(sizeof(pc), sizeof(int) + sizeof(float) + sizeof(double));
  ASSERT_EQ(sizeof(float), 4);

  auto endianswap = endian_swap_utility::swap_endian(pc);
  EXPECT_EQ(endianswap.b, endian_swap(2.0f));
  EXPECT_EQ(endianswap.c, endian_swap(3.0));
  uint8_t a = 0x12;
  uint16_t b = 0x1234, c = 0x3412;
  EXPECT_EQ(a,  auto_swap(a));
  EXPECT_EQ(b,  auto_swap(c));
  EXPECT_EQ(0x12345678,  auto_swap(0x78563412));
  EXPECT_EQ(0x1234567890abcdefL, auto_swap(0xefcdab9078563412L));

  EXPECT_EQ(endianswap.a, auto_swap(pc.a));

  EXPECT_EQ(endianswap.b, auto_swap(pc.b));

  EXPECT_EQ(endianswap.c, auto_swap(pc.c));

}

TEST(EndianSwapUtility, TestPod)
{
  double a = 3.1415926;
  auto endianswap = endian_swap_utility::swap_endian(a);
  EXPECT_EQ(endianswap, auto_swap(a));
}

TEST(EndianSwapUtility, TestPodArr)
{
  double a[5] = {3.1415926, 23.23, 66.12, 891.2, 594320349.9320};
  auto endianswap = endian_swap_utility::swap_endian_arr(a);
  for(int i = 0; i < 5; i++)
  {
    EXPECT_EQ(endianswap[i], auto_swap(a[i]));
  }
  free(endianswap);
  
}


