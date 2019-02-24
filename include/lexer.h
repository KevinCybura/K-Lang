//
// Created by kevin Cybura on 2019-02-20.
//

#pragma once
#include <string>
#include "parser.h"

// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.
enum Token {
    tok_eof = -1,

    // commands
    tok_def = -2,
    tok_extern = -3,

    // primary
    tok_identifier = -4,
    tok_number = -5,
};

int gettok(CurTok& x);
bool isOp(int c) ;
