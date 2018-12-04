# Meta JSON

## What is this?

A set of header only libraries that make it as DRY as possible to convert
between JSON strings and C++ structs.

The library depends on [nlohmann-json][1] and [boost-hana][2] to do the heavy lifting.

## Installation

Install either by using the [conan package manager][3] and searching for
`meta-json`

```bash
conan search meta-json -r all
conan install meta-json/1.0.0@trigger-happy/testing --build=missing
```

Or by adding this repository as a submodule in your own project and telling
your project's `CMakeLists.txt` to include the library into your own project by
using the `add_subdirectory` command.

When building your targets, make sure you add this library as as part of the
`target_link_libraries` command.

```cmake
find_package(meta_json 1.0.0 REQUIRED)

add_executable(something something.cpp)
target_link_libraries(something meta_json::meta_json)
```

## Usage

The library can only convert between json strings and structs if said structs
have been adapted to boost hana using the `BOOST_HANA_ADAPT_STRUCT` macro and
its other similar macros.

```cpp
#include <meta_json/from_json.h>
#include <cassert>

struct Flat {
  int a;
  bool b;
  std::string s;
  double d;
};

BOOST_HANA_ADAPT_STRUCT(Flat, a, b, s, d);

int main() {
  std::string json = R"({"a":32,"b":true,"d":2.2,"s":"hello"})";
  auto f = meta_json::from_json<Flat>(json);

  assert(f.a == 32);
  assert(f.b == true);
  assert(f.s == "hello");
  assert(f.d == 2.2);

  return 0;
}
```


[1]: https://github.com/nlohmann/json
[2]: https://www.boost.org/doc/libs/1_61_0/libs/hana/doc/html/index.html
[3]: https://conan.io/
