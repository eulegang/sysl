#pragma once
#include "arcana.h"

namespace sysltree {
enum class node : uint32_t {
  declare,
  ident,
  ns, // namespace
  st, // struct
  en, // enum
  bs, // bitset
};

struct ident_chain {
  uint16_t token;
  uint16_t len;
};

enum class perc : size_t {
  LOWEST,

  HIGHEST,
};

extern arcana_parser *parser;

void init_parser(void) __attribute__((constructor));
void deinit_parser(void) __attribute__((destructor));
} // namespace sysltree
