#ifndef MATH_FRACTION_FRACTION_H
#define MATH_FRACTION_FRACTION_H
#include "math/Field.h"

using namespace std;

class Fraction : public Field {
private:
    /**
     * Creates a Fraction without GCD reduction and without type checking / promotion. Allocates and returns.
    */
    static Fraction* fraction_no_gcd(const RF&, const RF&); 
    /*
     * Creates a Fraction without type checking / promotion. Allocates and returns.
    */
    static Fraction* fraction_no_check(const RF&, const RF&); 

    /**
     * Suppose over,under are polynomials over fractions. By default, under would be stored as a monic polynomial.
     * It should be better to display the polynomials in the numerator and denominator as non-fractional polynomials,
     * hence this function changes the internal representation whenever this scenario occurs.
    */
    void accomodate_polynomial_pretty_print();

    /**
     * Creates a fraction with uninitialized over and under. Used internally to skip the GCD operation. Use fraction_no_check and fraction_no_gcd
    */
    Fraction();
public:
    /**
     * Fraction(over, under). This represents a fraction over/under. 
    */
    Fraction(const R&,const R&);
protected:

    RF over;

    RF under;

    const Ring* addImpl (const Ring* r) const override;
    
    const Ring* minusImpl (const Ring* r) const override;
    
    const Ring* multImpl (const Ring* r) const override;
    
    const Field* divImpl (const Ring* div) const override;

    const Field* invert() const override;
    
    const Fraction* copy() const override;

    const Fraction* negate() const override;

    bool equalsImpl(const Ring* other) const override;

    virtual string to_string() const override;

    virtual string to_signed_string() const override;

    virtual string to_latex() const override;

    virtual string to_signed_latex() const override;

    virtual bool needs_bracket() const override;

    virtual bool needs_bracket_latex() const override;

    const Ring* promote(const Ring* const&) const override;

    const Ring* promote_one() const override;

    const Ring* complexify() const override;

    const Ring* to_finite_field(int modulo) const override;

    /**
     * Conjugates both over and under. Mathematically correct as conjugation is an automorphism.
    */
    const Ring* conjugate() const override;

    bool is_unit() const override;

    bool is_one() const override;

    bool is_zero() const override;

    void split_canonical(const Ring*&, const Ring*&) const override;
};

#endif