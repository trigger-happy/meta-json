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
  nlohmann::json j = {
    {"d", {1.1, 2.2, 3.3}},
    {"m", {}},
    {"n", {}},
    {"f", {}}
  };

  auto c = meta_json::from_json<WithContainers>(j);

  BOOST_CHECK(c.d[0] == 1.1);
}

BOOST_AUTO_TEST_SUITE_END()
