//
// Created by kevin Cybura on 2019-02-20.
//
#include "parser.h"
#include <llvm/ADT/STLExtras.h>
#include <iostream>
#include <memory>
#include "AST.h"
#include "lexer.h"
#include "parser.h"

int getNextToken(CurTok& x) { return x.cur = gettok(x); }

std::unique_ptr<ExprAST> LogError(const char* Str) {
    fprintf(stderr, "LogError: %s\n", Str);
    abort();
    return nullptr;
}

std::unique_ptr<PrototypeAST> LogErrorP(const char* Str) {
    LogError(Str);
    abort();
    return nullptr;
}

std::unique_ptr<ExprAST> ParseNumberExpr(CurTok& x) {
    auto Result = llvm::make_unique<NumberExprAST>(x.NumVal);
    getNextToken(x);
    return std::move(Result);
}

std::unique_ptr<ExprAST> ParseParenExpr(CurTok& x) {
    getNextToken(x);
    auto V = ParseExpression(x);
    if (!V) return nullptr;

    if (x.cur != ')') return LogError("expected ')'");
    getNextToken(x);
    return V;
}

std::unique_ptr<ExprAST> ParseIdentifierExpr(CurTok& x) {
    std::string IdName = x.IdentifierStr;

    getNextToken(x);

    if (x.cur != ')') return llvm::make_unique<VariableExprAST>(IdName);

    getNextToken(x);
    std::vector<std::unique_ptr<ExprAST>> Args;
    if (x.cur != ')') {
        while (1) {
            if (auto Arg = ParseExpression(x))
                Args.push_back(std::move(Arg));
            else
                return nullptr;

            if (x.cur == ')') break;

            if (x.cur != ',')
                return LogError("Expected ')' or ',' in argument list");
            getNextToken(x);
        }
    }

    getNextToken(x);
    return llvm::make_unique<CallExprAST>(IdName, std::move(Args));
}

int GetTokPrecdence(CurTok& x) {
    if (!isascii(x.cur)) return -1;

    int TokPrec = BinopPrecedence[x.cur];
    if (TokPrec < 0) return -1;
    return TokPrec;
}

std::unique_ptr<ExprAST> ParsePrimary(CurTok& x) {
    switch (x.cur) {
        default:
            return LogError("unknown token when expecting an expression");
        case tok_identifier:
            return ParseIdentifierExpr(x);
        case tok_number:
            return ParseNumberExpr(x);
        case '(':
            return ParseParenExpr(x);
    }
}

std::unique_ptr<ExprAST> ParseExpression(CurTok& x) {
    auto LHS = ParsePrimary(x);
    if (!LHS) return nullptr;
    return ParseBinOpRHS(0, std::move(LHS), x);
}

std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                       std::unique_ptr<ExprAST> LHS,
                                       CurTok& x) {
    while (true) {
        int TokPrec = GetTokPrecdence(x);

        if (TokPrec < ExprPrec) return LHS;

        int BinOp = x.cur;
        getNextToken(x);

        auto RHS = ParsePrimary(x);
        if (!RHS) return nullptr;
        int NextPrec = GetTokPrecdence(x);
        if (TokPrec < NextPrec) {
            RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS), x);
            if (!RHS) return nullptr;
        }

        LHS = llvm::make_unique<BinaryExprAST>(BinOp, std::move(LHS),
                                               std::move(RHS));
    }
}

std::unique_ptr<PrototypeAST> ParseProtpye(CurTok& x) {
    if (x.cur != tok_identifier)
        return LogErrorP("Expected function name in protoype");

    std::string FnName = x.IdentifierStr;
    getNextToken(x);

    if (x.cur != '(') return LogErrorP("Expected '('");

    std::vector<std::string> ArgNames;

    while (getNextToken(x) == tok_identifier)
        ArgNames.push_back(x.IdentifierStr);

    if (x.cur != ')') return LogErrorP("Expected ')' in prototype");

    getNextToken(x);

    return llvm::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

std::unique_ptr<FunctionAST> ParseDefinition(CurTok& x) {
    getNextToken(x);
    auto Proto = ParseProtpye(x);

    if (!Proto) return nullptr;

    if (auto E = ParseExpression(x))
        return llvm::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    return nullptr;
}

std::unique_ptr<PrototypeAST> ParseExtern(CurTok& x) {
    getNextToken(x);
    return ParseProtpye(x);
}

std::unique_ptr<FunctionAST> ParseTopLevelExpr(CurTok& x) {
    if (auto E = ParseExpression(x)) {
        auto Proto =
            llvm::make_unique<PrototypeAST>("", std::vector<std::string>());
        return llvm::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }
    return nullptr;
}

void HandleTopLevelExpression(CurTok& x) {
    if (ParseTopLevelExpr(x)) {
        fprintf(stderr, "Parsed a top-level expr\n");
    } else {
        getNextToken(x);
    }
}

void HandleExtern(CurTok& x) {
    if (ParseExtern(x)) {
        fprintf(stderr, "Parsed an extern\n");
    } else {
        getNextToken(x);
    }
}

void HandleDefinition(CurTok& x) {
    if (ParseDefinition(x)) {
        fprintf(stderr, "Parsed a function definition. \n");
    } else {
        getNextToken(x);
    }
}

void MainLoop(CurTok& x) {
    while (true) {
        fprintf(stderr, "ready> ");
        switch (x.cur) {
            case tok_eof:
                return;
            case ';':
                getNextToken(x);
                break;
            case tok_def:
                HandleDefinition(x);
                break;
            case tok_extern:
                HandleExtern(x);
                break;
            default:
                HandleTopLevelExpression(x);
                break;
        }
    }
}

