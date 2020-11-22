#ifndef EXPR_AST_H_
#define EXPR_AST_H_

#include "lexer.h"
#include "tokens.h"
#include "math_wrapper.h"
#include <vector>
#include <string>
#include <unordered_set>
#include <armadillo>


class ExprAst
{
public:
    virtual ~ExprAst() = default;
    virtual ROperand evalR() = 0;
};


class NumberExprAst : public ExprAst
{
    friend class Parser;
    friend class BinaryExprAst;

public:
    NumberExprAst(Token::TokName name, const std::string& raw): name(name), raw(raw) 
    {
        if (name == Token::TokName::INTEGRAL)    int_value = stoll(raw);
        else if (name == Token::TokName::FLOAT)    float_value = stoll(raw);
    }
         
    ROperand evalR() override; 

    long double evalArma() const;

private:
    static constexpr long double PI = 3.141592653589793;
    static constexpr long double E = 2.718281828459045;

    Token::TokName name{Token::TokName::INTEGRAL};
    std::string raw{""};
    long double float_value{0.0L};
    long long int_value{0L};
};


class MatrixExprAst : public ExprAst
{
    friend class Parser;

public:
    MatrixExprAst() = default;
    ~MatrixExprAst() 
    { 
        for (auto row : entries)
            for (auto col : row)
                delete col;
    }

    ROperand evalR() override;

private:
    std::vector<std::vector<ExprAst*>> entries;
};


class VariableExprAst : public ExprAst
{
    friend class Parser;

public:
    VariableExprAst(const std::string& name): name(name) { var_table.emplace(name); }
    std::string get_name() const { return name; }

    ROperand evalR() override;

private:
    static std::unordered_set<std::string> var_table;
    std::string name;
};


class BinaryExprAst : public ExprAst
{
    friend class Parser;

public:
    BinaryExprAst(Token::TokName op, const std::string& raw, ExprAst* lhs, ExprAst* rhs):
        op(op), raw(raw), lhs(lhs), rhs(rhs) {}

    ~BinaryExprAst() { delete lhs; delete rhs; }

    ROperand evalR() override;

private:
    Token::TokName op;
    std::string raw{""};
    ExprAst* lhs, *rhs;
};


class FunctionExprAst : public ExprAst
{
    friend class Parser;

public:
    FunctionExprAst(Token::TokName op, const std::string& raw): op(op), raw(raw) {}
    
    ~FunctionExprAst() { for (auto arg : args)  delete arg; }

    ROperand evalR() override;

private:
    Token::TokName op;
    std::string raw{""};
    std::vector<ExprAst*> args;
};


#endif /* EXPR_AST_H_ */