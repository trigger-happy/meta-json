#define BOOST_TEST_MODULE from_json
#include "shared_types.h"
#include <boost/test/included/unit_test.hpp>
#include <meta_json/from_json.h>
#include <boost/hana.hpp>

struct SmallTest {
  std::vector<double> d;
};

BOOST_HANA_ADAPT_STRUCT(SmallTest, d);

BOOST_AUTO_TEST_SUITE(from_json)

BOOST_AUTO_TEST_CASE(flat) {
  nlohmann::json j = {
    {"a", 32},
    {"b", true},
    {"s", "hello"},
    {"d", 2.2}
  };
  std::string jsonString = j.dump();

  {
    auto f = meta_json::from_json<Flat>(j);
    BOOST_CHECK(f.a == 32);
    BOOST_CHECK(f.b == true);
    BOOST_CHECK(f.s == "hello");
    BOOST_CHECK(f.d == 2.2);
  }
  {
    auto f = meta_json::from_json<Flat>(jsonString);
    BOOST_CHECK(f.a == 32);
    BOOST_CHECK(f.b == true);
    BOOST_CHECK(f.s == "hello");
    BOOST_CHECK(f.d == 2.2);
  }
}

BOOST_AUTO_TEST_CASE(nested_structs) {
  nlohmann::json j = {
    {"f", {
            {"a", 32},
            {"b", true},
            {"s", "hello"},
            {"d", 2.2}
          },
    },
    {"x", "hi"}
  };

  auto n = meta_json::from_json<Nested>(j);
  auto const &f = n.f;
  BOOST_CHECK(f.a == 32);
  BOOST_CHECK(f.b == true);
  BOOST_CHECK(f.s == "hello");
  BOOST_CHECK(f.d == 2.2);
  BOOST_CHECK(n.x == "hi");
}

BOOST_AUTO_TEST_CASE(with_optional) {
  {
    nlohmann::json j = {{"x", "optional"}};

    auto o = meta_json::from_json<WithOptional>(j);

    BOOST_CHECK(o.x == "optional");
    BOOST_CHECK(o.f == std::nullopt);
  }

  {
    nlohmann::json j = {
        {
            "f",
            {{"a", 32}, {"b", true}, {"s", "hello"}, {"d", 2.2}},
        },
        {"x", "optional"}};

    auto o = meta_json::from_json<WithOptional>(j);
    auto const &f = o.f.value();

    BOOST_CHECK(o.x == "optional");
    BOOST_CHECK(f.a == 32);
    BOOST_CHECK(f.b == true);
    BOOST_CHECK(f.s == "hello");
    BOOST_CHECK(f.d == 2.2);
  }
}

BOOST_AUTO_TEST_CASE(with_containers) {
  nlohmann::json j = R"({
    "d": [
      1.0,
      1.1,
      1.2
    ],
    "f": [
      {
        "a": 12,
        "b": false,
        "d": 2.2,
        "s": "hello"
      },
      {
        "a": 6,
        "b": true,
        "d": 2.2,
        "s": "world"
      },
      {
        "a": 3,
        "b": false,
        "d": 2.2,
        "s": "!"
      }
    ],
    "m": {
      "a": 1.0,
      "b": 2.0,
      "c": 3.0
    },
    "n": {
      "a": {
        "a": 12,
        "b": true,
        "d": 4.2,
        "s": "insert"
      },
      "b": {
        "a": 13,
        "b": false,
        "d": 2.4,
        "s": "some"
      },
      "c": {
        "a": 14,
        "b": false,
        "d": 0.2,
        "s": "message"
      }
    }
  })"_json;

  auto c = meta_json::from_json<WithContainers>(j);

  BOOST_CHECK(c.d[0] == 1.0);

  BOOST_CHECK(c.m["a"] == 1.0);
  BOOST_CHECK(c.m["b"] == 2.0);
  BOOST_CHECK(c.m["c"] == 3.0);

  Flat f{12, true, "insert", 4.2};
  BOOST_CHECK(c.n["a"].a == f.a);
  BOOST_CHECK(c.n["a"].s == f.s);

  BOOST_CHECK((c.f.begin())->a == 12);
}

BOOST_AUTO_TEST_SUITE_END()
