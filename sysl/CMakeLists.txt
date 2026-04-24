add_library(
  sysl SHARED
  src/tokens.cpp
)

pkg_check_modules(ARCANA REQUIRED IMPORTED_TARGET arcana)

target_link_libraries(sysl PUBLIC PkgConfig::ARCANA)

find_package(GTest REQUIRED)
enable_testing()
add_executable(
  unit 

  tests/tokens.cpp
  tests/utils.cpp
)

target_include_directories(unit PUBLIC ./src)
target_link_libraries(unit PkgConfig::ARCANA)
target_link_libraries(unit GTest::gtest_main)
target_link_libraries(unit sysl)

add_test(
  NAME unit-tests 
  COMMAND unit
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
