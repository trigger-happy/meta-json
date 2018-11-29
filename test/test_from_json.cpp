#define BOOST_TEST_MODULE from_json
#include "shared_types.h"
#include <boost/test/included/unit_test.hpp>
#include <meta_json/from_json.h>

BOOST_AUTO_TEST_SUITE(from_json)

BOOST_AUTO_TEST_CASE(flat) {
  nlohmann::json j = {
    {"a", 32},
    {"b", true},
    {"s", "hello"},
    {"d", 2.2}
  };

  auto f = meta_json::from_json<Flat>(j);
  BOOST_CHECK(f.a == 32);
  BOOST_CHECK(f.b == true);
  BOOST_CHECK(f.s == "hello");
  BOOST_CHECK(f.d == 2.2);
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
}

BOOST_AUTO_TEST_SUITE_END()
