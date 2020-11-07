#include "debug.h"
#include "math/ring.h"
#include <string>

#ifndef MATH_FIELD_H
#define MATH_FIELD_H

class Field : public Ring{
protected:
    Field(RingType);
    virtual const Field* divImpl (const Ring* r) const override;

    /*
     * It is not necessary to consider euclidean functions and remainder of fields. Returns -1 (int) and R::ZERO respectively.
    */
    int euclideanFunc() const override final;

    const Ring* remainderImpl(const Ring* r) const final override;
};

#endif