cmake_minimum_required(VERSION 3.10)
project(sysl
  LANGUAGES CXX
  VERSION 0.1.0
  DESCRIPTION "a systems language"
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ARCANA REQUIRED IMPORTED_TARGET arcana)
add_subdirectory(sysl)


