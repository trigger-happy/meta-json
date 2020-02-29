cmake_minimum_required(VERSION 3.16)
project(meta_json VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

option(BUILD_TESTS "Build unit tests" ON)
option(USE_EXTERNAL_JSON "Use external version of nlohmann_json library" OFF)
option(ENABLE_CLANG_TIDY "Run clang-tidy against all source files included in the build" "OFF")
option(ENABLE_CLANG_FORMAT "Run clang-format against all source files included in the build" "OFF")

set(META_JSON_TARGET_NAME               ${PROJECT_NAME})
set(META_JSON_CONFIG_INSTALL_DIR        "lib/cmake/${PROJECT_NAME}" CACHE INTERNAL "")
set(META_JSON_INCLUDE_INSTALL_DIR       "include")
set(META_JSON_TARGETS_EXPORT_NAME       "${PROJECT_NAME}Targets")
set(META_JSON_CMAKE_CONFIG_TEMPLATE     "cmake/config.cmake.in")
set(META_JSON_CMAKE_CONFIG_DIR          "${CMAKE_CURRENT_BINARY_DIR}")
set(META_JSON_CMAKE_VERSION_CONFIG_FILE "${META_JSON_CMAKE_CONFIG_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
set(META_JSON_CMAKE_PROJECT_CONFIG_FILE "${META_JSON_CMAKE_CONFIG_DIR}/${PROJECT_NAME}Config.cmake")
set(META_JSON_CMAKE_PROJECT_TARGETS_FILE "${META_JSON_CMAKE_CONFIG_DIR}/${PROJECT_NAME}Targets.cmake")


if(EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
  include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
  conan_basic_setup()
else()
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

# Code quality cmake modules
if(EXISTS "${CMAKE_SOURCE_DIR}/cmake/clang_tools.cmake")
  include(${CMAKE_SOURCE_DIR}/cmake/clang_tools.cmake)
endif()
if(EXISTS "${CMAKE_SOURCE_DIR}/cmake/cppcheck.cmake")
  include(${CMAKE_SOURCE_DIR}/cmake/cppcheck.cmake)
endif()

find_package(Boost 1.72 CONFIG)

add_library(${META_JSON_TARGET_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${META_JSON_TARGET_NAME} ALIAS ${META_JSON_TARGET_NAME})
target_compile_features(${META_JSON_TARGET_NAME} INTERFACE cxx_std_17)

target_include_directories(
  ${META_JSON_TARGET_NAME}
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)


include(CTest)

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()

add_subdirectory(thirdparty)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${META_JSON_CMAKE_VERSION_CONFIG_FILE} COMPATIBILITY SameMajorVersion
)
configure_file(
    ${META_JSON_CMAKE_CONFIG_TEMPLATE}
    ${META_JSON_CMAKE_PROJECT_CONFIG_FILE}
    @ONLY
)

install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/include/"
    DESTINATION ${META_JSON_INCLUDE_INSTALL_DIR}
)
install(
    FILES ${META_JSON_CMAKE_PROJECT_CONFIG_FILE} ${META_JSON_CMAKE_VERSION_CONFIG_FILE}
    DESTINATION ${META_JSON_CONFIG_INSTALL_DIR}
)
export(
    TARGETS ${META_JSON_TARGET_NAME}
    NAMESPACE ${PROJECT_NAME}::
    FILE ${META_JSON_CMAKE_PROJECT_TARGETS_FILE}
)
install(
    TARGETS ${META_JSON_TARGET_NAME}
    EXPORT ${META_JSON_TARGETS_EXPORT_NAME}
    INCLUDES DESTINATION ${META_JSON_INCLUDE_INSTALL_DIR}
)
install(
    EXPORT ${META_JSON_TARGETS_EXPORT_NAME}
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${META_JSON_CONFIG_INSTALL_DIR}
)
