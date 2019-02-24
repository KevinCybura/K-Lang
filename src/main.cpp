#include <iostream>
#include <functional>
#include <llvm/ADT/STLExtras.h>
#include "AST.h"
#include "lexer.h"
#include "parser.h"

int main() {
	fprintf(stderr, "ready> ");
	CurTok x = CurTok{};
	getNextToken(x);

	MainLoop(x);
	return 0;
}



