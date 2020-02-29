---
title: Meta JSON Reference

language_tabs:
  - cpp

search: true
---

# Introduction

A set of header only libraries that make it as DRY as possible to convert
between JSON strings and C++ structs.

The library depends on [nlohmann-json](https://github.com/nlohmann/json)
and [boost-hana](https://www.boost.org/doc/libs/1_72_0/libs/hana/doc/html/index.html)
to do the heavy lifting.

## Caveat

Compilation times will increase significantly when this library is used due to
the extensive use of C++ templates and `constexpr` statements. If the
increase in compilation times is unacceptable, then consider not using this
library in your project.

# Installation

You can use this library by adding it as a submodule in your own project and telling
your project's `CMakeLists.txt` to include the library into your own project by
using the `add_subdirectory` command.

When building your targets, make sure you add this library as as part of the
`target_link_libraries` command.

```cmake
find_package(meta_json 1.0.0 REQUIRED)

add_executable(something something.cpp)
target_link_libraries(something meta_json::meta_json)
```

# Converting to json

You can convert a a Hana adapted class/struct to a json string by using the
`meta_json::to_json` function.

> Converting to json

```cpp
#include <meta_json/to_json.h>
#include <cassert>

struct Flat {
  int a;
  bool b;
  std::string s;
  double d;
};

BOOST_HANA_ADAPT_STRUCT(Flat, a, b, s, d);

int main() {
  Flat f{32, true, "hi", 42.42};
  auto s = meta_json::to_json(f);

  std::string json = R"({"a":32,"b":true,"d":2.2,"s":"hello"})";
  assert(s == json);

  return 0;
}
```

# Converting from json

You can convert a json string into a Hana adapted class/struct by using the
`meta_json::from_json` function.

> Converting from json

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
