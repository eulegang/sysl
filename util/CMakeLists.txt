add_executable(
  sysl-ast

  src/ast.cpp
  src/util.cpp
)

add_executable(
  sysl-tokenize

  src/tokenize.cpp
  src/util.cpp
)

target_include_directories(sysl-ast PUBLIC ../sysltree/src)
target_link_libraries(sysl-ast sysltree)
target_link_libraries(sysl-ast PkgConfig::SIGIL)
target_link_libraries(sysl-ast PkgConfig::MFILE)
target_link_libraries(sysl-ast PkgConfig::CHROMA)

target_include_directories(sysl-tokenize PUBLIC ../sysltree/src)
target_link_libraries(sysl-tokenize sysltree)
target_link_libraries(sysl-tokenize PkgConfig::SIGIL)
target_link_libraries(sysl-tokenize PkgConfig::MFILE)
target_link_libraries(sysl-tokenize PkgConfig::CHROMA)

