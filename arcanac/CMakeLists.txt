add_executable(
  arcanac

  src/main.cpp
  src/tokenize.cpp
  src/args.cpp
  src/ast.cpp
)

target_include_directories(arcanac PUBLIC ../arcana/src)
target_link_libraries(arcanac arcana)
target_link_libraries(arcanac PkgConfig::SIGIL)
target_link_libraries(arcanac PkgConfig::MFILE)
target_link_libraries(arcanac PkgConfig::CHROMA)

# add_executable(
#   sysl-ast
#
#   src/ast.cpp
#   src/util.cpp
# )
#
# add_executable(
#   sysl-tokenize
#
#   src/tokenize.cpp
#   src/util.cpp
# )
#
# target_include_directories(sysl-ast PUBLIC ../sysltree/src)
# target_link_libraries(sysl-ast sysltree)
# target_link_libraries(sysl-ast PkgConfig::SIGIL)
# target_link_libraries(sysl-ast PkgConfig::MFILE)
# target_link_libraries(sysl-ast PkgConfig::CHROMA)
#
# target_include_directories(sysl-tokenize PUBLIC ../sysltree/src)
# target_link_libraries(sysl-tokenize sysltree)
# target_link_libraries(sysl-tokenize PkgConfig::SIGIL)
# target_link_libraries(sysl-tokenize PkgConfig::MFILE)
# target_link_libraries(sysl-tokenize PkgConfig::CHROMA)
#
