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
} // namespace sysltree
