# Lake

**Lake** is a work-in-progress C++ *base library* aiming to provide basic functionalities like containers, iterators, strings, smart pointers etc.

It is primarily an exercise of implementing things commonly contained in standard libraries.
It should not be used in production code, as it is certainly not bug-free, and most probably also not very performant.

# Features

* primitive fixed-width types (`u8`, `u16`, ...)
* spans and iterators (with `constexpr`)
* fixed-size arrays
* optional values
* owning smart pointers

## TODO
* fixed-size vectors (without compile-time known size, `constexpr`)
* static inline vectors (with maximum capacity)
* hash table and hash map
* reference-counted pointers
* strings

# Build and Install

## Dependencies

This project requires CMake. It is recommended to also use the Ninja generator.

## Build

```console
mkdir build
cd build
cmake -G Ninja ..
ninja -j
```

# Test

Run unit tests using the following command (in `build/`):

```console
ninja && ninja test
```

## Install

TODO

# License

This software is released under the terms of the MIT License.
See the [LICENSE file](./LICENSE) for further information.
