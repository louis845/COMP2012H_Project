#ifndef EXPR_AST_H_
#define EXPR_AST_H_

#include "parser/lexer.h"
#include "parser/tokens.h"
#include "utils/math_wrapper.h"
#include <vector>
#include <string>
#include <cmath>
#include <unordered_set>
#include <armadillo>


// Info object is used for communications between parser and gui
// The copy constructor and assignment are deleted since it is designed
// not to be copied when using, which is too computation expensive
// 
// instead, Parser::parse() and Parser::getInfo() will return a const Info&
struct Info         
{
    Info() = default;
    Info(const Info&) = delete;
    Info& operator=(const Info&) = delete;
    ~Info();

    bool success{true};             // will be false if parsing/evaluation failed
    std::string err_msg;            // updated whenever success=false or variable assignment failed
    
    int engine_chosen{0};           // user chosen engine, 0 = auto, 1 = R, 2 = Armadillo, 3 = linear system
    int engine_used{1};             // 1 for using R class, 2 for using Armadillo, 3 for parsing linear system
    
    bool float_exists{false};       // whether floating point number appears in user input
    bool var_exists{false};         // whether variables appears
    bool func_exists{false};        // true only if functions that R cannot handle appear


    // parsed_mat contains all matrices and corresponding operators in the input 
    // so that the step-by-step functions can retrieve the original data
    //
    // NOTE: the 2D arrays are deep-copied hence needed to be deallocated
    // after calling the step-by-step functions
    std::vector<std::pair<Token::TokName, R**>> parsed_mat;    
    

    std::vector<std::pair<int, int>>    mat_size;  // corresponding sizes for parsed_mat, row first

    std::string interpreted_input{""};  // an AsciiMath format string of the interpreted input


    // eval_result contains the computation result in LaTeX format
    // NOTE: both interpreted_input and eval_result are raw strings and are NOT enclosed by delimiters
    // i.e. $$ for LaTeX and ` for AsciiMath
    std::string eval_result{""};

    void clear();

    // Add a ROperand with corresponding operator to parsed_mat and
    // update the mat_size accordingly
    void addMat(ROperand operand, Token::TokName op);
};



// Base class for the nodes on the abstract syntax tree
class ExprAst
{
public:
    virtual ~ExprAst() = default;

    // Evaluate the expression in the subtree using our own 
    // implementation of linear algebra library i.e. R class
    //
    // Powerful when handling polynomial with parameters and exact solutions
    virtual ROperand evalR(Info& res) = 0;               

    // Evaluate using Armadillo, for approximated solutions and complicated
    // linear operations, as well as other functions
    virtual ArmaOperand eval() = 0;             

    // Generate AsciiMath formated strings for the subtree
    virtual std::string genAsciiMath() const = 0;
};


// AST node for numerical values and constants, including identity matrices
class NumberExprAst : public ExprAst
{
    friend class Parser;
    friend class BinaryExprAst;

public:
    // The TokName determines the attribute of the node
    // raw will hold the literal from the user input 
    // e.g. the string representation of the numbers
    NumberExprAst(Token::TokName name, const std::string& raw);
         
    ROperand evalR(Info& res) override;
    ArmaOperand eval() override;
    std::string genAsciiMath() const override; 

private:
    static constexpr long double PI = 3.141592653589793;        // obsoleted
    static constexpr long double E = 2.718281828459045;         // replaced by arma::datum::e

    Token::TokName name{Token::TokName::INTEGRAL};
    std::string raw{""};
    std::string tex{""};                    // unused
    bool scientific{false};                 // whether scientific notation is used
};


// Node type for matrices, each entry of the matrix will stored
// in a 2D vector whose elements are ExprAst*
// 
// Therefore, the grammar can be very flexible and general
// even allowing matrix embedded in another matrix, etc.
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


// Derived class representing variables in AST
class VariableExprAst : public ExprAst
{
    friend class Parser;

public:
    // when constructing a VariableExprAst node, parser need to pass
    // two pointers to variable look-up table so that the eval() and evalR()
    // can function correctly
    VariableExprAst(const std::string& name, std::map<std::string, ROperand>* r_table, 
                    std::map<std::string, ArmaOperand>* arma_table): name(name), r_table(r_table), 
                    arma_table(arma_table) {}

    std::string get_name() const { return name; }


    // evalR() will first check whether it is in linear system mode, if so
    // it will not search the look-up table for the value since in linear system
    // variables are treated as unknowns to be solved
    //
    // Otherwise, it will search the lookup table and attempt to retrieve the corresponding
    // value. If there is not any, it will assume the variable is a parameter and return
    // a new term with degree 1
    ROperand evalR(Info& res) override;


    // eval() will only check whether the variable can be found in the lookup table
    // if not, it will throw a std::invalid_argument
    ArmaOperand eval() override;
    std::string genAsciiMath() const override;

protected:
    std::map<std::string, ROperand>* r_table;
    std::map<std::string, ArmaOperand>* arma_table;
    std::string name;
    std::string tex{""};
};


// Node for binary operations
class BinaryExprAst : public ExprAst
{
    friend class Parser;

public:
    // The constructor accept the name of the operator and its raw representation
    // as well as two ExprAst pointers pointing to the left and right operands
    BinaryExprAst(Token::TokName op, const std::string& raw, ExprAst* lhs, ExprAst* rhs):
        op(op), raw(raw), lhs(lhs), rhs(rhs) {}

    // The whole AST can be deleted recursively just by deleting the root
    ~BinaryExprAst() { delete lhs; delete rhs; }

    ROperand evalR(Info& res) override;
    ArmaOperand eval() override;
    std::string genAsciiMath() const override;

private:
    // precedence is a static hashmap storing the precedence of the binary operators
    // It is used in genAsciiMath() to determine whether parentheses are needed when
    // reconstructing the input from the AST
    static std::unordered_map<Token::TokName, int> precedence;

    Token::TokName op;
    std::string raw{""};
    std::string tex{""};
    ExprAst* lhs, *rhs;
};


// Node class for functions, each argument of the function
// is stored as a ExprAst* in a vector
class FunctionExprAst : public ExprAst
{
    friend class Parser;

public:
    FunctionExprAst(Token::TokName op, const std::string& raw): op(op), raw(raw) {}
    
    // for recursively delete the whole AST
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
