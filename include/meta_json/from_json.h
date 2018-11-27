#ifndef FROM_JSON_H
#define FROM_JSON_H

#include "container_type_traits.h"
#include <algorithm>
#include <boost/hana.hpp>
#include <nlohmann/json.hpp>

namespace meta_json {

template<typename T>
std::enable_if_t<boost::hana::Struct<T>::value, T> from_json(const nlohmann::json &j, const std::string &name = "");

template<typename T>
std::enable_if_t<std::is_fundamental<T>::value
                || is_std_string<T>::value, T> from_json(const nlohmann::json& j, const std::string &name) {
  T val;
  val = j[name];
  return val;
}

template<typename T, typename U, template<class...> class C>
std::enable_if_t<std::is_same<C<T, U>, std::pair<T, U>>::value, std::pair<T, U>> from_json(const nlohmann::json &j, const std::string &name) {
  std::pair<T, U> val;

  auto it = j[name].cbegin();
  val.first = it.key();
  val.second = it.value();

  return val;
}

template<typename T>
std::enable_if_t<boost::hana::Struct<T>::value, T> from_json(const nlohmann::json &j, const std::string &name) {
  T val;
  nlohmann::json const* jm = nullptr;
  if (name.empty()) {
    jm = &j;
  } else {
    jm = &(j[name]);
  }

  boost::hana::for_each(boost::hana::keys(val),
    [&val, jm](auto name) {
      auto &member = boost::hana::at_key(val, name);
      using Member = std::decay_t<decltype(member)>;
      auto memberName = boost::hana::to<char const*>(name);

      if constexpr (is_optional<Member>::value) {
        if (jm->find(memberName) != jm->end()) {
          member = from_json<Member>(*(jm)[memberName]);
        } else {
          member = std::nullopt;
        }
      } else {
        member = from_json<Member>(*jm, memberName);
      }
    });

  return val;
}

}

#endif //FROM_JSON_H
