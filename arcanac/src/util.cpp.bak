#include "util.h"
#include <arcana.h>
#include <chroma.h>
#include <iostream>

void report_token_error(arcana_tokens_error error, std::string_view content) {
  switch (error.err) {
  case ARCANA_TOKENS_ERROR_MAP:
    std::cerr << "failed to map memory" << std::endl;
    break;
  case ARCANA_TOKENS_ERROR_OVERFLOW:
    std::cerr << "File to large to tokenize" << std::endl;
    break;
  case ARCANA_TOKENS_ERROR_INVALID:
    std::cerr << "Invalid token found" << std::endl;
    auto len = content.find_first_of("\n", error.pos);
    auto sub = content.substr(error.pos, len - 1);
    std::cerr << sub << std::endl;
    std::cerr << "^" << std::endl;

    break;
  }
}

std::array<std::string_view, 3> extract_line(std::string_view view,
                                             arcana_token token) {
  size_t offset = token.off;

  while (offset > 0) {
    if (view[offset - 1] == '\n') {
      break;
    }

    offset--;
  }

  size_t len = 0;

  while (token.off + token.len + len + 1 < view.length()) {
    if (view[token.off + token.len + len + 1] == '\n') {
      break;
    }

    len++;
  }

  return {
      view.substr(offset, token.off - offset),
      view.substr(token.off, token.len),
      view.substr(token.off + token.len, len),
  };
}

void report_parse_error(arcana_parser_error error, arcana_tokens *tokens,
                        std::string_view content) {
  arcana_token token = arcana_tokens_data(tokens)[error.token];
  arcana_linemeta meta = arcana_tokens_linemeta(tokens)[error.token];

  auto [pre, cur, post] = extract_line(content, token);

  std::cerr << "invalid token found at " << meta.line << ":" << meta.column
            << std::endl;
  std::cerr << chroma::clear << pre << chroma::red << cur << chroma::clear
            << post << std::endl;

  std::string pad(pre.length(), ' ');

  std::cerr << pad << "^" << std::endl;
  ;
}
