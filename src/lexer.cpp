//
// Created by kevin Cybura on 2019-02-20.
//
#include "lexer.h"

int getChar(std::ifstream& file) { return file.get(); }
/// gettok - Return the next token from standard input.
int gettok(CurTok& x) {
    // Skip any whitespace.
    while (isspace(x.LastChar)) x.LastChar = getChar(x.file);

    // First parse numbers because other identifiers shouldnt start with a number.
    if (isdigit(x.LastChar) || x.LastChar == '.') {  // Number: [0-9.]+
        std::string NumStr;
        do {
            NumStr += x.LastChar;
            x.LastChar = getChar(x.file);
        } while (isdigit(x.LastChar) || x.LastChar == '.');

        x.NumVal = strtod(NumStr.c_str(), 0);
        return tok_number;
    }

    // Parse keywords/variables.
    if (isalpha(x.LastChar)) {  // identifier: [a-zA-Z][a-zA-Z0-9]*
        x.IdentifierStr = x.LastChar;
        while (isalnum((x.LastChar = getChar(x.file)))) x.IdentifierStr += x.LastChar;

        if (x.IdentifierStr == "def") return tok_def;
        if (x.IdentifierStr == "extern") return tok_extern;
        return tok_identifier;
    }

    if (x.LastChar == '(') {
        x.LastChar = getChar(x.file);
        return '(';
    }

    if (x.LastChar == ')') {
        x.LastChar = getChar(x.file);
        return ')';
    }

    // Parse operator
    if(isOp(x.LastChar)) {
	    x.LastChar = getChar(x.file);
	    return tok_identifier;
    }

    // Ignore comments
    if (x.LastChar == '#') {
        // Comment until end of line.
        do
            x.LastChar = getChar(x.file);
        while (x.LastChar != EOF && x.LastChar != '\n' && x.LastChar != '\r');

        if (x.LastChar != EOF) return gettok(x);
    }
    // Check for end of file.  Don't eat the EOF.
    if (x.LastChar == EOF) return tok_eof;

    // Otherwise, just return the character as its ascii value.
    LogErrorP("Unhandled case");
    return x.LastChar;
}

bool isOp(int c) {
    switch (c) {
        case '+':
            return true;
	case '-':
	    return true;
	case '*':
	    return true;
	default:
	    return false;
    }
}
