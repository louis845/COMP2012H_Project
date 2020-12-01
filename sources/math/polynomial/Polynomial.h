#ifndef MATH_POLYNOMIAL_POLYNOMIAL_H
#define MATH_POLYNOMIAL_POLYNOMIAL_H

#include "math/Ring.h"

/**
 * Implementation of polynomials.
*/
class Polynomial final : public Ring {
private:
    int get_degree_no0check() const;
    static Polynomial* polynomial_no_check(const RF* const&, int length); //Creates a polynomial without checking for types. Used internally
    static Polynomial* polynomial_no_zero_check(const RF* const&, int length, int non_zero_count); //Creates a polynomial without checking for types, or checking for zero elements. Used internally.

    Polynomial(); //Creates a polynomial without initializing length, display_terms, ceoff

    const RF& get_leading_coefficient() const;
public:
    /**
     * Creates a polynomial with given coefficients and degree. This constructor copies the contents of the array, hence the coefficient array NEEDS to be
     * deleted if it is dynamically allocated. coefficient[0] corresponds to the constant, coefficient[1] to the power 1 term, etc...
     * 
     * Make sure the coefficients are compatible with each other (see Ring::is_type_compatible), and the length is non-zero.
    */
    Polynomial(const R* const &, int length);

    ~Polynomial();

    int get_degree() const;

    static char PY_CHAR;
protected:

    int length;
    //Used for output such as to_string
    int display_terms;
    RF* coeff;

    const Polynomial* multiply_const(const RF& mult, int degree) const;

    const Ring* addImpl (const Ring* r) const override;
    
    const Ring* minusImpl (const Ring* r) const override;
    
    const Ring* multImpl (const Ring* r) const override;
    
    const Ring* divImpl (const Ring* div) const override;

    const Ring* remainderImpl (const Ring* rem) const override;

    void quotAndRemainder (const Ring* div, const Ring*& quot, const Ring*& rem) const override;

    const Ring* invert() const override;
    
    const Polynomial* copy() const override;

    const Polynomial* negate() const override;

    int euclideanFuncCompare(const Ring* other) const override;

    bool equalsImpl(const Ring* other) const override;

    std::string to_string() const override;

    std::string to_signed_string() const override;

    std::string to_latex() const override;

    std::string to_signed_latex() const override;

    virtual bool needs_bracket() const override;

    virtual bool needs_bracket_latex() const override;

    const Ring* promote(const Ring* const &r) const override;

    const Ring* promote_one() const override;

    const Ring* complexify() const override;

    const Ring* conjugate() const override;

    bool is_one() const override;

    bool is_unit() const override;

    bool is_zero() const override;

    void split_canonical(const Ring*&, const Ring*&) const override;
};
#endif