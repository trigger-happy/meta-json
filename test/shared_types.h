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

struct Paired {
  std::pair<std::string, int> p;
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
BOOST_HANA_ADAPT_STRUCT(Paired, p);
BOOST_HANA_ADAPT_STRUCT(WithOptional, f, x);
BOOST_HANA_ADAPT_STRUCT(WithContainers, d, m, n, f);

#endif // SHARED_TYPES_H

