#ifndef __SYMBOL_H
#define __SYMBOL_H

#include <stddef.h>
#include <stdint.h>

#define SYMBOL_INTERN_FAIL 0xFFFF

#ifdef __cplusplus
extern "C" {
#endif

typedef struct symbol_table symbol_table;
typedef uint16_t symbol;

symbol_table *symbol_table_init(size_t cap, size_t pages);
void symbol_table_deinit(symbol_table *);

symbol symbol_table_intern(symbol_table *, const char *);
const char *symbol_table_resolve(symbol_table *, symbol);

#ifdef __cplusplus
}
#endif
#endif
