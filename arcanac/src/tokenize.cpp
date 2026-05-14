#include "tokenize.h"
#include "args.h"

#include <chroma.h>
#include <iostream>

chroma::basic_t token_color(arcana::Token type);

sigil::Tokens<arcana::Token> tokenize(std::string_view content) {
  try {
    sigil::Tokens<arcana::Token> tokens{content, arcana::tokenizer};
    return tokens;
  } catch (sigil_tokens_error err) {

    switch (err.err) {
    case SIGIL_TOKENS_ERROR_MAP:
      std::cerr << "failed to map memory" << std::endl;
      break;
    case SIGIL_TOKENS_ERROR_OVERFLOW:
      std::cerr << "File to large to tokenize" << std::endl;
      break;
    case SIGIL_TOKENS_ERROR_INVALID:
      std::cerr << "Invalid token found" << std::endl;
      auto len = content.find_first_of("\n", err.pos);
      auto sub = content.substr(err.pos, len - 1);
      std::cerr << sub << std::endl;
      std::cerr << "^" << std::endl;

      exit(2);
      break;
    }
    exit(4);
  } catch (...) {
    std::cerr << chroma::red << "unknown error while tokenizing" << std::endl;
    exit(4);
  }
}

void report_tokens(const sigil::Tokens<arcana::Token> &tokens) {

  size_t len = tokens.length();

  for (size_t i = 0; i < len; i++) {
    arcana::Token type = tokens[i].type;
    std::string_view text = tokens.content(i);

    if (verbose) {
      sigil_linemeta meta = tokens.linemeta(i);
      std::cout << chroma::cyan << i << "\t";
      std::cout << token_color(type) << type << "\t";
      std::cout << chroma::yellow << meta.line << ":" << meta.column << "\t";
      std::cout << chroma::purple << text << chroma::clear << std::endl;
    } else {
      std::cout << token_color(type) << type << chroma::clear << std::endl;
    }
  }
}

chroma::basic_t token_color(arcana::Token type) {

  switch (type) {
  case arcana::Token::ident:
    return chroma::cyan;
  case arcana::Token::ns:
  case arcana::Token::strukt:
  case arcana::Token::enumeration:
  case arcana::Token::bitset:
  case arcana::Token::alias:
  case arcana::Token::fn:
  case arcana::Token::opaque:
    return chroma::purple;

  case arcana::Token::cond:
  case arcana::Token::otherwise:
  case arcana::Token::ret:
  case arcana::Token::let:
  case arcana::Token::var:
    return chroma::purple;

  case arcana::Token::integer:
  case arcana::Token::str:
  case arcana::Token::bool_t:
  case arcana::Token::bool_f:
    return chroma::cyan;

  case arcana::Token::bool_and:
  case arcana::Token::bool_or:
  case arcana::Token::bool_and_assign:
  case arcana::Token::bool_or_assign:
  case arcana::Token::assign:
  case arcana::Token::plus:
  case arcana::Token::bang:
  case arcana::Token::minus:
  case arcana::Token::div:
  case arcana::Token::mult:
  case arcana::Token::mod:
  case arcana::Token::plus_assign:
  case arcana::Token::minus_assign:
  case arcana::Token::mult_assign:
  case arcana::Token::div_assign:
  case arcana::Token::mod_assign:
  case arcana::Token::eq:
  case arcana::Token::ne:
  case arcana::Token::lt:
  case arcana::Token::le:
  case arcana::Token::gt:
  case arcana::Token::ge:
    return chroma::blue;

  case arcana::Token::comma:
  case arcana::Token::semi:
  case arcana::Token::lparen:
  case arcana::Token::rparen:
  case arcana::Token::lbrace:
  case arcana::Token::rbrace:
  case arcana::Token::lbracket:
  case arcana::Token::rbracket:
  case arcana::Token::arrow:
  case arcana::Token::dcolon:
  case arcana::Token::colon:
    return chroma::yellow;
  }

  return chroma::cyan;
}
