
#include "parsers.h"
#include <arcana.h>

namespace sysltree {
enum class type_perc : size_t {
  LOWEST,

  HIGHEST,
};

arcana_state parse_slice(arcana_state state) {
  arcana_token token = arcana_state_token(state);
  if (token.type != token_code(lbracket)) {
    state.status |= 4;
    return state;
  }

  arcana_state_next(&state);

  token = arcana_state_token(state);

  uint16_t id = arcana_state_alloc_node(&state);
  arcana_node *root = arcana_state_node(state, id);
  switch ((sysltree::token)token.type) {
  case sysltree::token::rbracket:
    arcana_state_next(&state);
    *root = {
        .child = 0,
        .next = 0,
        .offset = 0,
        .type = node_code(slice),
    };

    break;
  case sysltree::token::integer: // eventually should be an expr
  {
    uint16_t data = arcana_state_malloc(&state, sizeof(uint16_t));
    *(uint16_t *)arcana_state_data(state, data) = state.token_cursor;

    arcana_state_next(&state);

    token = arcana_state_token(state);
    if (token.type != token_code(rbracket)) {
      state.status |= 4;
      return state;
    }

    arcana_state_next(&state);

    *root = {
        .child = 0,
        .next = 0,
        .offset = data,
        .type = node_code(array),
    };
  } break;

  default:
    state.status |= 4;
    return state;
  }

  state =
      arcana_parser_parse_expr(type_parser, state, (size_t)type_perc::LOWEST);

  root->child = state.subroot;

  state.subroot = id;
  return state;
}

arcana_state parse_pointer(arcana_state state) {
  arcana_token token = arcana_state_token(state);
  if (token.type != token_code(mult)) {
    state.status |= 4;
    return state;
  }

  arcana_state_next(&state);

  uint16_t id = arcana_state_alloc_node(&state);
  arcana_node *root = arcana_state_node(state, id);
  *root = {
      .child = 0,
      .next = 0,
      .offset = 0,
      .type = node_code(pointer),

  };

  state =
      arcana_parser_parse_expr(type_parser, state, (size_t)type_perc::LOWEST);

  root->child = state.subroot;

  state.subroot = id;
  return state;
}

arcana_state parse_type(arcana_state state) {
  uint16_t id = arcana_state_alloc_node(&state);
  arcana_node *root = arcana_state_node(state, id);
  *root = {
      .child = 0,
      .next = 0,
      .offset = 0,
      .type = node_code(ty),

  };

  state =
      arcana_parser_parse_expr(type_parser, state, (size_t)type_perc::LOWEST);

  root->child = state.subroot;

  state.subroot = id;
  return state;
}

arcana_parser *type_parser;

bool terminal(arcana_token_type type);
arcana_state init(arcana_state);

void init_type_parser(void) {
  type_parser =
      arcana_parser_init((uint16_t)token::END + 1, sysltree::terminal, init);

  arcana_parser_slots(type_parser)[(uint16_t)token::ident] = {
      .prefix = sysltree::parse_ident,
      .postfix = 0,
      .infix = 0,
      .perc = 0,
  };

  arcana_parser_slots(type_parser)[(uint16_t)token::lbracket] = {
      .prefix = sysltree::parse_slice,
      .postfix = 0,
      .infix = 0,
      .perc = 0,
  };

  arcana_parser_slots(type_parser)[(uint16_t)token::mult] = {
      .prefix = sysltree::parse_pointer,
      .postfix = 0,
      .infix = 0,
      .perc = 0,
  };

  arcana_parser_slots(type_parser)[(uint16_t)token::fn] = {
      .prefix = sysltree::parse_func_sig,
      .postfix = 0,
      .infix = 0,
      .perc = 0,
  };
}

void deinit_type_parser(void) { arcana_parser_deinit(type_parser); }

} // namespace sysltree
