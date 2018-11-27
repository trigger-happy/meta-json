set(
  TEST_LIBS
  pthread
  dl
  ${Boost_LIBRARIES}
  nlohmann_json::nlohmann_json
  ${PROJECT_NAME}::${META_JSON_TARGET_NAME}
)

set(
  TEST_INCLUDE_DIRS
)

file(GLOB TEST_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
foreach(testsrc ${TEST_SOURCES})
  string(REPLACE ".cpp" "" testname ${testsrc})
  add_executable(${testname} ${testsrc})
  target_link_libraries(${testname} ${TEST_LIBS})
  target_include_directories(${testname} PRIVATE ${TEST_INCLUDE_DIRS})
  set_property(TARGET ${testname} PROPERTY CXX_STANDARD 17)
  add_test(NAME ${testname} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin COMMAND ${testname})
endforeach(testsrc ${TEST_SOURCES})
