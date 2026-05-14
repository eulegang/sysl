#pragma once

#include "arcana.h"
#include <sigil.h>

sigil::Tokens<arcana::Token> tokenize(std::string_view content);
void report_tokens(const sigil::Tokens<arcana::Token> &tokens);
