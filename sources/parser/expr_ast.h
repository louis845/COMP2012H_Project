#ifndef EXPR_AST_H_
#define EXPR_AST_H_

#include "lexer.h"
#include "tokens.h"
#include <vector>
#include <string>


class ExprAst
{
public:
    virtual ~ExprAst() = default;
};


class NumberExprAst : public ExprAst
{
    friend class Parser;

public:
    NumberExprAst(Token::TokName name=Token::TokName::NUMERICAL, const long double& value=0.0L):
         name(name), value(value) {}
         
    long double eval() const { return value; }

private:
    Token::TokName name{Token::TokName::NUMERICAL};
    long double value{0.0L};
};


class MatrixExprAst : public ExprAst
{
    friend class Parser;

public:
    MatrixExprAst() = default;
    MatrixExprAst(std::vector<ExprAst*> entries): entries(entries) {}
    ~MatrixExprAst() { for (auto entry : entries)   delete entry; }

    MatrixExprAst* eval() const;

private:
    std::vector<ExprAst*> entries;
};


class VariableExprAst : public ExprAst
{
    friend class Parser;

public:
    VariableExprAst(const std::string& name): name(name) {}
    std::string get_name() const { return name; }

private:
    std::string name;
};


class BinaryExprAst : public ExprAst
{
    friend class Parser;

public:
    BinaryExprAst(Token::TokName op, ExprAst* left, ExprAst* right):
        op(op), left(left), right(right) {}
    ExprAst* eval() const;

    ~BinaryExprAst() { delete left; delete right; }

private:
    Token::TokName op;
    ExprAst* left, *right;
};


class FunctionExprAst : public ExprAst
{
    friend class Parser;

public:
    FunctionExprAst(Token::TokName op): op(op) {}
    FunctionExprAst(Token::TokName op, const std::vector<ExprAst*>& args):
        op(op), args(args) {}
    ~FunctionExprAst() { for (auto arg : args)  delete arg; }

    ExprAst* eval() const;

private:
    Token::TokName op;
    std::vector<ExprAst*> args;
};


#endif /* EXPR_AST_H_ */