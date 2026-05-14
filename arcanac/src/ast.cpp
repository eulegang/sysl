#include "ast.h"
#include "arcana.h"
#include "args.h"
#include <chroma.h>
#include <iostream>

std::array<std::string_view, 3>
extract_line(std::string_view view, sigil::Tokens<arcana::Token>::Token token);

struct ctx {
  std::ostream *out;
  const sigil::Tokens<arcana::Token> &tokens;
};

sigil::Ast<arcana::Node> ast(const sigil::Tokens<arcana::Token> &tokens) {
  try {
    sigil::Ast<arcana::Node> ast{arcana::parser, tokens};
    return ast;
  } catch (sigil_parser_error err) {
    auto token = tokens[err.token];
    sigil_linemeta meta = tokens.linemeta(err.token);

    auto [pre, cur, post] = extract_line(tokens.content(), token);

    std::cerr << "invalid token found at " << meta.line << ":" << meta.column
              << std::endl;
    std::cerr << chroma::clear << pre << chroma::red << cur << chroma::clear
              << post << std::endl;

    std::string pad(pre.length(), ' ');

    std::cerr << pad << "^" << std::endl;
    exit(2);
  } catch (...) {
    std::cerr << chroma::red << "unknown error while tokenizing" << std::endl;
    exit(4);
  }
}

void dump_nodes(uint16_t id, sigil::Ast<arcana::Node>::Node node, void *data,
                size_t level, ctx *ctx);

void report_ast(const sigil::Tokens<arcana::Token> &tokens,
                const sigil::Ast<arcana::Node> &tree) {
  std::ostream *out = &std::cout;
  ctx ctx = {
      .out = out,
      .tokens = tokens,
  };

  tree.visit(&ctx, dump_nodes);
}

std::array<std::string_view, 3>
extract_line(std::string_view view, sigil::Tokens<arcana::Token>::Token token) {
  size_t offset = token.off;

  while (offset > 0) {
    if (view[offset - 1] == '\n') {
      break;
    }

    offset--;
  }

  size_t len = 0;

  while (token.off + token.len + len + 1 < view.length()) {
    if (view[token.off + token.len + len + 1] == '\n') {
      break;
    }

    len++;
  }

  return {
      view.substr(offset, token.off - offset),
      view.substr(token.off, token.len),
      view.substr(token.off + token.len, len),
  };
}

void dump_nodes(uint16_t, sigil::Ast<arcana::Node>::Node node, void *data,
                size_t level, ctx *ctx) {

  auto &out = *ctx->out;

  auto ty = node.type;
  out << std::string(2 * level, ' ');

  switch (ty) {
  case arcana::Node::declare:
    out << chroma::hex(0xffff00) << "declare";
    break;

  case arcana::Node::ident:
    out << chroma::hex(0x00c0ffee) << "ident";

    if (verbose) {
      uint16_t idx = *(uint16_t *)data;
      std::string_view ident = ctx->tokens.content(idx);

      out << " " << chroma::hex(0xaaffaa) << ident;
    }

    break;

  case arcana::Node::ns:
    out << chroma::hex(0xff00ff) << "namespace";
    break;

  case arcana::Node::alias:
    out << chroma::hex(0xff00ff) << "alias";
    break;

  case arcana::Node::decls:
    out << chroma::hex(0xff00ff) << "decls";
    break;

  case arcana::Node::st:
    out << chroma::hex(0xff00ff) << "struct";

    if (verbose) {
      uint16_t meta = *(uint16_t *)data;

      if ((meta & arcana::ACCESS_OPAQUE) != 0) {
        out << " " << chroma::hex(0xaaffaa) << "opaque";
      }
    }

    break;

  case arcana::Node::st_fields:
    out << chroma::hex(0xff00ff) << "fields";
    break;

  case arcana::Node::st_field:
    out << chroma::hex(0xff00ff) << "field";

    if (verbose) {
      uint16_t idx = *(uint16_t *)data;
      std::string_view ident = ctx->tokens.content(idx);

      out << " " << chroma::hex(0xaaffaa) << ident;
    }

    break;

  case arcana::Node::en:
    out << chroma::hex(0xff00ff) << "enum";
    break;

  case arcana::Node::en_case:
    out << chroma::hex(0xff00aa) << "enum-case";

    if (verbose) {
      uint16_t idx = *(uint16_t *)data;
      std::string_view ident = ctx->tokens.content(idx);

      out << " " << chroma::hex(0xaaffaa) << ident;
    }

    break;

  case arcana::Node::bs:
    out << chroma::hex(0xff00ff) << "bitset";
    break;

  case arcana::Node::bs_case:
    out << chroma::hex(0xff00aa) << "bitset-case";

    if (verbose) {
      uint16_t idx = *(uint16_t *)data;
      std::string_view ident = ctx->tokens.content(idx);

      out << " " << chroma::hex(0xaaffaa) << ident;
    }

    break;

  case arcana::Node::fn:
    out << chroma::hex(0xff00aa) << "fn";
    break;

  case arcana::Node::fn_params:
    out << chroma::hex(0xff00aa) << "params";
    break;

  case arcana::Node::fn_param:
    out << chroma::hex(0xff00aa) << "param";

    if (verbose) {
      uint16_t idx = *(uint16_t *)data;
      std::string_view ident = ctx->tokens.content(idx);

      out << " " << chroma::hex(0xaaffaa) << ident;
    }

    break;

  case arcana::Node::fn_ret:
    out << chroma::hex(0xff00aa) << "return";
    break;

  case arcana::Node::literal: {
    out << chroma::hex(0xff00ff) << "literal ";
    arcana::LiteralData lit = *(arcana::LiteralData *)data;

    switch (lit.prim) {
    case arcana::Primitive::integer:
      out << "integer";
      break;
    case arcana::Primitive::floating:
      out << "float";
      break;
    case arcana::Primitive::boolean:
      out << "bool";
      break;
    }

    if (verbose) {
      std::string_view ident = ctx->tokens.content(lit.token);

      out << " " << chroma::hex(0xaaffaa) << ident;
    }

    break;
  }

  case arcana::Node::infer_type:
    out << chroma::hex(0x0ffaaaa) << "infer";
    break;

  case arcana::Node::ty:
    out << chroma::hex(0xaaccaa) << "type";
    break;

  case arcana::Node::slice:
    out << chroma::hex(0xaaff88) << "slice";
    break;

  case arcana::Node::array:
    out << chroma::hex(0xaaff88) << "array";

    if (verbose) {
      uint16_t idx = *(uint16_t *)data;
      std::string_view ident = ctx->tokens.content(idx);

      out << " " << chroma::hex(0xaaffaa) << ident;
    }

    break;

  case arcana::Node::pointer:
    out << chroma::hex(0xaaff88) << "pointer";

    break;
  }

  out << chroma::clear << std::endl;
}
