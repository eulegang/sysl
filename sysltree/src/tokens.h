#pragma once

#include "arcana.h"
#include <ostream>
namespace sysltree {
enum class token : arcana_token_type {
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

ssize_t tokenizer(size_t, arcana_slice, arcana_token_type *);

extern arcana_table *table;

void init_table(void) __attribute__((constructor));
void deinit_table(void) __attribute__((destructor));
} // namespace sysltree

std::ostream &operator<<(std::ostream &, const sysltree::token &);
