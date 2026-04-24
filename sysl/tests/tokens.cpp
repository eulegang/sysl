#include <arcana.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tokens.h"
#include "utils.h"

TEST(empty, empty) {}

TEST(x, tokens) {
  std::string_view sv = "arcana :: namespace {\n"
                        "  slice :: struct { }\n"
                        "}\n";

  arcana_tokens_options opts = {
      .content = {.data = sv.data(), .len = sv.length()},
      .tokenizer = sysl_tokenizer,
  };

  arcana_tokens_error err;

  arcana_tokens *tokens = arcana_tokens_init(opts, &err);
  ASSERT_NE(tokens, nullptr)
      << std::format("position {} {}", err.pos, sv.substr(err.pos));

  EXPECT_THAT(testing::arcana_slices(tokens),
              ::testing::ElementsAre("let", "five", "=", "5", ";", "let", "ten",
                                     "=", "10", ";", "let", "add", "=", "fn",
                                     "(", "x", ",", "y", ")", "{", "x", "+",
                                     "y", ";", "}", ";", "let", "result", "=",
                                     "add", "(", "five", ",", "ten", ")", ";"));

  arcana_tokens_deinit(tokens);
}
