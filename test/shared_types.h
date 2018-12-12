#ifndef SHARED_TYPES_H
#define SHARED_TYPES_H

#include <boost/hana.hpp>
#include <list>
#include <map>
#include <string>
#include <vector>

struct Flat {
  int a;
  bool b;
  std::string s;
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

struct NestedContainers {
  std::vector<std::vector<double>> d;
};

BOOST_HANA_ADAPT_STRUCT(Flat, a, b, s, d);
BOOST_HANA_ADAPT_STRUCT(Nested, f, x);
BOOST_HANA_ADAPT_STRUCT(WithOptional, f, x);
BOOST_HANA_ADAPT_STRUCT(WithContainers, d, m, n, f);
BOOST_HANA_ADAPT_STRUCT(NestedContainers, d);

#endif // SHARED_TYPES_H

