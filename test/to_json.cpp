#define BOOST_TEST_MODULE to_json
#include <boost/test/included/unit_test.hpp>
#include <list>
#include <map>
#include <meta_json/to_json.h>
#include <string>
#include <vector>

using namespace std;

struct Flat {
  int a;
  bool b;
  string s;
  double d;
};

struct Nested {
  Flat f;
  std::string x;
};

struct WithOptional {
  std::optional<Flat> f;
  std::string x;
};

struct WithContainers {
  std::vector<double> d;
  std::map<std::string, double> m;
  std::map<std::string, Flat> n;
  std::list<Flat> f;
};

BOOST_HANA_ADAPT_STRUCT(Flat, a, b, s, d);
BOOST_HANA_ADAPT_STRUCT(Nested, f, x);
BOOST_HANA_ADAPT_STRUCT(WithOptional, f, x);
BOOST_HANA_ADAPT_STRUCT(WithContainers, d, m, n, f);

template <typename T> void checker(nlohmann::json &json, const T &t) {
  boost::hana::for_each(boost::hana::keys(t), [&json, &t](auto name) {
    auto const &member = boost::hana::at_key(t, name);
    using Member = std::decay_t<decltype(member)>;
    auto memberName = boost::hana::to<char const *>(name);

    if constexpr (boost::hana::Struct<Member>::value) {
      checker(json[memberName], member);
    } else if constexpr (meta_json::is_optional<Member>::value) {
      if (member) {
        checker(json[memberName], member.value());
      }
    } else {
      BOOST_CHECK(json[memberName] == member);
    }
  });
}

BOOST_AUTO_TEST_SUITE(struct_to_json)

BOOST_AUTO_TEST_CASE(flat) {
  Flat f{32, true, "hi", 42.42};

  auto json = meta_json::to_json(f);
  checker(json, f);
}

BOOST_AUTO_TEST_CASE(nested_structs) {
  Nested n{{32, true, "hello", 1.1}, "world"};

  auto json = meta_json::to_json(n);
  checker(json, n);
}

BOOST_AUTO_TEST_CASE(with_optional) {
  WithOptional o{Flat{12, false, "hello", 2.2}, "opt"};

  auto json = meta_json::to_json(o);
  checker(json, o);
}

BOOST_AUTO_TEST_CASE(with_containers) {
  WithContainers c{{1.0, 1.1, 1.2},
                   {{"a", 1.0}, {"b", 2.0}, {"c", 3.0}},
                   {
                      {"a", {12, true, "insert", 4.2}},
                      {"b", {13, false, "some", 2.4}},
                      {"c", {14, false, "message", 0.2}},
                   },
                   {
                       {12, false, "hello", 2.2},
                       {6, true, "world", 2.2},
                       {3, false, "!", 2.2},
                   }};

  auto json = meta_json::to_json(c);

  BOOST_CHECK(json["d"][1] == 1.1);
  BOOST_CHECK(json["m"]["c"] == 3.0);
  checker(json["n"]["b"], c.n["b"]);
  BOOST_CHECK(json["f"][0]["s"] == "hello");
}

BOOST_AUTO_TEST_SUITE_END()
