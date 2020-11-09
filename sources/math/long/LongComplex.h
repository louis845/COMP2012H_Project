#ifndef MATH_LONG_LONGCPX_H
#define MATH_LONG_LONGCPX_H

#include "math/Ring.h"

using namespace std;

/**
 * Implementation of complex long, actually this just uses two primitive types long long.
 * See Gaussian Integers on wiki.
*/
class LongComplex final : public Ring {
public:
    LongComplex(const long long&, const long long&);
    
protected:
    const long long re, im;

    const Ring* addImpl (const Ring* r) const override;
    
    const Ring* minusImpl (const Ring* r) const override;
    
    const Ring* multImpl (const Ring* r) const override;
    
    const Ring* divImpl (const Ring* div) const override;

    const Ring* remainderImpl (const Ring* div) const override;

    void quotAndRemainder (const Ring* div, const Ring*& quot, const Ring*& rem) const override;

    const Ring* invert() const override;
    
    const LongComplex* copy() const override;

    const LongComplex* negate() const override;

    int euclideanFuncCompare(const Ring* other) const override;

    bool equalsImpl(const Ring* other) const override;

    string to_string() const override;

    string to_signed_string() const override;

    string to_latex() const override;

    string to_signed_latex() const override;

    bool needs_bracket() const override;

    bool needs_bracket_latex() const override;

    const LongComplex* promote(const Ring* const &r) const override;

    const LongComplex* promote_one() const override;

    bool is_one() const override;

    bool is_unit() const override;

    void split_canonical(const Ring*&, const Ring*&) const override;
};
#endif