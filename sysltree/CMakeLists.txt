add_library(
  sysltree SHARED
  src/tokens.cpp
)

pkg_check_modules(ARCANA REQUIRED IMPORTED_TARGET arcana)
pkg_check_modules(MFILE REQUIRED IMPORTED_TARGET mfile)
pkg_check_modules(CHROMA REQUIRED IMPORTED_TARGET chroma)

target_link_libraries(sysltree PUBLIC PkgConfig::ARCANA)

find_package(GTest REQUIRED)
enable_testing()
add_executable(
  unit 

  tests/tokens.cpp
  tests/utils.cpp
  tests/token_table.cpp
)

target_include_directories(unit PUBLIC ./src)
target_link_libraries(unit PkgConfig::ARCANA)
target_link_libraries(unit GTest::gtest_main)
target_link_libraries(unit sysltree)

add_test(
  NAME unit-tests 
  COMMAND unit
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(
  sysl-tokenize

  bin/tokenize.cpp
)

target_include_directories(sysl-tokenize PUBLIC ./src)
target_link_libraries(sysl-tokenize sysltree)
target_link_libraries(sysl-tokenize PkgConfig::ARCANA)
target_link_libraries(sysl-tokenize PkgConfig::MFILE)
target_link_libraries(sysl-tokenize PkgConfig::CHROMA)
