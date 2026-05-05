#include "utils.h"
#include "tokens.h"
#include <arcana.h>
#include <iostream>

std::vector<std::string_view> testing::arcana_slices(arcana_tokens *tokens) {
  std::vector<std::string_view> res;

  size_t len = arcana_tokens_len(tokens);

  for (size_t i = 0; i < len; i++) {
    arcana_slice slice = arcana_tokens_slice(tokens, i);

    res.emplace_back(slice.data, slice.len);
  }

  return res;
}

std::vector<sysl_token> testing::arcana_token_types(arcana_tokens *tokens) {
  std::vector<sysl_token> res;

  size_t len = arcana_tokens_len(tokens);
  arcana_token *base = arcana_tokens_data(tokens);

  for (size_t i = 0; i < len; i++) {

    res.push_back((sysl_token)base[i].type);
  }

  return res;
}

std::ostream &operator<<(std::ostream &os, const sysl_token &token_type) {
  arcana_table *table = sysl_token_table();
  const char *name = arcana_table_data(table)[(arcana_token_type)token_type];

  return os << name;
}
