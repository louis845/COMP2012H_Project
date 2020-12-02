#ifndef MATH_FIELD_MODULO_H
#define MATH_FIELD_MODULO_H

#include "math/Field.h"
#include "math/R.h"

using namespace std;

/**
 * Implementation of double, actually this just uses the primitive type.
*/
class ModField final : public Field {
public:
    ModField(const int& val, const int& mod);

    const int val;
    const int mod;
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
    
    const Ring* copy() const override;

    const Ring* negate() const override;

    bool equalsImpl(const Ring* other) const override;

    string to_string() const override;

    string to_signed_string() const override;

    string to_latex() const override;

    string to_signed_latex() const override;

    const Ring* promote(const Ring* const& r) const override;

    const Ring* promote_one() const override;

    const Ring* complexify() const override;

    bool is_one() const override;

    bool is_unit() const override;

    void split_canonical(const Ring*&, const Ring*&) const override;
};
#endif