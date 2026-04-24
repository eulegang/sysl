#pragma once

#include "arcana.h"

enum class sysl_token : arcana_token_type {
  ident,
  ns, // namespace
  strukt,
  enumeration,
  bitset,

  let,
  var,

  integer,
  str,

  assign,
  plus,
  comma,
  semi,
  bang,
  minus,
  div,
  mult,
  mod,
  plus_assign,
  minus_assign,
  mult_assign,
  div_assign,
  mod_assign,
  eq,
  ne,

  lt,
  le,
  gt,
  ge,

  lparen,
  rparen,
  lbrace,
  rbrace,

  arrow,
  dcolon,
  colon,

  cond,      // if
  otherwise, // else
  ret,

  bool_t,
  bool_f,

  bool_and,
  bool_or,

  bool_and_assign,
  bool_or_assign,
};

ssize_t sysl_tokenizer(size_t, arcana_slice, arcana_token_type *);

arcana_token_table_t *sysl_token_table();
