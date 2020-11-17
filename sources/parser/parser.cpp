#include "parser.h"

typedef Token::TokName TokName;
typedef Token::TokType TokType;

/*  
 *  number      ::= [(\+|-)?(\.[0-9]+|[0-9]+\.?[0-9]*)((e|E)(\+|-)?[0-9]+)?] | constants
 *  variable    ::= [A-Za-z] | greek letters
 *  function    ::= functions
 *  binop       ::= + | - | * | ** | xx | // | / | ^ | % 
 */


// TODO: add support for intermediate expressions with super/subscripts
std::unordered_map<TokName, int> Parser::bin_op_precedence = 
{
    {TokName::PLUS, 100}, {TokName::MINUS, 100}, 
    {TokName::AST, 200}, {TokName::CDOT, 200}, {TokName::DIV, 200}, {TokName::FRAC, 200}, {TokName::PERCENT, 200}, 
    {TokName::CROSS, 300}, {TokName::SUP, 400}
};


// only return precedence of binary operators
// any non-binary operator will have precedence -1
int Parser::getTokPrecedence()
{
    if (cur_tok == nullptr || cur_tok->get_type() != TokType::OP)    return -1;
    TokName name = cur_tok->get_name();
    return bin_op_precedence.count(name) == 0 ? -1 : bin_op_precedence[name];
}


// numberexpr      
//          ::= number
NumberExprAst* Parser::parseNum()
{
    if (cur_tok == nullptr || cur_tok->get_type() != TokType::NUM) return nullptr;
    NumberExprAst* num;
    switch (cur_tok->get_name()) 
    {
        case TokName::NUMERICAL:
            num = new NumberExprAst(TokName::NUMERICAL, cur_tok->get_num_value());
            break;
        case TokName::E:
            num = new NumberExprAst(TokName::E);
            break;
        case TokName::I:
            num = new NumberExprAst(TokName::I);
            break;
        case TokName::IDENTITY_MATRIX:
            num = new NumberExprAst(TokName::IDENTITY_MATRIX);
            break;
        default:
            num = nullptr;
            break;
    }
    return num;
}


// matrixexpr      
//          ::= '[' bracketexpr (',' bracketexpr)* ']'
MatrixExprAst* Parser::parseMatrix()
{
    if (cur_tok == nullptr || cur_tok->get_name() != TokName::LSB)  return nullptr;
    getNextToken();

    MatrixExprAst* matrix = new MatrixExprAst;
    MatrixExprAst* row = parseBracket();
    if (parseBracket == nullptr)
    {
        delete matrix;
        throw std::domain_error("Invalid use of brackets for a non-matrix expression.");
    }
    auto col_size = row->entries.size();
    matrix->entries.push_back(row);
    getNextToken();

    while (cur_tok != nullptr && cur_tok->get_name() == TokName::COMMA)
    {
        row = parseBracket();
        if (parseBracket == nullptr)
        {
            delete matrix;
            throw std::domain_error("Invalid entries for a matrix.");
        }
        if (row->entries.size() != col_size)
        {
            delete matrix;
            throw std::domain_error("Column size mismatch.");
        }
        matrix->entries.push_back(row);
        getNextToken();
    }

    if (cur_tok->get_name() != TokName::RSB)
    {
        delete matrix;
        throw std::domain_error("Missing right square bracket.");
    }
    getNextToken();
    return matrix;
}


// bracketexpr     
//          ::= '[' expression (',' expression)* ']'
MatrixExprAst* Parser::parseBracket()
{
    if (cur_tok == nullptr || cur_tok->get_name() != TokName::LSB)  return nullptr;
    MatrixExprAst* row = new MatrixExprAst;
    getNextToken();

    try 
    {
        ExprAst* entry = parseExpr();
        row->entries.push_back(entry);
        getNextToken();

        while (cur_tok != nullptr && cur_tok->get_name() == TokName::COMMA)
        {
            getNextToken();
            entry = parseExpr();
            row->entries.push_back(entry);
        }

        if (cur_tok == nullptr || cur_tok->get_name() != TokName::RSB)
        {
            delete row;
            return nullptr;
        }
        return row;
    }
    catch (...)     // catch all exceptions but do not rethrow
    {
        delete row;
        return nullptr;     // only let top-level parseMatrix() handle exceptions
    } 
}


// parenexpr       
//          ::= '(' expression ')'
//          ::= '|' expression '|'
ExprAst* Parser::parseParen()
{
    if (cur_tok == nullptr)     return nullptr;
    try
    {
        if (cur_tok->get_name() == TokName::LP)
        {
            getNextToken();
            ExprAst* temp = parseExpr();
            if (cur_tok->get_name() != TokName::RP)
            {
                delete temp;
                throw std::domain_error("Missing right parenthesis.");
            }

            getNextToken();
            return temp;
        }
        
        if (cur_tok->get_name() == TokName::TEXTBAR)
        {
            getNextToken();
            ExprAst* temp = parseExpr();
            if (cur_tok->get_name() != TokName::TEXTBAR)
            {
                delete temp;
                throw std::domain_error("Missing closing textbar.");
            }

            FunctionExprAst* abs = new FunctionExprAst(TokName::ABS, std::vector<ExprAst*>{temp});
            getNextToken();
            return abs;
        }
    }
    catch(...)
    {
        throw;      // rethrow, leave the exceptions to parsePrimary()
    }
}


// identifierexpr  
//          ::= variable
//          ::= function ( number | variable )
//          ::= function '(' expression (',' expression)* ')'
ExprAst* Parser::parseId()
{
    if (cur_tok == nullptr)     return nullptr;
    if (cur_tok->get_type() == TokType::ID)
    {
        ExprAst* var = new VariableExprAst(cur_tok->get_raw_value());
        getNextToken();
        return var;
    }
    if (cur_tok->get_type() != TokType::OP || getTokPrecedence() != -1)     
        return nullptr;     // binops are not functions

    FunctionExprAst* func = new FunctionExprAst(cur_tok->get_name());
    getNextToken();

    if (cur_tok->get_type() == TokType::NUM)
    {
        func->args.push_back(parseNum());
        getNextToken();
        return func;
    }

    if (cur_tok->get_type() == TokType::ID)
    {
        ExprAst* var = new VariableExprAst(cur_tok->get_raw_value());
        func->args.push_back(var);
        getNextToken();
        return func;
    }

    if (cur_tok->get_name() == TokName::LP)
    {
        getNextToken();
        try
        {
            ExprAst* expr = parseExpr();
            func->args.push_back(expr);
            while (cur_tok && cur_tok->get_name() == TokName::COMMA)
            {
                getNextToken();
                expr = parseExpr();
                func->args.push_back(expr);
            }

            if (cur_tok->get_name() != TokName::RP)
                throw std::domain_error("Missing right parenthesis for function.");

            getNextToken();
            return func;
        }
        catch(...)
        {
            throw;      // rethrow, parsePrimary() will handle
        }
    }

    return nullptr;
}


ExprAst* Parser::parsePrimary()
{
    // TODO
}