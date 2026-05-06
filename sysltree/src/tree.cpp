
#include "tree.h"
#include "tokens.h"
#include <arcana.h>

namespace sysltree {
arcana_parser *parser;

bool terminal(arcana_token_type type) {
  return type == (arcana_token_type)token::semi;
}

arcana_state decl_ident(arcana_state state) {
  arcana_token token = arcana_state_token(state);

  if (token.type != (uint16_t)token::ident) {
    state.status |= 1;
    return state;
  }

  arcana_state_next(&state);
  if (state.status) {
    return state;
  }

  return state;
}

arcana_state init(arcana_state state) { return state; }

} // namespace sysltree

void sysltree::init_parser(void) {
  parser = arcana_parser_init((uint16_t)token::END + 1, terminal, init);
}

void sysltree::deinit_parser(void) { arcana_parser_deinit(parser); }
