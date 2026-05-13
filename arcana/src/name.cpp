#include "name.h"
#include "arcana.h"
#include "symbol.h"
#include <cstdint>

namespace arcana {
NamePass::NamePass(SymbolTable &table) : symbol_table{table} {}

void NamePass::scan(const Unit &unit, uint16_t space, uint16_t cur) {
  auto node = unit.ast[cur];
  uint16_t subspace = space;

  switch (node.type) {
  case Node::declare:
    if (node.child != 0 && unit.ast[node.child].type == Node::ident) {
      auto ident = unit.ast[node.child];
      std::string_view content = unit.tokens.content();
      auto token = unit.tokens[ident.offset];

      symbol sym = symbol_table.intern(content.substr(token.off, token.len));
      Name *name = overlay.alloc(cur);

      name->_parent = space;
      name->_symbol = sym;
    }

    subspace = cur;
    break;

  case Node::ident: {
    auto ident = unit.ast[node.child];
    std::string_view content = unit.tokens.content();

    auto token = unit.tokens[ident.offset];
    symbol sym = symbol_table.intern(content.substr(token.off, token.len));
    Name *name = overlay.alloc(cur);

    name->_parent = 0;
    name->_symbol = sym;

  } break;
  default:
    break;
  }

  if (node.child != 0) {
    scan(unit, subspace, node.child);
  }

  if (node.next != 0) {
    scan(unit, space, node.next);
  }
}
void NamePass::run(const Unit &unit) {
  overlay = sigil::Overlay<Name>(unit.ast.ptr.get(), 4);

  scan(unit, 0, 0);
}

} // namespace arcana
