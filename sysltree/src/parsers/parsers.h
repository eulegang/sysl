#pragma once

#include "../tokens.h"
#include "../tree.h"
#include <arcana.h>

#define token_code(T) (uint16_t)sysltree::token::T
#define node_code(T) (uint16_t)sysltree::node::T

namespace sysltree {
arcana_state parse_bitset(arcana_state state);
arcana_state parse_ident(arcana_state state);
arcana_state parse_lit(arcana_state state);
arcana_state parse_namespace(arcana_state state);
arcana_state parse_declaration(arcana_state state);
arcana_state parse_enum(arcana_state state);
arcana_state parse_struct(arcana_state state);
arcana_state parse_alias(arcana_state state);

arcana_state parse_type(arcana_state state);

extern arcana_parser *type_parser;

void init_type_parser(void) __attribute__((constructor));
void deinit_type_parser(void) __attribute__((destructor));
} // namespace sysltree
