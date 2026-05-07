
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
