#ifndef MATH_LONG_LONG_H
#define MATH_LONG_LONG_H

#include "math/Ring.h"
#include "math/long/LongType.h"

#include <gmpxx.h> // GNU GMP, we only use the big integer class mpz_class in place of long.

using namespace std;

/**
 * Implementation of long, actually this just uses the primitive type long long.
*/

class Long final : public Ring {
public:
    Long(PRIMITIVE_LONG_TYPE);

    const PRIMITIVE_LONG_TYPE val;
protected:

    const Ring* addImpl (const Ring* r) const override;
    
    const Ring* minusImpl (const Ring* r) const override;
    
    const Ring* multImpl (const Ring* r) const override;
    
    const Ring* divImpl (const Ring* div) const override;

    const Ring* remainderImpl (const Ring* rem) const override;

    const Ring* invert() const override;
    
    const Long* copy() const override;

    const Long* negate() const override;

    int euclideanFuncCompare(const Ring* other) const override;

    bool equalsImpl(const Ring* other) const override;

    string to_string() const override;

    string to_signed_string() const override;

    string to_latex() const override;

    string to_signed_latex() const override;

    const Long* promote(const Ring* const &r) const override;

    const Long* promote_one() const override;

    bool is_one() const override;

    bool is_unit() const override;

    void split_canonical(const Ring*&, const Ring*&) const override;
};
#endif