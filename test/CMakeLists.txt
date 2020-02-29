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
  ${Boost_INCLUDE_DIRS}
)

file(GLOB TEST_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test_*.cpp)
foreach(testsrc ${TEST_SOURCES})
  string(REPLACE ".cpp" "" testname ${testsrc})
  add_executable(${testname} ${testsrc})
  target_link_libraries(${testname} ${TEST_LIBS})
  target_include_directories(${testname} PRIVATE ${TEST_INCLUDE_DIRS})
  set_property(TARGET ${testname} PROPERTY CXX_STANDARD 17)
  add_test(NAME ${testname} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin COMMAND ${testname})
endforeach(testsrc ${TEST_SOURCES})

# special case for the fuzzy test
if(FUZZER_SANITIZER)
  add_executable(fuzz_test fuzz_test.cpp)
  target_compile_options(fuzz_test PUBLIC -fsanitize=fuzzer,address)
  target_link_options(fuzz_test PUBLIC -fsanitize=fuzzer,address)
  target_link_libraries(fuzz_test ${TEST_LIBS})
  target_include_directories(fuzz_test PRIVATE ${TEST_INCLUDE_DIRS})
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/fuzz_corpus DESTINATION ${CMAKE_BINARY_DIR}/)
  set_property(TARGET fuzz_test PROPERTY CXX_STANDARD 17)
  add_test(NAME fuzz_test WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin COMMAND fuzz_test -seed=42 -runs=10000000 ${CMAKE_BINARY_DIR}/fuzz_corpus)
  set_tests_properties(fuzz_test PROPERTIES TIMEOUT 60)
endif()
