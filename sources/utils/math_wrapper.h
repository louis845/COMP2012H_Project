#ifndef MATH_WRAPPER_H
#define MATH_WRAPPER_H

#include "math/R.h"
#include "math/polynomial/Polynomial.h"
#include "math/long/Long.h"
#include "math/long/LongComplex.h"
#include "math/fraction/Fraction.h"
#include "math/double/Double.h"
#include "math/double/DoubleComplex.h"
#include <utility>
#include <vector>
#include <complex>
#include <armadillo>

// a generic wrapper for self-implemented math library R
// and the Armadillo library
//
// encapsulate some operations and unify matrices and 
// scalars

// NOTE: the R math library is not designed to be a high performance
// scientific calculation/linear algebra library, hence optimization is very limited


// Wrapper class for R scalar and matrix
class ROperand 
{
    friend class BinaryExprAst;
    friend class MatrixExprAst;
    friend class FunctionExprAst;
    friend class Parser;
    friend class Info;

public:
    // To distinguish the type of ROperand
    // NOR: scalar  MAT: matrix  IMAT: identity matrix
    enum class Type { NOR, MAT, IMAT };

    ROperand(): type(Type::IMAT) {}

    // Instantiate from an R object
    explicit ROperand(R value): type(Type::NOR), value(std::move(value)) {}

    // Instantiate an ROperand with type matrix, instantiate
    // the vector mat with corresponding number of rows
    explicit ROperand(size_t row_num): type(Type::MAT), mat(row_num) {} 
    
    // Instantiate an ROperand with type matrix with a 2D array
    // R**, along with its size
    explicit ROperand(R** matR, int row_num, int col_num);
    ~ROperand() = default;

    ROperand operator-();
    ROperand operator+(ROperand rhs);           // Use PBV and non-const parameter since the operation
    ROperand operator-(ROperand rhs);           // may change the values of the operands e.g. complexify
    ROperand operator*(ROperand rhs);
    ROperand operator/(ROperand rhs);
    ROperand operator^(int rhs);
    ROperand operator+=(ROperand rhs);
    ROperand operator-=(ROperand rhs);

    // Generate LaTeX formated strings for rendering
    std::string genTex() const;

private:
    Type type;
    std::vector<std::vector<R>> mat;

    // If type == NOR, value represents a scalar value
    // otherwise it represents the coefficient of the identity matrix
    R value{new Long{1L}};
};


// Wrapper class for Armadillo, unify scalars, matrices and
// identity matrices
//
// Scalars in all computation using Armadillo are assumed to be complex
// numbers, and matrices are using complex entries by default 
class ArmaOperand
{
    friend class BinaryExprAst;
    friend class MatrixExprAst;
    friend class FunctionExprAst;
    friend class Parser;

public:
    enum class Type { NOR, MAT, IMAT };

    ArmaOperand(): type(Type::IMAT) {}

    // Instantiate a scalar from a complex number
    explicit ArmaOperand(const std::complex<double>& value): type(Type::NOR), value(value) {}

    // Copy the matrix from an Armadillo built-in matrix type
    explicit ArmaOperand(arma::cx_mat mat): type(Type::MAT), mat(std::move(mat)) {}

    // Instantiate a scalar by passing real and imaginary parts
    explicit ArmaOperand(double real, double imag=0.0): type(Type::NOR), value(real, imag) {}

    // Instantiate a matrix by passing its size
    //
    // arma::fill::none means that the entries of the matrix will not be 
    // initialized by value, but remain to be the original values in memory
    ArmaOperand(size_t row, size_t col): type(Type::MAT), mat(row, col, arma::fill::none) {}

    // Instantiate an identity matrix by passing its coefficient
    ArmaOperand(Type type, const std::complex<double>& coeff): type(type), value(coeff) {}

    ArmaOperand operator+(const ArmaOperand& rhs) const;
    ArmaOperand operator-(const ArmaOperand& rhs) const;
    ArmaOperand operator*(const ArmaOperand& rhs) const;
    ArmaOperand operator/(const ArmaOperand& rhs) const;
    ArmaOperand operator^(const ArmaOperand& rhs) const;

    // Return a square matrix, which is the product of the identity matrix
    // and the coefficient
    arma::cx_mat fromImat(size_t row) const;

    // Test if the value is approximately a real number
    bool almostReal() const
    {
        return fabs(value.imag()) <= std::numeric_limits<double>::epsilon();
    }

    // Generate LaTeX formatted string
    std::string genTex() const;

private:
    Type type;

    // Entries by default are stored in double, cx = complex
    arma::cx_mat mat;
    std::complex<double> value{1.0, 0.0};
};


// 
inline R newInt(long value) { return R{new Long{value}}; }
inline R newInt(const std::string& value) { return R{new Long{mpz_wrapper(value)}}; }

inline R newComplex(long real, long imag) { return R{new LongComplex{real, imag}}; }
inline R newComplex(const std::string& real, const std::string& imag)
{
    return R{new LongComplex{mpz_wrapper(real), mpz_wrapper(imag)}};
}

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
