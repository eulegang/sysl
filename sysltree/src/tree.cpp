
#include "tree.h"
#include "tokens.h"
#include <arcana.h>
#include <cstdint>

#define token_code(T) (uint16_t)token::T
#define node_code(T) (uint32_t)node::T

namespace sysltree {
arcana_parser *parser;

bool terminal(arcana_token_type type) {
  return type == (arcana_token_type)token::semi;
}

arcana_state parse_ident_chain_parse(arcana_state state) {
  arcana_token token = arcana_state_token(state);

  if (token.type != token_code(ident)) {
    state.status |= 1;
    return state;
  }

  uint16_t node = arcana_state_alloc_node(&state);
  uint16_t data = arcana_state_malloc(&state, sizeof(ident_chain));
  ident_chain *chain = (ident_chain *)arcana_state_data(state, data);

  state.subroot = node;
  *arcana_state_node(state, node) = {
      .child = 0,
      .next = 0,
      .offset = data,
      .type = node_code(ident),
  };

  chain->token = state.token_cursor;
  chain->len = 1;

  arcana_state_next(&state);
  if (state.status) {
    return state;
  }

  while (true) {
    token = arcana_state_token(state);
    if (token.type != token_code(dcolon)) {
      return state;
    }

    token = arcana_state_peek(state, 1);
    if (token.type != token_code(ident)) {
      return state;
    }

    arcana_state_next(&state);
    arcana_state_next(&state);
    if (state.status)
      return state;

    chain->len += 1;
  }

  return state;
}

arcana_state parse_declaration(arcana_state state) {
  state = parse_ident_chain_parse(state);
  arcana_token token = arcana_state_token(state);
  if (token.type != token_code(dcolon)) {
    state.status |= 4;
    return state;
  }

  arcana_state_next(&state);

  token = arcana_state_token(state);

  switch ((sysltree::token)token.type) {

  case sysltree::token::ns:
  case sysltree::token::strukt:
  case sysltree::token::enumeration:
  case sysltree::token::bitset:
    break;

  default:
    state.status |= 4;
    return state;
  }

  return state;
}

arcana_state init(arcana_state state) {

  uint16_t last = 0xFFFF;

  while (!arcana_state_done(state)) {
    state = parse_declaration(state);

    if (last != 0xFFFF) {
      arcana_node *node = arcana_ast_nodes(state.ast) + last;
      node->next = state.subroot;
    }

    last = state.subroot;

    if (state.status) {
      return state;
    }
  }

  return state;
}

} // namespace sysltree

void sysltree::init_parser(void) {
  parser = arcana_parser_init((uint16_t)token::END + 1, terminal, init);
}

void sysltree::deinit_parser(void) { arcana_parser_deinit(parser); }
