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
#include <complex>
#include <armadillo>

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
    enum class Type { NOR, MAT, IMAT };             // IMAT : identity matrix 

    ROperand(): type(Type::IMAT) {}
    explicit ROperand(R value): type(Type::NOR), value(value) {}
    explicit ROperand(size_t row_num): type(Type::MAT), mat(row_num) {} 
    ~ROperand() = default;

    ROperand operator-();
    ROperand operator+(ROperand rhs);           // use PBV and non-const since the implementation 
    ROperand operator-(ROperand rhs);           // may change the values of the operands i.e. complexify
    ROperand operator*(ROperand rhs);
    ROperand operator/(ROperand rhs);
    ROperand operator^(int rhs);

private:
    Type type;
    std::vector<std::vector<R>> mat;
    R value;            // if type == NOR, value represents numerical value, otherwise the coefficient of IMAT
};


class ArmaOperand
{
    friend class BinaryExprAst;
    friend class MatrixExprAst;
    friend class FunctionExprAst;
    friend class Parser;

public:
    enum class Type { NOR, MAT, IMAT };

    ArmaOperand(): type(Type::IMAT) {}
    explicit ArmaOperand(const std::complex<double>& value): type(Type::NOR), value(value) {}
    explicit ArmaOperand(const arma::cx_mat& mat): type(Type::MAT), mat(mat) {}
    explicit ArmaOperand(double real, double imag=0.0): type(Type::NOR), value(real, imag) {}
    ArmaOperand(size_t row, size_t col): type(Type::MAT), mat(row, col, arma::fill::none) {}
    ArmaOperand(Type type, const std::complex<double>& coeff): type(type), value(coeff) {}          // for IMAT

    ArmaOperand operator+(const ArmaOperand& rhs) const;
    ArmaOperand operator-(const ArmaOperand& rhs) const;
    ArmaOperand operator*(const ArmaOperand& rhs) const;
    ArmaOperand operator/(const ArmaOperand& rhs) const;
    ArmaOperand operator^(const ArmaOperand& rhs) const;

    bool almostReal() const
    {
        return fabs(value.imag()) <= std::numeric_limits<double>::epsilon();
    }

private:
    Type type;
    arma::cx_mat mat;              // entries by default are stored in double, cx = complex
    std::complex<double> value{1.0, 0.0};
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
