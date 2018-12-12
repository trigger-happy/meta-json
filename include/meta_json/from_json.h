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

// read into strings
template <typename T>
std::enable_if_t<is_std_string<T>::value, T>
from_json(const nlohmann::json &j, const std::string &name) {
  T val;

  if (j[name].is_string()) {
    val = j[name];
  } else if (j[name].is_boolean()) {
    val = std::to_string((bool)j[name]);
  } else if (j[name].is_number()) {
    val = std::to_string((double)j[name]);
  }

  return val;
}

// read into numbers
template <typename T>
std::enable_if_t<std::is_fundamental<T>::value, T>
from_json(const nlohmann::json &j, const std::string &name) {
  T val;

  if (j[name].is_string()) {
    if constexpr (std::is_floating_point_v<T>) {
      val = std::stod((std::string)j[name]);
    } else if constexpr (std::is_integral_v<T>) {
      val = std::stoi((std::string)j[name]);
    }
  } else {
    val = j[name];
  }

  return val;
}

// read into associative containers
template <typename T>
std::enable_if_t<is_associative_container<T>::value, T>
from_json(const nlohmann::json &j, const std::string &name) {
  using MappedType = typename T::mapped_type;

  T val;

  for (auto i = j[name].cbegin(); i != j[name].cend(); ++i) {
    val.emplace(i.key(), from_json<MappedType>(j[name], i.key()));
  }

  return val;
}

// read into vector/list like containers
template <typename T>
std::enable_if_t<is_vector_like_container<T>::value, T>
from_json(const nlohmann::json &j, const std::string &name) {
  using ValueType = typename T::value_type;

  T val;

  if constexpr (std::is_fundamental_v<ValueType> ||
                std::is_same_v<ValueType, std::string>) {
    // read into a vector of fundamental/string objects
    std::for_each(j[name].cbegin(), j[name].cend(),
                  [&val](auto const &i) { val.emplace_back(i); });
  } else {
    std::for_each(j[name].cbegin(), j[name].cend(),
                  [&val](auto const jsonElement) {
                    val.emplace_back(from_json<ValueType>(jsonElement));
                  });
  }

  return val;
}

// optional stuff
template <typename T>
std::enable_if_t<is_optional<T>::value, T> from_json(const nlohmann::json &j,
                                                     const std::string &name) {
  using ValueType = typename T::value_type;

  T val;

  auto iter = j.find(name);
  if (iter != j.end()) {
    val = from_json<ValueType>(j, name);
  } else {
    val = std::nullopt;
  }

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

    member = from_json<Member>(*jm, memberName);
  });

  return val;
}

template <typename T>
std::enable_if_t<boost::hana::Struct<T>::value, T>
from_json(const std::string &jsonString, const std::string &name = "") {
  return from_json<T>(nlohmann::json::parse(jsonString), name);
}

} // namespace meta_json

#endif // FROM_JSON_H
