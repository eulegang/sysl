#include <bit>
#include <cstdlib>
#include <getopt.h>

#include <iostream>
#include <string_view>

#include <chroma.h>
#include <mfile.h>
#include <sigil.h>

#include "arcana.h"
#include "args.h"
#include "ast.h"
#include "tokenize.h"

int main(int argc, char **argv) {
  parse_args(argc, argv);
  validate_args();

  mfile content = read_file();

  const sigil::Tokens<arcana::Token> tokens = tokenize(content);

  if (stops == 1) {
    report_tokens(tokens);
    return 0;
  }

  const sigil::Ast<arcana::Node> tree = ast(tokens);

  if (stops == 2) {
    report_ast(tokens, tree);
    return 0;
  }

  return 0;
}
