
#include "../tokens.h"
#include "../tree.h"
#include "parsers.h"
#include <arcana.h>
#include <cstdint>

namespace sysltree {
arcana_state parse_declaration(arcana_state state) {
  state = sysltree::parse_ident(state);
  if (state.status)
    return state;

  uint16_t ident_node = state.subroot;

  arcana_token token = arcana_state_token(state);
  if (token.type != token_code(dcolon)) {
    state.status |= 4;
    return state;
  }

  arcana_state_next(&state);
  token = arcana_state_token(state);

  uint16_t idx = arcana_state_alloc_node(&state);
  arcana_node *node = arcana_state_node(state, idx);
  node->type = node_code(declare);
  node->child = ident_node;

  arcana_node *ident = arcana_state_node(state, ident_node);
  switch ((sysltree::token)token.type) {
  case sysltree::token::bitset:
    state = parse_bitset(state);
    ident->next = state.subroot;
    break;

  case sysltree::token::ns:
    state = parse_namespace(state);
    ident->next = state.subroot;
    break;

  case sysltree::token::enumeration:
  case sysltree::token::strukt:
    state.status |= 4;
    break;

  default:
    state.status |= 4;
    return state;
  }

  if (state.status) {
    return state;
  }

  state.subroot = idx;
  return state;
}
} // namespace sysltree
