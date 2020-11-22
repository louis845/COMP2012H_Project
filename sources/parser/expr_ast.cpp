#include "expr_ast.h"
using std::vector;

typedef Token::TokName TokName;
typedef Token::TokType TokType;

std::unordered_set<std::string> VariableExprAst::var_table;

ROperand NumberExprAst::evalR()
{
    if (name == TokName::INTEGRAL)  return ROperand(newInt(static_cast<long>(int_value)));
    if (name == TokName::FLOAT)     // not really recommended, use Armadillo for approximated solutions
    {
        long int_part = static_cast<long>(float_value);
        long float_part = static_cast<long>((float_value - int_part) * 1e6);    // naive implementation, avoid to use it
        return ROperand(newFrac(newInt(int_part), newInt(1L)) + newFrac(newInt(float_part), newInt(1e6L)));
    }
    if (name == TokName::E)     return ROperand(newInt(E));
    if (name == TokName::PI)    return ROperand(newInt(PI));
    if (name == TokName::I)     return ROperand(newComplex(1L, 1L));
    throw std::invalid_argument("Fail to convert the numerical value to an R object.");
}

ROperand MatrixExprAst::evalR()
{
    ROperand matrix(entries.size());
    for (size_t row = 0; row < entries.size(); ++row)
        for (size_t col = 0; col < entries[row].size(); ++col)
        {
            ROperand entry = entries[row][col]->evalR();
            
            if (entry.type == ROperand::Type::MAT)
                throw std::invalid_argument("Nested matrices are not supported currently.");

            matrix.mat[row].push_back(entry.value);
        }
    return matrix;
}

ROperand VariableExprAst::evalR()
{
    if (var_table.size() != 1)
        throw std::invalid_argument("Warning: the expression has more than one variable.");
    
    return ROperand(newTerm(1));
}

ROperand BinaryExprAst::evalR()
{
    ROperand lhsR = lhs->evalR();
    ROperand rhsR = rhs->evalR();
    
    switch (op)
    {
        case TokName::PLUS:
            return lhsR + rhsR;

        case TokName::MINUS:
            return lhsR - rhsR;

        case TokName::AST: case TokName::CDOT: case TokName::CROSS:         // so far do not distinguish cross cdot and *
            return lhsR * rhsR;

        case TokName::DIV: case TokName::FRAC:
            return lhsR / rhsR;

        case TokName::SUP:              // exponential, may be enhanced in the future
            if (typeid(*rhs) == typeid(NumberExprAst))
            {
                NumberExprAst* temp = dynamic_cast<NumberExprAst*>(rhs);
                if (temp->name == TokName::INTEGRAL)    return lhsR ^ (static_cast<int>(temp->int_value));
            }
            else throw std::runtime_error("Error: unsupported exponential operation, expecting a natural number as exponential.");       
            
        default:
            throw std::runtime_error("Error: unsupported binary operation.");   // mod may be added soon
    }
}


ROperand FunctionExprAst::evalR()
{
    // TODO: need to check the implementation for row reduction, gcd, etc
}