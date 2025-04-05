#ifndef CODEGEN_H
#define CODEGEN_H
#include <vector>
#include <string>
#include <ostream>

void generateAssembly(const std::vector<std::string>& tac, std::ostream& out);
#endif