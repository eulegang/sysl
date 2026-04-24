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

TEST(lexing, bitset_sample) {
  std::string_view sv = "color :: bitset { ERR, ASYNC }";
  mk_tokens(sv);
  EXPECT_THAT(testing::arcana_slices(tokens),
              ::testing::ElementsAre("color", "::", "bitset", "{", "ERR", ",",
                                     "ASYNC", "}"));

  EXPECT_THAT(testing::arcana_token_types(tokens),
              ::testing::ElementsAre(token(ident), token(dcolon), token(bitset),
                                     token(lbrace), token(ident), token(comma),
                                     token(ident), token(rbrace)));
  arcana_tokens_deinit(tokens);
}

TEST(lexing, func_sample) {
  std::string_view sv = "do_thing :: (id: u32, premium: bool) -> err!*u32";
  mk_tokens(sv);
  EXPECT_THAT(testing::arcana_slices(tokens),
              ::testing::ElementsAre("do_thing", "::", "(", "id", ":", "u32",
                                     ",", "premium", ":", "bool", ")", "->",
                                     "err", "!", "*", "u32"));

  EXPECT_THAT(testing::arcana_token_types(tokens),
              ::testing::ElementsAre(
                  token(ident), token(dcolon), token(lparen), token(ident),
                  token(colon), token(ident), token(comma), token(ident),
                  token(colon), token(ident), token(rparen), token(arrow),
                  token(ident), token(bang), token(mult), token(ident)));

  arcana_tokens_deinit(tokens);
}

TEST(lexing, const_sample) {
  std::string_view sv = "let name: u32 = 42";
  mk_tokens(sv);
  EXPECT_THAT(testing::arcana_slices(tokens),
              ::testing::ElementsAre("let", "name", ":", "u32", "=", "42"));

  EXPECT_THAT(testing::arcana_token_types(tokens),
              ::testing::ElementsAre(token(let), token(ident), token(colon),
                                     token(ident), token(assign),
                                     token(integer)));

  arcana_tokens_deinit(tokens);
}

TEST(lexing, var_sample) {
  std::string_view sv = "var name: u32 = 42";
  mk_tokens(sv);
  EXPECT_THAT(testing::arcana_slices(tokens),
              ::testing::ElementsAre("var", "name", ":", "u32", "=", "42"));

  EXPECT_THAT(testing::arcana_token_types(tokens),
              ::testing::ElementsAre(token(var), token(ident), token(colon),
                                     token(ident), token(assign),
                                     token(integer)));

  arcana_tokens_deinit(tokens);
}

TEST(lexing, bool_operators) {
  std::string_view sv = "! && || &&= ||=";
  mk_tokens(sv);
  EXPECT_THAT(testing::arcana_slices(tokens),
              ::testing::ElementsAre("!", "&&", "||", "&&=", "||="));

  EXPECT_THAT(testing::arcana_token_types(tokens),
              ::testing::ElementsAre(token(bang), token(bool_and),
                                     token(bool_or), token(bool_and_assign),
                                     token(bool_or_assign)));

  arcana_tokens_deinit(tokens);
}

TEST(lexing, num_operators) {
  std::string_view sv = "+ * - / % += *= -= /= %=";
  mk_tokens(sv);
  EXPECT_THAT(testing::arcana_slices(tokens),
              ::testing::ElementsAre("+", "*", "-", "/", "%",
                                     "+=", "*=", "-=", "/=", "%="));

  EXPECT_THAT(testing::arcana_token_types(tokens),
              ::testing::ElementsAre(token(plus), token(mult), token(minus),
                                     token(div), token(mod), token(plus_assign),
                                     token(mult_assign), token(minus_assign),
                                     token(div_assign), token(mod_assign)));

  arcana_tokens_deinit(tokens);
}

TEST(lexing, literals) {
  std::string_view sv = "true false 0 32 \"hello\"";
  mk_tokens(sv);
  EXPECT_THAT(testing::arcana_slices(tokens),
              ::testing::ElementsAre("true", "false", "0", "32", "\"hello\""));

  EXPECT_THAT(testing::arcana_token_types(tokens),
              ::testing::ElementsAre(token(bool_t), token(bool_f),
                                     token(integer), token(integer),
                                     token(str)));

  arcana_tokens_deinit(tokens);
}

TEST(lexing, line_comments) {
  std::string_view sv = "1 // 0\n 2";
  mk_tokens(sv);
  EXPECT_THAT(testing::arcana_slices(tokens), ::testing::ElementsAre("1", "2"));

  EXPECT_THAT(testing::arcana_token_types(tokens),
              ::testing::ElementsAre(token(integer), token(integer)));

  arcana_tokens_deinit(tokens);
}

TEST(lexing, block_comments) {
  std::string_view sv = "1 /* 0 */ 2";
  mk_tokens(sv);
  EXPECT_THAT(testing::arcana_slices(tokens), ::testing::ElementsAre("1", "2"));

  EXPECT_THAT(testing::arcana_token_types(tokens),
              ::testing::ElementsAre(token(integer), token(integer)));

  arcana_tokens_deinit(tokens);
}
