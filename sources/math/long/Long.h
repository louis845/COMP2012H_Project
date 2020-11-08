#ifndef MATH_LONG_LONG_H
#define MATH_LONG_LONG_H

#include "math/Ring.h"

using namespace std;

/**
 * Implementation of long, actually this just uses the primitive type long long.
*/
class Long final : public Ring {
public:
    Long(long long);
protected:

    const long long val;

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
};
#endif