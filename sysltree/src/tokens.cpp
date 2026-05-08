#include "tokens.h"
#include <arcana.h>

namespace sysltree {
bool is_space(char ch) {
  return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
}

bool is_ident(char ch) {
  return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ch == '_' ||
         ('0' <= ch && ch <= '9');
}

bool is_integer(char ch) { return ('0' <= ch && ch <= '9'); }
bool not_newline(char ch) { return ch != '\n'; }

bool end_block_comment(void *st, char ch) {
  uint32_t *state = (uint32_t *)st;

  if (*state == 2) {
    return false;
  }

  switch (ch) {
  case '/': {
    if (*state == 1) {
      *state = 2;
    }
  } break;

  case '*':
    *state = 1;
    break;

  default:
    *state = 0;
  }

  return true;
}

bool end_str(void *st, char ch) {
  uint32_t *state = (uint32_t *)st;

  switch (*state) {
  case 0: {
    if (ch == '"') {
      *state = 1;
    }
  } break;

  case 1: {
    if (ch == '\\') {
      *state = 2;
    } else if (ch == '"') {
      *state = 3;
    }
  } break;
  case 2:
    *state = 1;
    break;

  case 3:
    return false;
  }

  return true;
}

#define token(T) (arcana_token_type) sysltree::token::T
ssize_t tokenizer(size_t cur, arcana_slice content,
                  arcana_token_type *token_type) {
  arcana_slice window = arcana_slice_advance(content, cur);

  char ch = content.data[cur];
  ssize_t inc = 0;

  switch (ch) {
  case ' ':
  case '\t':
  case '\n':
  case '\r':
    return -arcana_util_take_while(window, sysltree::is_space);

  case ';':
    *token_type = token(semi);
    return 1;

  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    *token_type = token(integer);

    return arcana_util_take_while(window, sysltree::is_integer);
  case '=':
    if ((inc = arcana_util_keyword(window, "=="))) {
      *token_type = token(eq);
      return inc;
    }

    *token_type = token(assign);
    return 1;

  case '!':
    if ((inc = arcana_util_keyword(window, "!="))) {
      *token_type = token(ne);
      return inc;
    }

    *token_type = token(bang);
    return 1;

  case '*':
    if ((inc = arcana_util_keyword(window, "*="))) {
      *token_type = token(mult_assign);
      return inc;
    }

    *token_type = token(mult);
    return 1;

  case '-':
    if ((inc = arcana_util_keyword(window, "->"))) {
      *token_type = token(arrow);
      return inc;
    }

    if ((inc = arcana_util_keyword(window, "-="))) {
      *token_type = token(minus_assign);
      return inc;
    }

    *token_type = token(minus);
    return 1;

  case '/':
    if (window.len > 1 && window.data[1] == '/') {
      return -arcana_util_take_while(window, sysltree::not_newline);
    }

    uint32_t state;
    if (window.len > 1 && window.data[1] == '*') {
      return -arcana_util_take_stateful(window, &state,
                                        sysltree::end_block_comment);
    }

    if ((inc = arcana_util_keyword(window, "/="))) {
      *token_type = token(div_assign);
      return inc;
    }

    *token_type = token(div);
    return 1;

  case '"': {
    int state = 0;
    *token_type = token(str);
    return arcana_util_take_stateful(window, &state, sysltree::end_str);
  } break;

  case '%':
    if ((inc = arcana_util_keyword(window, "%="))) {
      *token_type = token(mod_assign);
      return inc;
    }

    *token_type = token(mod);
    return 1;

  case ',':
    *token_type = token(comma);
    return 1;

  case 'a':
    if ((inc = arcana_util_keyword(window, "alias"))) {
      *token_type = token(alias);
      return inc;
    }
    break;

  case 'b':
    if ((inc = arcana_util_keyword(window, "bitset"))) {
      *token_type = token(bitset);
      return inc;
    }
    break;

  case 'e':
    if ((inc = arcana_util_keyword(window, "enum"))) {
      *token_type = token(enumeration);
      return inc;
    }
    break;

  case 'f':
    if ((inc = arcana_util_keyword(window, "false"))) {
      *token_type = token(bool_f);
      return inc;
    }
    break;

  case 'l':
    if ((inc = arcana_util_keyword(window, "let"))) {
      *token_type = token(let);
      return inc;
    }
    break;

  case 't':
    if ((inc = arcana_util_keyword(window, "true"))) {
      *token_type = token(bool_t);
      return inc;
    }

    break;

  case 'n':
    if ((inc = arcana_util_keyword(window, "namespace"))) {
      *token_type = token(ns);
      return inc;
    }
    break;

  case 's':
    if ((inc = arcana_util_keyword(window, "struct"))) {
      *token_type = token(strukt);
      return inc;
    }
    break;

  case 'v':
    if ((inc = arcana_util_keyword(window, "var"))) {
      *token_type = token(var);
      return inc;
    }
    break;

  case ':':
    if ((inc = arcana_util_keyword(window, "::"))) {
      *token_type = token(dcolon);
      return inc;
    }

    *token_type = token(colon);
    return 1;

  case '{':
    *token_type = token(lbrace);
    return 1;

  case '}':
    *token_type = token(rbrace);
    return 1;

  case '(':
    *token_type = token(lparen);
    return 1;

  case ')':
    *token_type = token(rparen);
    return 1;

  case '&':
    if ((inc = arcana_util_keyword(window, "&&="))) {
      *token_type = token(bool_and_assign);
      return inc;
    }

    if ((inc = arcana_util_keyword(window, "&&"))) {
      *token_type = token(bool_and);
      return inc;
    }

    break;

  case '|':
    if ((inc = arcana_util_keyword(window, "||="))) {
      *token_type = token(bool_or_assign);
      return inc;
    }

    if ((inc = arcana_util_keyword(window, "||"))) {
      *token_type = token(bool_or);
      return inc;
    }

    break;

  case '+':
    if ((inc = arcana_util_keyword(window, "+="))) {
      *token_type = token(plus_assign);
      return inc;
    }

    *token_type = token(plus);
    return 1;
  }

  if (('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ch == '_') {
    *token_type = token(ident);
    return arcana_util_take_while(window, sysltree::is_ident);
  }

  return 0;
}

arcana_table *table;

#define push_str(id) arcana_table_push(&table, #id);
void init_table() {
  table = arcana_table_init();

  push_str(ident);
  push_str(ns);
  arcana_table_push(&table, "struct"); // push_str(strukt);
  arcana_table_push(&table, "enum");   // push_str(enumeration);
  push_str(bitset);
  push_str(alias);

  push_str(let);
  push_str(var);

  push_str(integer);
  push_str(str);

  push_str(assign);
  push_str(plus);
  push_str(comma);
  push_str(semi);
  push_str(bang);
  push_str(minus);
  push_str(div);
  push_str(mult);
  push_str(mod);

  push_str(plus_assign);
  push_str(minus_assign);
  push_str(mult_assign);
  push_str(div_assign);
  push_str(mod_assign);
  push_str(eq);
  push_str(ne);

  push_str(lt);
  push_str(le);
  push_str(gt);
  push_str(ge);

  push_str(lparen);
  push_str(rparen);
  push_str(lbrace);
  push_str(rbrace);

  push_str(arrow);
  push_str(dcolon);
  push_str(colon);

  arcana_table_push(&table, "if");   // push_str(cond);      // if
  arcana_table_push(&table, "else"); // push_str(otherwise); // else
  push_str(ret);

  push_str(bool_t);
  push_str(bool_f);

  push_str(bool_and);
  push_str(bool_or);

  push_str(bool_and_assign);
  push_str(bool_or_assign);
}

void deinit_table() { arcana_table_deinit(table); }

} // namespace sysltree

std::ostream &operator<<(std::ostream &os, const sysltree::token &token_type) {
  const char *name =
      arcana_table_data(sysltree::table)[(arcana_token_type)token_type];

  return os << name;
}
