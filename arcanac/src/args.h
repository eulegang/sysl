#pragma once
#include <mfile.h>

extern bool verbose;
extern std::string_view path;
extern uint16_t stops;

void parse_args(int argc, char **argv);
void validate_args();
mfile read_file();
