#pragma once
#include "arcana.h"

namespace sysltree {
enum class node : uint16_t {
  declare,
  ident,

  ns, // namespace
  st, // struct

  /* Enums */
  en, // enum
  en_case,

  /* BitSet */
  bs, // bitset
  bs_case,

  literal,

  infer_type,
};

enum class primitive : uint16_t {
  integer,
  floating,
  boolean,
};

struct literal_data {
  uint16_t token;
  primitive prim;
};

enum class perc : size_t {
  LOWEST,

  HIGHEST,
};

extern arcana_parser *parser;

void init_parser(void) __attribute__((constructor));
void deinit_parser(void) __attribute__((destructor));
} // namespace sysltree
