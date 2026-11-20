#pragma once
#include <vector>
#include "lexer/lexer.h"
#include"parser/ast.h"

Program parseProgram(const std::vector<Token>& tokens);
void printProgram(const Program& p);

