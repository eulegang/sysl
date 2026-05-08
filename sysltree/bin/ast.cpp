#include "tokens.h"
#include "tree.h"
#include <arcana.h>
#include <getopt.h>
#include <iostream>
#include <mfile.h>
#include <vector>

#include <chroma.h>

bool verbose = false;
bool color = true;

struct ctx {
  std::ostream *out;
  arcana_tokens *tokens;
};

bool parse_file(std::string_view content, const char *path);

int main(int argc, char **argv) {
  static struct option long_options[] = {
      {"verbose", no_argument, 0, 'v'},
      {"help", no_argument, 0, 'h'},
      {0, 0, 0, 0},
      // Sentinel to mark the end of the array
  };

  int c;

  const char *no_color = getenv("NO_COLOR");

  if (no_color && no_color[0] != 0) {
    color = false;
  }

  std::vector<std::string> files;
  while ((c = getopt_long(argc, argv, "vh", long_options, NULL)) != -1) {
    switch (c) {
    case 'v':
      verbose = true;
      break;

    case 'h':
      std::cout << "sysl-tokenize [-v] [files...]" << std::endl;
      return 0;
      break;
    }
  }

  int opt_i = optind;
  while (opt_i < argc) {
    files.push_back(argv[opt_i++]);
  }

  if (!color) {
    std::cout << chroma::disable;
  }

  bool success = true;
  if (files.empty()) {
    std::string content((std::istreambuf_iterator<char>(std::cin)),
                        std::istreambuf_iterator<char>());
    success &= parse_file(content, NULL);
  } else if (files.size() == 1) {
    mfile file{files[0]};
    success &= parse_file(file, NULL);
  } else {
    for (const auto &path : files) {
      mfile file{path.c_str()};
      success &= parse_file(file, path.c_str());
    }
  }

  if (!success) {
    return 2;
  }
}

void dump_sysl(arcana_node node, void *data, size_t level, arcana_slice content,
               void *ctx_param) {
  ctx *ctx = (::ctx *)ctx_param;
  auto &out = *ctx->out;

  auto ty = (sysltree::node)node.type;
  out << std::string(2 * level, ' ');

  switch (ty) {
  case sysltree::node::declare:
    out << chroma::hex(0xffff00) << "declare";
    break;

  case sysltree::node::ident:
    out << chroma::hex(0x00c0ffee) << "ident";

    if (verbose) {
      uint16_t idx = *(uint16_t *)data;
      auto token = arcana_tokens_data(ctx->tokens)[idx];
      std::string_view ident{content.data + token.off, token.len};

      out << " " << chroma::hex(0xaaffaa) << ident;
    }

    break;

  case sysltree::node::ns:
    out << chroma::hex(0xff00ff) << "namespace";
    break;

  case sysltree::node::alias:
    out << chroma::hex(0xff00ff) << "alias";
    break;

  case sysltree::node::st:
    out << chroma::hex(0xff00ff) << "struct";
    break;

  case sysltree::node::st_fields:
    out << chroma::hex(0xff00ff) << "fields";
    break;

  case sysltree::node::st_field:
    out << chroma::hex(0xff00ff) << "field";

    if (verbose) {
      uint16_t idx = *(uint16_t *)data;
      auto token = arcana_tokens_data(ctx->tokens)[idx];
      std::string_view ident{content.data + token.off, token.len};

      out << " " << chroma::hex(0xaaffaa) << ident;
    }

    break;

  case sysltree::node::en:
    out << chroma::hex(0xff00ff) << "enum";
    break;

  case sysltree::node::en_case:
    out << chroma::hex(0xff00aa) << "enum-case";

    if (verbose) {
      uint16_t idx = *(uint16_t *)data;
      auto token = arcana_tokens_data(ctx->tokens)[idx];
      std::string_view ident{content.data + token.off, token.len};

      out << " " << chroma::hex(0xaaffaa) << ident;
    }

    break;

  case sysltree::node::bs:
    out << chroma::hex(0xff00ff) << "bitset";
    break;

  case sysltree::node::bs_case:
    out << chroma::hex(0xff00aa) << "bitset-case";

    if (verbose) {
      uint16_t idx = *(uint16_t *)data;
      auto token = arcana_tokens_data(ctx->tokens)[idx];
      std::string_view ident{content.data + token.off, token.len};

      out << " " << chroma::hex(0xaaffaa) << ident;
    }

    break;

  case sysltree::node::literal: {
    out << chroma::hex(0xff00ff) << "literal ";
    sysltree::literal_data lit = *(sysltree::literal_data *)data;

    switch (lit.prim) {
    case sysltree::primitive::integer:
      out << "integer";
      break;
    case sysltree::primitive::floating:
      out << "float";
      break;
    case sysltree::primitive::boolean:
      out << "bool";
      break;
    }

    if (verbose) {
      auto token = arcana_tokens_data(ctx->tokens)[lit.token];
      std::string_view ident{content.data + token.off, token.len};

      out << " " << chroma::hex(0xaaffaa) << ident;
    }

    break;
  }

  case sysltree::node::infer_type:
    out << chroma::hex(0x0ffaaaa) << "infer";
    break;

  case sysltree::node::ty:
    out << chroma::hex(0xaaccaa) << "type";
    break;

  case sysltree::node::slice:
    out << chroma::hex(0xaaff88) << "slice";
    break;

  case sysltree::node::array:
    out << chroma::hex(0xaaff88) << "array";

    if (verbose) {
      uint16_t idx = *(uint16_t *)data;
      auto token = arcana_tokens_data(ctx->tokens)[idx];
      std::string_view ident{content.data + token.off, token.len};

      out << " " << chroma::hex(0xaaffaa) << ident;
    }

    break;

  case sysltree::node::pointer:
    out << chroma::hex(0xaaff88) << "pointer";

    break;
  }

  out << chroma::clear << std::endl;
}

bool parse_file(std::string_view content, const char *path) {
  arcana_tokens_options opts = {
      .content = {.data = content.data(), .len = content.length()},
      .tokenizer = sysltree::tokenizer,
  };

  arcana_tokens_error err;
  arcana_tokens *tokens = arcana_tokens_init(opts, &err);

  if (!tokens) {
    if (path)
      std::cerr << path << ":" << std::endl;

    switch (err.err) {
    case ARCANA_TOKENS_ERROR_MAP:
      std::cerr << "failed to map memory" << std::endl;
      break;
    case ARCANA_TOKENS_ERROR_OVERFLOW:
      std::cerr << "File to large to tokenize" << std::endl;
      break;
    case ARCANA_TOKENS_ERROR_INVALID:
      std::cerr << "Invalid token found" << std::endl;
      auto len = content.find_first_of("\n", err.pos);
      auto sub = content.substr(err.pos, len - 1);
      std::cerr << sub << std::endl;
      std::cerr << "^" << std::endl;

      break;
    }

    return false;
  }

  arcana_ast *ast = arcana_parser_parse(sysltree::parser, tokens);

  if (!ast) {
    std::cerr << "failed to parse" << std::endl;
    arcana_tokens_deinit(tokens);
    return true;
  }

  std::ostream *out = &std::cout;
  ctx ctx = {
      .out = out,
      .tokens = tokens,
  };

  arcana_ast_visit(ast, opts.content, &ctx, dump_sysl);

  arcana_ast_deinit(ast);
  arcana_tokens_deinit(tokens);

  return true;
}
