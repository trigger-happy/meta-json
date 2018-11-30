#ifndef FROM_JSON_H
#define FROM_JSON_H

#include "container_type_traits.h"
#include <algorithm>
#include <boost/hana.hpp>
#include <nlohmann/json.hpp>

namespace meta_json {

template <typename T>
std::enable_if_t<boost::hana::Struct<T>::value, T>
from_json(const nlohmann::json &j, const std::string &name = "");

template <typename T>
std::enable_if_t<std::is_fundamental<T>::value || is_std_string<T>::value, T>
from_json(const nlohmann::json &j, const std::string &name) {
  T val;
  val = j[name];
  return val;
}

template <typename T, typename U, template <class...> class C>
std::enable_if_t<std::is_same<C<T, U>, std::pair<T, U>>::value, std::pair<T, U>>
from_json(const nlohmann::json &j, const std::string &name) {
  std::pair<T, U> val;

  auto it = j[name].cbegin();
  val.first = it.key();
  val.second = it.value();

  return val;
}

template <typename T>
std::enable_if_t<boost::hana::Struct<T>::value, T>
from_json(const nlohmann::json &j, const std::string &name) {
  T val;
  nlohmann::json const *jm = nullptr;
  if (name.empty()) {
    jm = &j;
  } else {
    jm = &(j[name]);
  }

  boost::hana::for_each(boost::hana::keys(val), [&val, jm](auto name) {
    auto &member = boost::hana::at_key(val, name);
    using Member = std::decay_t<decltype(member)>;
    auto memberName = boost::hana::to<char const *>(name);

    if constexpr (is_optional<Member>::value) {
      typedef typename Member::value_type innerType;

      auto iter = jm->find(memberName);
      if (iter != jm->end()) {
        member = from_json<innerType>(*jm, memberName);
      } else {
        member = std::nullopt;
      }
    } else if constexpr (is_stl_container<Member>::value) {
      if constexpr (is_associative_container<Member>::value) {
        if constexpr (std::is_fundamental<typename Member::value_type>::value
            || std::is_same<typename Member::value_type, std::string>::value) {
          std::for_each(std::begin((*jm)[memberName]), std::end((*jm)[memberName]),
            [&member](auto const& i) {
              member.emplace_back(i.key(), i.value());
            });
        } else {
          //TODO implement me
        }
      } else {
        using innerType = typename Member::value_type;

        if constexpr (std::is_fundamental<innerType>::value
            || std::is_same<innerType, std::string>::value) {

          std::for_each(std::begin((*jm)[memberName]), std::end((*jm)[memberName]),
            [&member](auto const &i) {
              member.emplace_back(i);
            });

        } else {
          //TODO implement me
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
