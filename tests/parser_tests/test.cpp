//
// Created by kevin Cybura on 2019-02-25.
//

#include <fstream>
#include "gtest/gtest.h"
#include "lexer.h"
#include "parser.h"
#include <string>

using namespace std::string_literals;

TEST(lexer_test, lexer) {
    std::fstream file("./lang.k"s);
    file << "def";
    CurTok x = CurTok{};
    x.file = std::move(file);
    getNextToken(x);
    EXPECT_EQ(x.IdentifierStr, "def");
    std::remove("lang.k");
}
