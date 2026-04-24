#include <arcana.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tokens.h"
#include "utils.h"

#define token(T) sysl_token::T

#define mk_tokens(sv)                                                          \
  arcana_tokens *tokens;                                                       \
  do {                                                                         \
    arcana_tokens_options opts = {                                             \
        .content = {.data = sv.data(), .len = sv.length()},                    \
        .tokenizer = sysl_tokenizer,                                           \
    };                                                                         \
    arcana_tokens_error err;                                                   \
    tokens = arcana_tokens_init(opts, &err);                                   \
    ASSERT_NE(tokens, nullptr)                                                 \
        << std::format("position {} {}", err.pos, sv.substr(err.pos));         \
  } while (0)

TEST(lexing, namespace_sample) {
  std::string_view sv = "arcana :: namespace {}";
  mk_tokens(sv);
  EXPECT_THAT(testing::arcana_slices(tokens),
              ::testing::ElementsAre("arcana", "::", "namespace", "{", "}"));

  EXPECT_THAT(testing::arcana_token_types(tokens),
              ::testing::ElementsAre(token(ident), token(dcolon), token(ns),
                                     token(lbrace), token(rbrace)));
  arcana_tokens_deinit(tokens);
}

TEST(lexing, struct_sample) {
  std::string_view sv = "slice :: struct { base: *u8, len: u64, }";
  mk_tokens(sv);
  EXPECT_THAT(testing::arcana_slices(tokens),
              ::testing::ElementsAre("slice", "::", "struct", "{", "base", ":",
                                     "*", "u8", ",", "len", ":", "u64", ",",
                                     "}"));

  EXPECT_THAT(testing::arcana_token_types(tokens),
              ::testing::ElementsAre(token(ident), token(dcolon), token(strukt),
                                     token(lbrace), token(ident), token(colon),
                                     token(mult), token(ident), token(comma),
                                     token(ident), token(colon), token(ident),
                                     token(comma), token(rbrace)));
  arcana_tokens_deinit(tokens);
}

TEST(lexing, enum_sample) {
  std::string_view sv = "type :: enum { NORMAL, ABNORMAL }";
  mk_tokens(sv);
  EXPECT_THAT(testing::arcana_slices(tokens),
              ::testing::ElementsAre("type", "::", "enum", "{", "NORMAL", ",",
                                     "ABNORMAL", "}"));

  EXPECT_THAT(testing::arcana_token_types(tokens),
              ::testing::ElementsAre(token(ident), token(dcolon),
                                     token(enumeration), token(lbrace),
                                     token(ident), token(comma), token(ident),
                                     token(rbrace)));
  arcana_tokens_deinit(tokens);
}
