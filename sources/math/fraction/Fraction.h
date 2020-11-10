#ifndef MATH_FRACTION_FRACTION_H
#define MATH_FRACTION_FRACTION_H
#include "math/Field.h"

using namespace std;

class Fraction : public Field {
public:
    /**
     * Fraction(over, under). This represents a fraction over/under. 
    */
    Fraction(const R&,const R&);

    ~Fraction() override;
protected:
    /**
     * Creates a fraction with uninitialized over and under. Used internally to skip the GCD operation. Make sure to set the appropriate type.
    */
    Fraction();

    R* over;

    R* under;

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

    bool is_unit() const override;

    bool is_one() const override;

    bool is_zero() const override;

    void split_canonical(const Ring*&, const Ring*&) const override;
};

#endif