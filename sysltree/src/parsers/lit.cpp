#include "../tokens.h"
#include "../tree.h"
#include <arcana.h>
#include <cstdint>

namespace sysltree {
arcana_state parse_lit(arcana_state state) {
  arcana_token token = arcana_state_token(state);

  sysltree::primitive prim = sysltree::primitive::integer;
  switch ((sysltree::token)token.type) {
  case token::integer:
    prim = sysltree::primitive::integer;
    break;

  default:
    state.status |= 1;
    return state;
    break;
  }

  uint16_t data = arcana_state_malloc(&state, sizeof(literal_data));
  *(literal_data *)arcana_state_data(state, data) = {
      .token = state.token_cursor,
      .prim = prim,
  };

  uint16_t idx = arcana_state_alloc_node(&state);
  *arcana_state_node(state, idx) = {
      .child = 0,
      .next = 0,
      .offset = data,
      .type = (uint16_t)node::literal,
  };

  arcana_state_next(&state);

  state.subroot = idx;
  return state;
}

} // namespace sysltree
