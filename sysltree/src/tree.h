#pragma once
#include "arcana.h"

namespace sysltree {
enum class node : uint16_t {
  declare,
  ident,

  decls,

  ns, // namespace
  alias,

  /* Struct */
  st,
  st_fields,
  st_field,

  infer_type, // useful for bitset / enums

  /* Enums */
  en, // enum
  en_case,

  /* BitSet */
  bs,
  bs_case,

  literal,

  ty, // a type slot

  slice,
  array,
  pointer,

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
