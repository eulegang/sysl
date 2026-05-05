#pragma once

#include "tokens.h"
#include <arcana.h>
#include <string_view>
#include <vector>

namespace testing {
std::vector<std::string_view> arcana_slices(arcana_tokens *);
std::vector<sysltree::token> arcana_token_types(arcana_tokens *);
} // namespace testing
