#include "parsers.h"
#include <arcana.h>

namespace sysltree {
arcana_state parse_bitset_backing(arcana_state state) {
  arcana_token token = arcana_state_token(state);

  if (token.type != token_code(lparen)) {
    uint16_t node = arcana_state_alloc_node(&state);
    *arcana_state_node(state, node) = {
        .child = 0,
        .next = 0,
        .offset = 0,
        .type = node_code(infer_type),
    };

    state.subroot = node;
    return state;
  }

  arcana_state_next(&state);

  state = parse_ident(state);
  if (state.status) {
    return state;
  }

  token = arcana_state_token(state);

  if (token.type != token_code(rparen)) {
    state.status |= 4;
    return state;
  }

  arcana_state_next(&state);

  return state;
}

arcana_state parse_bitset_case(arcana_state state) {
  arcana_token token = arcana_state_token(state);

  if (token.type != token_code(ident)) {
    state.status |= 4;
    return state;
  }

  uint16_t data = arcana_state_malloc(&state, sizeof(uint16_t));
  *(uint16_t *)arcana_state_data(state, data) = state.token_cursor;

  uint16_t node = arcana_state_alloc_node(&state);
  arcana_node *root = arcana_state_node(state, node);
  *root = {
      .child = 0,
      .next = 0,
      .offset = data,
      .type = node_code(bs_case),
  };

  arcana_state_next(&state);
  if (state.status) {
    return state;
  }

  token = arcana_state_token(state);
  if (token.type != token_code(assign)) {
    state.subroot = node;
    return state;
  }

  arcana_state_next(&state);
  if (state.status) {
    return state;
  }

  state =
      arcana_parser_parse_expr(sysltree::parser, state, (size_t)perc::LOWEST);

  root->child = state.subroot;

  state.subroot = node;
  return state;
}

arcana_state parse_bitset(arcana_state state) {
  arcana_token token = arcana_state_token(state);

  if (token.type != token_code(bitset)) {
    state.status |= 4;
    return state;
  }

  uint16_t node = arcana_state_alloc_node(&state);
  arcana_node *root = arcana_state_node(state, node);
  *root = {
      .child = 0,
      .next = 0,
      .offset = 0,
      .type = node_code(bs),
  };
  arcana_state_next(&state);
  if (state.status)
    return state;

  state = parse_bitset_backing(state);

  root->child = state.subroot;

  token = arcana_state_token(state);
  if (token.type != token_code(lbrace)) {
    state.status |= 1;
    return state;
  }

  arcana_state_next(&state);
  if (state.status)
    return state;

  arcana_node *cur = arcana_ast_nodes(state.ast) + root->child;

  while (true) {
    state = parse_bitset_case(state);
    if (state.status) {
      return state;
    }

    cur->next = state.subroot;
    cur = arcana_ast_nodes(state.ast) + cur->next;

    token = arcana_state_token(state);
    if (token.type == token_code(rbrace)) {
      arcana_state_next(&state);
      break;
    }

    if (token.type != token_code(comma)) {
      state.status |= 4;
      return state;
    }

    arcana_state_next(&state);

    token = arcana_state_token(state);
    if (token.type == token_code(rbrace)) {
      arcana_state_next(&state);
      break;
    }
  }

  state.subroot = node;
  return state;
} // namespace sysltree
} // namespace sysltree
