#pragma once

#include "arcana.h"
#include <sigil.h>

sigil::Ast<arcana::Node> ast(const sigil::Tokens<arcana::Token> &tokens);

void report_ast(const sigil::Tokens<arcana::Token> &tokens,
                const sigil::Ast<arcana::Node> &tree);
