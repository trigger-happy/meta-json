#ifndef FROM_JSON_H
#define FROM_JSON_H

#include "container_type_traits.h"
#include <algorithm>
#include <boost/hana.hpp>
#include <nlohmann/json.hpp>

namespace meta_json {

// forward declaration for converting to hana adapted/defined structs
template <typename T>
std::enable_if_t<boost::hana::Struct<T>::value, T>
from_json(const nlohmann::json &j, const std::string &name = "");

// fundamental value or string serialization
template <typename T>
std::enable_if_t<std::is_fundamental<T>::value || is_std_string<T>::value, T>
from_json(const nlohmann::json &j, const std::string &name) {
  T val;
  val = j[name];
  return val;
}

// for serializing into structs
template <typename T>
std::enable_if_t<boost::hana::Struct<T>::value, T>
from_json(const nlohmann::json &j, const std::string &name) {
  T val;
  nlohmann::json const *jm = nullptr;
  // access the inner json object if requested
  if (name.empty()) {
    jm = &j;
  } else {
    jm = &(j[name]);
  }

  // loop through all the members of the struct
  boost::hana::for_each(boost::hana::keys(val), [&val, jm](auto name) {
    auto &member = boost::hana::at_key(val, name);
    using Member = std::decay_t<decltype(member)>;
    auto memberName = boost::hana::to<char const *>(name);

    if constexpr (is_optional<Member>::value) {
      // optional members
      typedef typename Member::value_type innerType;

      auto iter = jm->find(memberName);
      if (iter != jm->end()) {
        member = from_json<innerType>(*jm, memberName);
      } else {
        member = std::nullopt;
      }

    } else if constexpr (is_stl_container<Member>::value) {

      // STL containers
      auto const &innerJson = (*jm)[memberName];

      if constexpr (is_associative_container<Member>::value) {
        // map-like container, one size fits all
        using MappedType = typename Member::mapped_type;
        for (auto i = innerJson.cbegin(); i != innerJson.cend(); ++i) {
          member.emplace(i.key(), from_json<MappedType>(innerJson, i.key()));
        }

      } else {

        // vector/list like container
        using innerType = typename Member::value_type;
        if constexpr (std::is_fundamental<innerType>::value ||
                      std::is_same<innerType, std::string>::value) {

          // read into a vector of fundamental/string objects
          std::for_each(innerJson.cbegin(), innerJson.cend(),
                        [&member](auto const &i) { member.emplace_back(i); });

        } else {
          // vector of hana adapted structs
          std::for_each(innerJson.cbegin(), innerJson.cend(),
                        [&member](auto const jsonElement) {
                          member.emplace_back(
                              from_json<innerType>(jsonElement));
                        });
        }
      }
    } else {
      member = from_json<Member>(*jm, memberName);
    }
  });

  return val;
}

} // namespace meta_json

#endif // FROM_JSON_H
