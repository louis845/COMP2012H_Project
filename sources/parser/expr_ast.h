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


struct Info         // for communications between parser and gui / step-by-step functions
{
    Info() = default;
    Info(const Info&) = delete;
    Info& operator=(const Info&) = delete;
    ~Info();

    bool success{true};
    std::exception* err{nullptr};
    int engine_chosen{0};           // user chosen engine, 0 = auto, 1 = R, 2 = Armadillo
    int engine_used{1};             // 1 for using R class, 2 for using Armadillo
    bool float_exists{false};
    bool var_exists{false};
    bool func_exists{false};        // true only if function that R cannot handle appears

    std::vector<std::pair<Token::TokName, R**>> parsed_mat;    
    // contains all matrices and corresponding operators the input for step-by-step functions
    // N.B. the 2D arrays are deep-copied hence needed to be deallocated
    // after calling the step-by-step functions

    std::vector<std::pair<int, int>>    mat_size;  // corresponding sizes, row first

    std::string interpreted_input{""};  // an AsciiMath format string of the interpreted input
    std::string eval_result{""};        // computation result in LaTeX format
                                        // N.B. both are raw strings and are NOT enclosed by delimiters
                                        // i.e. $$ for LaTeX and ` for AsciiMath

    std::string warning{""};

    void clear();
    void addMat(ROperand operand, Token::TokName op);
};


class ExprAst
{
public:
    virtual ~ExprAst() = default;
    virtual ROperand evalR(Info& res) = 0;               // using our own implementation of linear algebra library
    virtual ArmaOperand eval() = 0;             // using Armadillo, for approximated solutions
    virtual std::string genAsciiMath() const = 0;   // generate AsciiMath formated strings
};


// AST node for numerical values and constants, including identity matrices
class NumberExprAst : public ExprAst
{
    friend class Parser;
    friend class BinaryExprAst;

public:
    NumberExprAst(Token::TokName name, const std::string& raw);
         
    ROperand evalR(Info& res) override;
    ArmaOperand eval() override;
    std::string genAsciiMath() const override { return raw; }; 

private:
    static constexpr long double PI = 3.141592653589793;        // obsoleted
    static constexpr long double E = 2.718281828459045;         // replaced by arma::datum::pi

    Token::TokName name{Token::TokName::INTEGRAL};
    std::string raw{""};
    std::string tex{""};
    bool scientific{false};                 // whether scientific notation is used
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

    ROperand evalR(Info& res) override;
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

    ROperand evalR(Info& res) override;
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

    ROperand evalR(Info& res) override;
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

    ROperand evalR(Info& res) override;
    ArmaOperand eval() override;
    std::string genAsciiMath() const override;

private:
    Token::TokName op;
    std::string raw{""};
    std::string tex{""};
    std::vector<ExprAst*> args;
};


#endif /* EXPR_AST_H_ */