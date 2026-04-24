#pragma once

#include "tokens.h"
#include <arcana.h>
#include <string_view>
#include <vector>

std::ostream &operator<<(std::ostream &, const sysl_token &);
// bool operator==(const monkey_token &, const monkey_token &);

namespace testing {
std::vector<std::string_view> arcana_slices(arcana_tokens_t *);
std::vector<sysl_token> arcana_token_types(arcana_tokens_t *);
} // namespace testing
