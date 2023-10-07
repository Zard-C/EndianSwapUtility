# EndianSwapUtility

A utility for easy c-style struct endianess conversion. Depends on boost::hana(use reflection in cpp) and C++17.

## Dependencies

- [Boost.Hana]
- [C++17]

[Boost.Hana]: https://www.boost.org/doc/libs/1_66_0/libs/hana/doc/html/index.html
[C++17]: https://en.wikipedia.org/wiki/C%2B%2B17

## Supported Data Types

- [x] nested structs with POD types and no padding
- [x] c-style `nested structs with POD types and no padding` arrays
- [x] fundamental types
- [x] c-style fundamental array
- [ ] std::array<T, N>

## Usage

```cpp
#include <endian_swap_utility.hpp>

#if defined(_MSC_VER)
#pragma pack(push, 1)
#endif
struct MyStruct {
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

int main() {
    MyStruct my_struct{1, 2.0f, 3.0};
    auto swapped = endian_swap_utility::swap_endian(my_struct);
    return 0;
}
```

## Example

See  [Example](./example/src/example.cpp) for a more complete example.

## Testing

```bash
cd EndianSwapUtility
mkdir build
cd build
cmake .. -DENABLE_TESTING=ON
make
make test
```

Run example with `./example/example`.
