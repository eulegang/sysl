
#include <arcana.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tokens.h"
#include "utils.h"

#define token(T) sysl_token::T

#define check(token)                                                           \
  ss.str("");                                                                  \
  ss << sysl_token::token;                                                     \
  EXPECT_EQ(ss.str(), #token);

TEST(token_table, exact_match) {
  std::stringstream ss;

  check(ident);

  check(ident);
  check(ns);
  check(strukt);
  check(enumeration);

  check(integer);

  check(assign);
  check(plus);
  check(comma);
  check(semi);
  check(bang);
  check(minus);
  check(div);
  check(mult);
  check(eq);
  check(ne);

  check(lt);
  check(le);
  check(gt);
  check(ge);

  check(lparen);
  check(rparen);
  check(lbrace);
  check(rbrace);

  check(arrow);
  check(dcolon);
  check(colon);

  check(cond);
  check(otherwise);
  check(ret);

  check(bool_t);
  check(bool_f);
}
