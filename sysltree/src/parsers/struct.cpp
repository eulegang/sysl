;
#include "parsers.h"
#include <arcana.h>

namespace sysltree {
arcana_state parse_struct_field(arcana_state state) {
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
      .type = node_code(st_field),
  };

  arcana_state_next(&state);
  if (state.status) {
    return state;
  }

  token = arcana_state_token(state);
  if (token.type != token_code(colon)) {
    state.subroot = node;
    return state;
  }

  arcana_state_next(&state);
  if (state.status) {
    return state;
  }

  state = parse_type(state);

  if (state.status)
    return state;

  root->child = state.subroot;

  state.subroot = node;
  return state;
}

arcana_state parse_struct(arcana_state state) {
  uint16_t meta = 0;
  if (arcana_state_token(state).type == token_code(opaque)) {
    meta |= SYSLTREE_ACCESS_OPAQUE;
    arcana_state_next(&state);
  }

  check_token(strukt);

  data_id struct_meta_id = arcana_state_malloc(&state, sizeof(uint16_t));
  uint16_t *data = (uint16_t *)arcana_state_data(state, struct_meta_id);
  *data = meta;

  uint16_t node = arcana_state_alloc_node(&state);
  arcana_node *root = arcana_state_node(state, node);
  *root = {
      .child = 0,
      .next = 0,
      .offset = struct_meta_id,
      .type = node_code(st),
  };

  uint16_t fields_id = arcana_state_alloc_node(&state);
  arcana_node *fields = arcana_state_node(state, fields_id);
  root->child = fields_id;
  *fields = {
      .child = 0,
      .next = 0,
      .offset = 0,
      .type = node_code(st_fields),
  };

  uint16_t decls_id = arcana_state_alloc_node(&state);
  arcana_node *decls = arcana_state_node(state, decls_id);
  fields->next = decls_id;
  *decls = {
      .child = 0,
      .next = 0,
      .offset = 0,
      .type = node_code(decls),
  };

  arcana_state_next(&state);
  if (state.status)
    return state;

  arcana_token token = arcana_state_token(state);
  if (token.type != token_code(lbrace)) {
    state.status |= 1;
    return state;
  }

  arcana_state_next(&state);

  arcana_node *fields_cur = fields;
  arcana_node *decls_cur = decls;

  while (true) {
    token = arcana_state_token(state);
    if (token.type == token_code(rbrace)) {
      arcana_state_next(&state);
      break;
    }

    token = arcana_state_token(state);
    arcana_token peek = arcana_state_peek(state, 1);

    if (token.type != token_code(ident)) {
      state.status |= 4;
      return state;
    }

    bool semi_required = false;
    switch ((sysltree::token)peek.type) {
    case sysltree::token::colon:
      state = parse_struct_field(state);

      if (fields_cur == fields) {
        fields_cur->child = state.subroot;
        fields_cur = arcana_ast_nodes(state.ast) + fields_cur->child;
      } else {
        fields_cur->next = state.subroot;
        fields_cur = arcana_ast_nodes(state.ast) + fields_cur->next;
      }

      semi_required = true;

      break;

    case sysltree::token::dcolon:
      state = parse_declaration(state);
      if (decls_cur == decls) {
        decls_cur->child = state.subroot;
        decls_cur = arcana_ast_nodes(state.ast) + decls_cur->child;
      } else {
        decls_cur->next = state.subroot;
        decls_cur = arcana_ast_nodes(state.ast) + decls_cur->next;
      }

      semi_required = false;

      break;

    default:
      state.status |= 4;
      return state;
    }

    if (state.status) {
      return state;
    }

    token = arcana_state_token(state);
    if (token.type == token_code(rbrace)) {
      arcana_state_next(&state);
      break;
    }

    if (semi_required) {
      if (token.type != token_code(semi)) {
        state.status |= 4;
        return state;
      }

      arcana_state_next(&state);
    }
  }

  state.subroot = node;

  return state;
}

arcana_state parse_alias(arcana_state state) {
  arcana_token token = arcana_state_token(state);

  if (token.type != token_code(alias)) {
    state.status |= 4;
    return state;
  }
  uint16_t node = arcana_state_alloc_node(&state);
  arcana_node *root = arcana_state_node(state, node);
  *root = {
      .child = 0,
      .next = 0,
      .offset = 0,
      .type = node_code(alias),
  };

  arcana_state_next(&state);

  state = parse_type(state);
  if (state.status)
    return state;

  token = arcana_state_token(state);
  if (token.type != token_code(semi)) {
    state.status |= 4;
    return state;
  }

  arcana_state_next(&state);

  root->child = state.subroot;

  state.subroot = node;
  return state;
}

} // namespace sysltree
