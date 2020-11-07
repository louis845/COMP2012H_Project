#include "math/Field.h"

Field::Field(RingType t) : Ring(t){}

/**
 * This is just multiplication by the inverse of r, with some downcast conversions.
*/
const Field* Field::divImpl(const Ring* r) const{
    const Field* f=dynamic_cast<const Field*>(r);
    return dynamic_cast<const Field*>(multImpl(f->invert()));
}

int Field::euclideanFunc() const{
    return -1;
}

const Ring* Field::remainderImpl(const Ring* r) const{
    return new ZeroElmt{};
}