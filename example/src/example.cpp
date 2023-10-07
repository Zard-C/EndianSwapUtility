#include <iostream>
#include <endian_swap_utility/endian_swap_utility.h>
#include <example.h>


int main()
{
  // create a struct with nested struct and array
  ExampleStruct example_struct = { 0x12345678, 0x9ABC,
                                   0xDE,       { 0x12345678, 0x9ABC, 0xDE },
                                   0x12345678, 0x9ABC,
                                   0xDE,       { { 0x12345678, 0x9ABC, 0xDE }, { 0x12345678, 0x9ABC, 0xDE } } };

  // swap the endian of the struct
  auto swapped_example_struct = endian_swap_utility::swap_endian(example_struct);

  // print the example_struct
  std::cout << "example_struct.a: " << std::hex << example_struct.a << std::endl;
  std::cout << "example_struct.b: " << std::hex << example_struct.b << std::endl;
  std::cout << "example_struct.c: " << std::hex << static_cast<int>(example_struct.c) << std::endl;
  std::cout << "example_struct.nested_struct.a: " << std::hex << example_struct.nested_struct.a << std::endl;
  std::cout << "example_struct.nested_struct.b: " << std::hex << example_struct.nested_struct.b << std::endl;
  std::cout << "example_struct.nested_struct.c: " << std::hex << static_cast<int>(example_struct.nested_struct.c)
            << std::endl;
  std::cout << "example_struct.d: " << std::hex << example_struct.d << std::endl;
  std::cout << "example_struct.e: " << std::hex << example_struct.e << std::endl;
  std::cout << "example_struct.f: " << std::hex << static_cast<int>(example_struct.f) << std::endl;
  std::cout << "example_struct.nested_struct_array[0].a: " << std::hex << example_struct.nested_struct_array[0].a
            << std::endl;
  std::cout << "example_struct.nested_struct_array[0].b: " << std::hex << example_struct.nested_struct_array[0].b
            << std::endl;
  std::cout << "example_struct.nested_struct_array[0].c: " << std::hex
            << static_cast<int>(example_struct.nested_struct_array[0].c) << std::endl;
  std::cout << "example_struct.nested_struct_array[1].a: " << std::hex << example_struct.nested_struct_array[1].a
            << std::endl;
  std::cout << "example_struct.nested_struct_array[1].b: " << std::hex << example_struct.nested_struct_array[1].b
            << std::endl;
  std::cout << "example_struct.nested_struct_array[1].c: " << std::hex
            << static_cast<int>(example_struct.nested_struct_array[1].c) << std::endl;

  // print a split line
  std::cout << "----------------------------------------" << std::endl;

  // print the swapped_example_struct
  std::cout << "swapped_example_struct.a: " << std::hex << swapped_example_struct.a << std::endl;
  std::cout << "swapped_example_struct.b: " << std::hex << swapped_example_struct.b << std::endl;
  std::cout << "swapped_example_struct.c: " << std::hex << static_cast<int>(swapped_example_struct.c) << std::endl;
  std::cout << "swapped_example_struct.nested_struct.a: " << std::hex << swapped_example_struct.nested_struct.a
            << std::endl;
  std::cout << "swapped_example_struct.nested_struct.b: " << std::hex << swapped_example_struct.nested_struct.b

            << std::endl;
  std::cout << "swapped_example_struct.nested_struct.c: " << std::hex

            << static_cast<int>(swapped_example_struct.nested_struct.c) << std::endl;
  std::cout << "swapped_example_struct.d: " << std::hex << swapped_example_struct.d << std::endl;
  std::cout << "swapped_example_struct.e: " << std::hex << swapped_example_struct.e << std::endl;
  std::cout << "swapped_example_struct.f: " << std::hex << static_cast<int>(swapped_example_struct.f) << std::endl;
  std::cout << "swapped_example_struct.nested_struct_array[0].a: " << std::hex
            << swapped_example_struct.nested_struct_array[0].a << std::endl;
  std::cout << "swapped_example_struct.nested_struct_array[0].b: " << std::hex
            << swapped_example_struct.nested_struct_array[0].b << std::endl;
  std::cout << "swapped_example_struct.nested_struct_array[0].c: " << std::hex
            << static_cast<int>(swapped_example_struct.nested_struct_array[0].c) << std::endl;
  std::cout << "swapped_example_struct.nested_struct_array[1].a: " << std::hex
            << swapped_example_struct.nested_struct_array[1].a << std::endl;
  std::cout << "swapped_example_struct.nested_struct_array[1].b: " << std::hex
            << swapped_example_struct.nested_struct_array[1].b << std::endl;
  std::cout << "swapped_example_struct.nested_struct_array[1].c: " << std::hex
            << static_cast<int>(swapped_example_struct.nested_struct_array[1].c) << std::endl;


  int single_int = 0x12345678;
  std::cout << "single_int: " << std::hex << single_int << std::endl;
  std::cout << "swapped_single_int: " << std::hex << endian_swap_utility::swap_endian(single_int) << std::endl;

  uint32_t fundamental_array[2] = { 0x01234567, 0x89ABCDEF};
  std::cout << "fundamental_array[0]: " << std::hex << fundamental_array[0] << std::endl;
  std::cout << "fundamental_array[1]: " << std::hex << fundamental_array[1] << std::endl;
  auto swapped_fundamental_array =  endian_swap_utility::swap_endian(fundamental_array);
  std::cout << "swapped_fundamental_array[0]: " << std::hex << swapped_fundamental_array[0] << std::endl;
  std::cout << "swapped_fundamental_array[1]: " << std::hex << swapped_fundamental_array[1] << std::endl;

  return 0;
}
