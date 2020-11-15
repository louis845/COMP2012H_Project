#ifndef MATH_DOUBLE_DOUBLECPX_H
#define MATH_DOUBLE_DOUBLECPX_H

#include "math/Field.h"

using namespace std;

/**
 * Implementation of double, actually this just uses the primitive type.
*/
class DoubleComplex final : public Field {
public:
    DoubleComplex(const double&, const double&);

    const double re, im;
protected:
    /**
     * Precision in decimal places, of the string/latex representation.
    */
    static int PRECISION;

    const Ring* addImpl (const Ring* r) const override;
    
    const Ring* minusImpl (const Ring* r) const override;
    
    const Ring* multImpl (const Ring* r) const override;
    
    const Field* divImpl (const Ring* div) const override;

    const Field* invert() const override;
    
    const DoubleComplex* copy() const override;

    const DoubleComplex* negate() const override;

    bool equalsImpl(const Ring* other) const override;

    string to_string() const override;

    string to_signed_string() const override;

    string to_latex() const override;

    string to_signed_latex() const override;

    const DoubleComplex* promote(const Ring* const& r) const override;

    const DoubleComplex* promote_one() const override;

    const Ring* complexify() const override;

    bool is_one() const override;

    bool is_unit() const override;

    void split_canonical(const Ring*&, const Ring*&) const override;
};
#endif