#ifndef MATH_FIELD_H
#define MATH_FIELD_H

#include "debug.h"
#include "math/Ring.h"
#include <string>

class Field : public Ring{
protected:
    Field(RingType);
    virtual const Field* divImpl (const Ring* r) const override;

    /*
     * It is not necessary to consider euclidean functions and remainder of fields. Returns -1 (int) and RF::ZERO respectively.
    */
    int euclideanFuncCompare(const Ring* r) const override final;

    const Ring* remainderImpl(const Ring* r) const final override;

    bool is_field() const override;
};

#endif
