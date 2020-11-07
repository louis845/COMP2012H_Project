#ifndef MATH_DOUBLE_DOUBLE_H
#define MATH_DOUBLE_DOUBLE_H

#include "math/Field.h"

/**
 * Implementation of double, actually this just uses the primitive type.
*/
class Double final : public Field {

public:
    const double val;

    Double(double);

    const Ring* addImpl (const Ring* r) const override;
    
    const Ring* minusImpl (const Ring* r) const override;
    
    const Ring* multImpl (const Ring* r) const override;
    
    const Field* divImpl (const Ring* div) const override;

    const Field* invert() const override;
    
    const Double* copy() const override;

    bool equalsImpl(const Ring* other) const override;
};
#endif