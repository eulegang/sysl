cmake_minimum_required(VERSION 3.10)
project(sysl
  LANGUAGES C CXX
  VERSION 0.1.0
  DESCRIPTION "a systems language"
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ARCANA REQUIRED IMPORTED_TARGET arcana)
pkg_check_modules(MFILE REQUIRED IMPORTED_TARGET mfile)
pkg_check_modules(CHROMA REQUIRED IMPORTED_TARGET chroma)

add_subdirectory(sysltree)
add_subdirectory(util)
add_subdirectory(symbol)


