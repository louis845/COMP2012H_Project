#ifndef EXPR_AST_H_
#define EXPR_AST_H_

#include "lexer.h"
#include "tokens.h"
#include "math_wrapper.h"
#include <vector>
#include <string>
#include <cmath>
#include <unordered_set>
#include <armadillo>


class ExprAst
{
public:
    virtual ~ExprAst() = default;
    virtual ROperand evalR() = 0;               // using our own implementation of linear algebra library
    virtual ArmaOperand eval() = 0;             // using Armadillo, for approximated solutions
    virtual std::string genAsciiMath() const = 0;   // generate AsciiMath formated strings
};


// AST node for numerical values and constants, including identity matrices
class NumberExprAst : public ExprAst
{
    friend class Parser;
    friend class BinaryExprAst;

public:
    NumberExprAst(Token::TokName name, const std::string& raw): name(name), raw(raw) 
    {
        if (name == Token::TokName::INTEGRAL)    int_value = stol(raw);
        else if (name == Token::TokName::FLOAT)    float_value = stod(raw);
    }
         
    ROperand evalR() override;
    ArmaOperand eval() override;
    std::string genAsciiMath() const override { return raw; }; 

private:
    static constexpr long double PI = 3.141592653589793;        // obsoleted
    static constexpr long double E = 2.718281828459045;         // replaced by arma::datum::pi

    Token::TokName name{Token::TokName::INTEGRAL};
    std::string raw{""};
    std::string tex{""};
    double float_value{0.0L};
    long int_value{0L};
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
    ArmaOperand eval() override;
    std::string genAsciiMath() const override;

private:
    std::vector<std::vector<ExprAst*>> entries;
    std::string tex{""};
};



// TODO: add feature that allows user to assign specific values to variables
class VariableExprAst : public ExprAst
{
    friend class Parser;

public:
    VariableExprAst(const std::string& name): name(name) {}
    std::string get_name() const { return name; }

    ROperand evalR() override;
    ArmaOperand eval() override;
    std::string genAsciiMath() const override { return name; }

private:
    std::string name;
    std::string tex{""};
};


class BinaryExprAst : public ExprAst
{
    friend class Parser;

public:
    BinaryExprAst(Token::TokName op, const std::string& raw, ExprAst* lhs, ExprAst* rhs):
        op(op), raw(raw), lhs(lhs), rhs(rhs) {}

    ~BinaryExprAst() { delete lhs; delete rhs; }

    ROperand evalR() override;
    ArmaOperand eval() override;
    std::string genAsciiMath() const override;

private:
    static std::unordered_map<Token::TokName, int> precedence;  // cannot reuse code from Parser class due to cross-referencing
    Token::TokName op;
    std::string raw{""};
    std::string tex{""};
    ExprAst* lhs, *rhs;
};


class FunctionExprAst : public ExprAst
{
    friend class Parser;

public:
    FunctionExprAst(Token::TokName op, const std::string& raw): op(op), raw(raw) {}
    
    ~FunctionExprAst() { for (auto arg : args)  delete arg; }

    ROperand evalR() override;
    ArmaOperand eval() override;
    std::string genAsciiMath() const override;

private:
    Token::TokName op;
    std::string raw{""};
    std::string tex{""};
    std::vector<ExprAst*> args;
};


#endif /* EXPR_AST_H_ */