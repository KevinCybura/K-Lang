//
// Created by kevin Cybura on 2019-02-20.
//

#pragma once
#include <map>
#include <memory>
#include "AST.h"

struct CurTok {
    int cur = 0;
    std::string IdentifierStr = "";
    double NumVal = 0.0;
    int LastChar = ' ';
};

void MainLoop(CurTok &x);
void HandleTopLevelExpression(CurTok &x);
void HandleExtern(CurTok &x);
void HandleDefinition(CurTok &x);
std::unique_ptr<FunctionAST> ParseTopLevelExpr(CurTok &x);
std::unique_ptr<PrototypeAST> ParseExtern(CurTok &x);
std::unique_ptr<PrototypeAST> ParseProtpye(CurTok &x);
std::unique_ptr<PrototypeAST> ParseProtpye(CurTok &x);
std::unique_ptr<ExprAST> ParseNumberExpr(CurTok &x);
std::unique_ptr<ExprAST> ParsePrimary(CurTok &x);
std::unique_ptr<ExprAST> ParseExpression(CurTok &x);
std::unique_ptr<ExprAST> ParseIdentifierExpr(CurTok &x);
std::unique_ptr<PrototypeAST> LogErrorP(const char *Str);
std::unique_ptr<ExprAST> LogError(const char *Str);
int getNextToken(CurTok &x);
int GetTokPrecedence(CurTok &x);
std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                       std::unique_ptr<ExprAST> LHS, CurTok &x);
static std::map<char, int> BinopPrecedence = {
    {'<', 10}, {'+', 20}, {'-', 20}, {'*', 40}};
