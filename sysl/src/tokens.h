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

  assign,
  plus,
  comma,
  semi,
  bang,
  minus,
  div,
  mult,
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
};

ssize_t sysl_tokenizer(size_t, arcana_slice, arcana_token_type *);

arcana_token_table_t *sysl_token_table();
