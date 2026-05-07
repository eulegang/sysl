
#include "tree.h"
#include "parsers/parsers.h"
#include "tokens.h"
#include <arcana.h>
#include <cstdint>

namespace sysltree {
arcana_parser *parser;

bool terminal(arcana_token_type type) {
  return type == (arcana_token_type)token::semi;
}

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
  case sysltree::token::strukt:
  case sysltree::token::enumeration:
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

arcana_state init(arcana_state state) {

  uint16_t last = 0xFFFF;

  while (!arcana_state_done(state)) {
    state = parse_declaration(state);

    if (last != 0xFFFF) {
      arcana_node *node = arcana_ast_nodes(state.ast) + last;
      node->next = state.subroot;
    } else {
      arcana_node *node = arcana_ast_nodes(state.ast);
      node[0] = node[state.subroot];
      last = 0;
    }

    last = state.subroot;

    if (state.status) {
      return state;
    }
  }

  state.subroot = last;

  return state;
}

} // namespace sysltree

void sysltree::init_parser(void) {
  parser = arcana_parser_init((uint16_t)token::END + 1, terminal, init);

  arcana_parser_slots(parser)[(uint16_t)token::integer] = {
      .prefix = sysltree::parse_lit,
      .postfix = 0,
      .infix = 0,
      .perc = 0,
  };
}

void sysltree::deinit_parser(void) { arcana_parser_deinit(parser); }
