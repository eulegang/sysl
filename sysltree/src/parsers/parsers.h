#pragma once

#include "../tokens.h"
#include "../tree.h"
#include <arcana.h>

typedef uint16_t node_id;
typedef uint16_t data_id;

#define token_code(T) (uint16_t)sysltree::token::T
#define node_code(T) (uint16_t)sysltree::node::T

#define check_token(T)                                                         \
  {                                                                            \
    arcana_token token = arcana_state_token(state);                            \
    if (token.type != token_code(T)) {                                         \
      state.status |= 4;                                                       \
      return state;                                                            \
    }                                                                          \
  }

#define loop_terminal_token(T)                                                 \
  {                                                                            \
    arcana_token token = arcana_state_token(state);                            \
    if (token.type == token_code(T)) {                                         \
      arcana_state_next(&state);                                               \
      break;                                                                   \
    }                                                                          \
  }

namespace sysltree {
arcana_state parse_bitset(arcana_state state);
arcana_state parse_ident(arcana_state state);
arcana_state parse_lit(arcana_state state);
arcana_state parse_namespace(arcana_state state);
arcana_state parse_declaration(arcana_state state);
arcana_state parse_enum(arcana_state state);
arcana_state parse_struct(arcana_state state);
arcana_state parse_alias(arcana_state state);

arcana_state parse_func_sig(arcana_state state);

arcana_state parse_type(arcana_state state);

extern arcana_parser *type_parser;

void init_type_parser(void) __attribute__((constructor));
void deinit_type_parser(void) __attribute__((destructor));
} // namespace sysltree
