#ifndef MATH_DOUBLE_DOUBLE_H
#define MATH_DOUBLE_DOUBLE_H

#include "math/Field.h"

using namespace std;

/**
 * Implementation of double, actually this just uses the primitive type.
*/
class Double final : public Field {
public:
    Double(double);
protected:
    /**
     * Precision in decimal places, of the string/latex representation.
    */
    static int PRECISION;

    const double val;

    const Ring* addImpl (const Ring* r) const override;
    
    const Ring* minusImpl (const Ring* r) const override;
    
    const Ring* multImpl (const Ring* r) const override;
    
    const Field* divImpl (const Ring* div) const override;

    const Field* invert() const override;
    
    const Double* copy() const override;

    const Double* negate() const override;

    bool equalsImpl(const Ring* other) const override;

    string to_string() const override;

    string to_signed_string() const override;

    string to_latex() const override;

    string to_signed_latex() const override;

    const Double* promote(const Ring* const& r) const override;

    bool is_one() const override;

    bool is_unit() const override;

    void split_canonical(const Ring*&, const Ring*&) const override;
};
#endif