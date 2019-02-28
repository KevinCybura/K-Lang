//
// Created by kevin Cybura on 2019-02-25.
//

#include <fstream>
#include <string>
#include "gtest/gtest.h"
#include "lexer.h"
#include "parser.h"

using namespace std::string_literals;

std::fstream temp_file(const std::string& to_write) {
    std::fstream file("./lang.k"s, std::fstream::out);
    file.write(to_write.c_str(), to_write.size());
    file.close();
    file.open("./lang.k"s, std::fstream::in);
    return file;
}

TEST(lexer_parse_ident, should_parser_def) {
    CurTok x = CurTok{};
    x.file = std::move(temp_file("def"s));
    getNextToken(x);
    EXPECT_EQ(x.IdentifierStr, "def");
    std::remove("lang.k");
}

TEST(lexer_parse_ident, should_parser_def_with_extra_chars) {
    CurTok x = CurTok{};
    x.file = std::move(temp_file(" def "s));
    getNextToken(x);
    EXPECT_EQ(x.IdentifierStr, "def");

    x.file = std::move(temp_file("\ndef\n"s));
    getNextToken(x);
    EXPECT_EQ(x.IdentifierStr, "def");

    x.file = std::move(temp_file("def()"s));
    getNextToken(x);
    EXPECT_EQ(x.IdentifierStr, "def");
    getNextToken(x);
    EXPECT_EQ(x.cur, static_cast<int>('('));
    getNextToken(x);
    EXPECT_EQ(x.cur, static_cast<int>(')'));
    std::remove("lang.k");
}

TEST(lexer_parse_ident, should_parse_function_signiture) {
    CurTok x = CurTok{};
    x.file = std::move(temp_file("def foo(x y)"s));
    getNextToken(x);
    EXPECT_EQ(x.IdentifierStr, "def");
    getNextToken(x);
    EXPECT_EQ(x.IdentifierStr, "foo");
    getNextToken(x);
    EXPECT_EQ(x.cur, static_cast<int>('('));
    getNextToken(x);
    EXPECT_EQ(x.IdentifierStr, "x");
    getNextToken(x);
    EXPECT_EQ(x.IdentifierStr, "y");
    getNextToken(x);
    EXPECT_EQ(x.cur, static_cast<int>(')'));
    std::remove("lang.k");
}

TEST(parser_tests, should_parse_proto) {
    CurTok x = CurTok{};
    x.file = std::move(temp_file("def foo()"s));
    getNextToken(x);
    EXPECT_EQ(x.IdentifierStr, "def");
    getNextToken(x);
    EXPECT_EQ(x.IdentifierStr, "foo");
    auto proto = ParseProtpye(x);
    ASSERT_NE(proto, nullptr);
    EXPECT_EQ(proto->getName(), "foo");
    EXPECT_EQ(proto->numArgs(), 0);
    
}













