# EndianSwapUtility

A utility for easy c-style struct endianess conversion. Depends on boost::hana and C++17.

## Dependencies

- [Boost.Hana]
- [C++17]

[Boost.Hana]: https://www.boost.org/doc/libs/1_66_0/libs/hana/doc/html/index.html
[C++17]: https://en.wikipedia.org/wiki/C%2B%2B17

## Restrictions

- Only supports structs with POD types.
- Only supports structs with no padding.
- Only supports structs with no bitfields.

## Usage

```cpp
#include <endian_swap_utility.hpp>

#pragma pack(1)
struct MyStruct {
    int a;
    float b;
    double c;
};
#pragma pack()

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

## TODO

- [ ] Add unit test for `swap_endian` function. Currently only tested manually.
