#ifndef MATH_WRAPPER_H
#define MATH_WRAPPER_H

#include "R.h"
#include "Polynomial.h"
#include "Long.h"
#include "LongComplex.h"
#include "Fraction.h"
#include "Double.h"
#include "DoubleComplex.h"
#include <vector>

// a generic wrapper for self-implemented math library
// encapsulate some operations and add a class ROperand
// to unify matrices and non-matrix expressions

// note that the math library is not designed to be a high performance
// scientific calculation/linear algebra library, hence optimization is very limited

class ROperand 
{
    friend class BinaryExprAst;
    friend class MatrixExprAst;
    friend class FunctionExprAst;
    friend class Parser;

public:
    enum class Type { NOR, MAT }; 

    explicit ROperand(R value): value(value) { type = Type::NOR; }
    explicit ROperand(size_t row_num): mat(row_num) { type = Type::MAT; } 
    ~ROperand() = default;

    ROperand operator+(ROperand rhs);
    ROperand operator-(ROperand rhs);
    ROperand operator*(ROperand rhs);
    ROperand operator/(ROperand rhs);
    ROperand operator^(int rhs);

private:
    Type type;
    std::vector<std::vector<R>> mat;
    R value;
};


inline R newInt(long value) { return R{new Long{value}}; }

inline R newComplex(long real, long imag) { return R{new LongComplex{real, imag}}; }

inline R newFrac(R num, R denom)
{
    if (R::complexify_if_needed(num, denom))
    {
        if (denom.is_zero())    throw std::overflow_error("Divide by zero.");
        return R{new Fraction{num, denom}};
    }
    throw std::invalid_argument("Numerator and denominator are in incompatible type.");
}

inline R newTerm(int degree)
{
    R* arr = new R[degree + 1];
    arr[degree] = R{new Fraction{newInt(1L), newInt(1L).promote_one()}};
    R result = R{new Polynomial{arr, degree + 1}};
    delete[] arr;
    return result;
}


#endif /* MATH_WRAPPER_H */
