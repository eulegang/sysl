#include "../tokens.h"
#include "../tree.h"
#include "parsers.h"
#include <arcana.h>
#include <cstdint>

arcana_state sysltree::parse_ident(arcana_state state) {
  arcana_token token = arcana_state_token(state);

  if (token.type != token_code(ident)) {
    state.status |= 1;
    return state;
  }

  uint16_t node = arcana_state_alloc_node(&state);
  uint16_t data = arcana_state_malloc(&state, sizeof(uint16_t));
  uint16_t *data_ptr = (uint16_t *)arcana_state_data(state, data);

  state.subroot = node;
  *arcana_state_node(state, node) = {
      .child = 0,
      .next = 0,
      .offset = data,
      .type = node_code(ident),
  };

  *data_ptr = state.token_cursor;

  arcana_state_next(&state);
  if (state.status) {
    return state;
  }

  return state;
}
