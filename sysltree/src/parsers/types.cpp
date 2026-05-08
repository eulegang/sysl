
#include "parsers.h"
#include <arcana.h>

namespace sysltree {
enum class type_perc : size_t {
  LOWEST,

  HIGHEST,
};

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
}

void deinit_type_parser(void) { arcana_parser_deinit(type_parser); }

} // namespace sysltree
