#ifndef MATH_DOUBLE_DOUBLE_H
#define MATH_DOUBLE_DOUBLE_H

#include "math/Field.h"

/**
 * Implementation of double, actually this just uses the primitive type.
*/
class Double : public Field final{

public:
    double val;

    Double(double);

    virtual Ring* addImpl (const Ring* r) const;
    
    virtual Ring* minusImpl (const Ring* r) const;
    
    virtual Ring* multImpl (const Ring* r) const;
    
    virtual Ring* divImpl (const Ring* div) const;
    
    virtual Ring* remainderImpl (const Ring* div) const;
};
#endif