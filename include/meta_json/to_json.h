#ifndef TO_JSON_H
#define TO_JSON_H

#include "container_type_traits.h"
#include <boost/hana.hpp>
#include <iostream>
#include <string>
#include <numeric>
#include <algorithm>
#include <nlohmann/json.hpp>

namespace meta_json {

// forward declaration for serializing hana adapted/defined structs
template<typename T>
std::enable_if_t<boost::hana::Struct<T>::value, nlohmann::json> to_json(T const& s);

// fundamental value or string serialization
template<typename T>
std::enable_if_t<std::is_fundamental<T>::value
                || is_std_string<T>::value, nlohmann::json> to_json(T const &s) {
  nlohmann::json json;
  json = s;
  return json;
}

// specifically for std::pairs
template<typename T, typename U, template<class...> class C>
std::enable_if_t<std::is_same<C<T, U>, std::pair<T, U>>::value, nlohmann::json> to_json(C<T, U> const& s) {
  auto json = nlohmann::json::object();
  json[s.first] = s.second;

  return json;
}

// for STL containers
template<typename T, typename U, template<class...> class C>
std::enable_if_t<is_stl_container<C<T, U>>::value, nlohmann::json> to_json(C<T, U> const& s) {
  nlohmann::json json;

  if constexpr (is_associative_container<C<T, U>>::value) {
    if constexpr (std::is_fundamental<U>::value
                  || std::is_same<U, std::string>::value) {
      json = nlohmann::json(s);
    } else {
      // this is a bit of a hack for the nested custom type condition
      // we create a json object with the right keys and 0 int for the value
      // then replace the values with the actual json object
      C<T, int> temp;

      std::for_each(std::begin(s), std::end(s), [&temp](auto const& i) { temp[i.first] = 0; });
      json = nlohmann::json(temp);

      std::for_each(std::begin(s), std::end(s),
          [&json](auto const& i) { json[i.first] = to_json(i.second); });
    }
  } else {
    json = nlohmann::json::array();
    std::for_each(std::begin(s), std::end(s),
      [&json](auto const &i) {
        json.emplace_back(to_json(i));
      });
  }


  return json;
}

// serializing hana defined/adapted structs
template<typename T>
std::enable_if_t<boost::hana::Struct<T>::value, nlohmann::json> to_json(T const& s) {
  nlohmann::json json;

  boost::hana::for_each(boost::hana::keys(s),
    [&s, &json](auto name) {
      auto const &member = boost::hana::at_key(s, name);
      using Member = std::decay_t<decltype(member)>;
      auto memberName = boost::hana::to<char const*>(name);

      if constexpr (is_optional<Member>::value) {
        if (member) {
          json[memberName] = to_json(member.value());
        }
      } else {
        json[memberName] = to_json(member);
      }
    });

  return json;
}

}

#endif //TO_JSON_H

