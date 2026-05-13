#pragma once

#include "arcana.h"
#include "symbol.h"

namespace arcana {
struct NamePass : Pass {
  struct Name {
    symbol _symbol;
    uint16_t _parent;
  };

  SymbolTable &symbol_table;
  sigil::Overlay<Name> overlay;

  NamePass(SymbolTable &);

  void run(const Unit &) override;
  void scan(const Unit &unit, uint16_t space, uint16_t cur);
};
} // namespace arcana
