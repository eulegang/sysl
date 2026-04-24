#include "tokens.h"
#include <arcana.h>
ssize_t sysl_tokenizer(size_t, arcana_slice, arcana_token_type *) { return 0; }

#define push_str(id) arcana_token_table_push(&table, #id);
arcana_token_table_t *sysl_token_table() {
  static arcana_token_table_t *table = NULL;

  if (!table) {
    table = arcana_token_table_init();

    push_str(ident);

    push_str(integer);

    push_str(assign);
    push_str(plus);
    push_str(comma);
    push_str(semi);
    push_str(bang);
    push_str(minus);
    push_str(div);
    push_str(mult);
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

    push_str(cond);      // if
    push_str(otherwise); // else
    push_str(ret);

    push_str(bool_t);
    push_str(bool_f);
  }

  return table;
}
