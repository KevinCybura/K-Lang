#include <llvm/ADT/STLExtras.h>
#include <fstream>
#include <functional>
#include <iostream>
#include "AST.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char* argv[]) {
    std::ifstream file(argv[1]);
    char str[5];
    CurTok x = CurTok{};
    x.file = std::move(file);
    getNextToken(x);

    MainLoop(x);
    return 0;
}
