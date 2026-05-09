#include "parsers.h"
#include <arcana.h>
#include <cstdint>

namespace sysltree {
arcana_state parse_func_sig_arg(arcana_state state) {
  check_token(ident);

  node_id id = arcana_state_alloc_node(&state);
  arcana_node *root = arcana_state_node(state, id);

  data_id data_id = arcana_state_malloc(&state, sizeof(uint16_t));
  uint16_t *ident_slot = (uint16_t *)arcana_state_data(state, data_id);

  *ident_slot = state.token_cursor;

  *root = {
      .child = 0,
      .next = 0,
      .offset = data_id,
      .type = node_code(fn_param),
  };

  arcana_state_next(&state);
  check_token(colon);
  arcana_state_next(&state);

  state = arcana_parser_parse_expr(type_parser, state, (size_t)perc::LOWEST);

  root->child = state.subroot;
  state.subroot = id;

  return state;
}

arcana_state parse_func_sig_ret(arcana_state state) {
  if (arcana_state_token(state).type != token_code(arrow)) {
    state.subroot = 0;
    return state;
  }

  arcana_state_next(&state);

  state = arcana_parser_parse_expr(type_parser, state, (size_t)perc::LOWEST);

  if (arcana_state_token(state).type != token_code(bang)) {
    return state;
  }

  arcana_state_next(&state);

  node_id err_type_id = state.subroot;

  state = arcana_parser_parse_expr(type_parser, state, (size_t)perc::LOWEST);

  arcana_state_node(state, state.subroot)->next = err_type_id;

  return state;
}

arcana_state parse_func_sig(arcana_state state) {
  check_token(fn);
  arcana_state_next(&state);

  node_id id = arcana_state_alloc_node(&state);
  arcana_node *root = arcana_state_node(state, id);

  node_id params_id = arcana_state_alloc_node(&state);
  arcana_node *params_node = arcana_state_node(state, params_id);

  node_id return_id = arcana_state_alloc_node(&state);
  arcana_node *return_node = arcana_state_node(state, return_id);

  *root = {
      .child = params_id,
      .next = 0,
      .offset = 0xFFFF,
      .type = node_code(fn),
  };

  *params_node = {
      .child = 0,
      .next = return_id,
      .offset = 0xFFFF,
      .type = node_code(fn_params),
  };

  *return_node = {
      .child = 0,
      .next = 0,
      .offset = 0xFFFF,
      .type = node_code(fn_ret),
  };

  check_token(lparen);
  arcana_state_next(&state);

  arcana_node *params_cur = params_node;
  while (true) {
    loop_terminal_token(rparen);
    state = parse_func_sig_arg(state);

    if (params_cur == params_node) {
      params_cur->child = state.subroot;
      params_cur = arcana_state_node(state, state.subroot);
    } else {
      params_cur->next = state.subroot;
      params_cur = arcana_state_node(state, state.subroot);
    }

    loop_terminal_token(rparen);

    check_token(comma);
    arcana_state_next(&state);

    loop_terminal_token(rparen);
  }

  state = parse_func_sig_ret(state);

  return_node->child = state.subroot;

  state.subroot = id;
  return state;
}
} // namespace sysltree
