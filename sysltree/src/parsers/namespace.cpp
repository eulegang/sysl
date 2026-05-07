
#include "parsers.h"
#include <arcana.h>

namespace sysltree {
arcana_state parse_namespace(arcana_state state) {
  arcana_token token = arcana_state_token(state);

  if (token.type != token_code(ns)) {
    state.status |= 4;
    return state;
  }

  uint16_t node = arcana_state_alloc_node(&state);
  arcana_node *root = arcana_state_node(state, node);
  *root = {
      .child = 0,
      .next = 0,
      .offset = 0,
      .type = node_code(ns),
  };
  arcana_state_next(&state);
  if (state.status)
    return state;

  token = arcana_state_token(state);
  if (token.type != token_code(lbrace)) {
    state.status |= 1;
    return state;
  }

  arcana_state_next(&state);
  if (state.status)
    return state;

  arcana_node *cur = root;
  while (true) {
    state = parse_declaration(state);
    if (state.status) {
      return state;
    }

    if (cur == root) {
      cur->child = state.subroot;
      cur = arcana_ast_nodes(state.ast) + cur->child;
    } else {
      cur->next = state.subroot;
      cur = arcana_ast_nodes(state.ast) + cur->next;
    }

    token = arcana_state_token(state);
    if (token.type == token_code(rbrace)) {
      arcana_state_next(&state);
      break;
    }
  }

  state.subroot = node;
  return state;
}
} // namespace sysltree
