#include "debug.h"
#include "math/ring.h"
#include <string>

#ifndef MATH_FIELD_H
#define MATH_FIELD_H

class Field : public Ring{
protected:
    virtual Field* divImpl (const Ring* r) const override;
    virtual Field* invert () const = 0;
};

#endif