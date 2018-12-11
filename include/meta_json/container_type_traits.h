#ifndef CONTAINER_TYPE_TRAITS_H_H
#define CONTAINER_TYPE_TRAITS_H_H

#include <variant>
#include <vector>
#include <list>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <set>
#include <type_traits>

namespace meta_json {

template<typename T> struct is_stl_container : std::false_type{};
template<typename... Args> struct is_stl_container<std::vector<Args...>> : std::true_type{};
template<typename... Args> struct is_stl_container<std::list<Args...>> : std::true_type{};
template<typename... Args> struct is_stl_container<std::set<Args...>> : std::true_type{};
template<typename... Args> struct is_stl_container<std::unordered_set<Args...>> : std::true_type{};
template<typename... Args> struct is_stl_container<std::map<Args...>> : std::true_type{};

// add more here as needed

template<typename T> struct is_associative_container : std::false_type{};
template<typename... Args> struct is_associative_container<std::map<Args...>> : std::true_type{};
template<typename... Args> struct is_associative_container<std::multimap<Args...>> : std::true_type{};
template<typename... Args> struct is_associative_container<std::unordered_map<Args...>> : std::true_type{};

template<typename T> struct is_std_string : std::false_type{};
template<> struct is_std_string<std::string> : std::true_type{};

template<typename T> struct is_variant : std::false_type{};
template<typename... Args> struct is_variant<std::variant<Args...>> : std::true_type{};

template<typename T> struct is_optional : std::false_type{};
template<typename... Args> struct is_optional<std::optional<Args...>> : std::true_type{};

template<typename... Args> struct is_pair : std::false_type{};
template<typename... Args> struct is_pair<std::pair<Args...>> : std::true_type{};

}

#endif //CONTAINER_TYPE_TRAITS_H_H
