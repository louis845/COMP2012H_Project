#include "math/Field.h"

/**
 * This is just multiplication by the inverse of r, with some downcast conversions.
*/
Field* Field::divImpl(const Ring* r) const{
    const Field* f=dynamic_cast<const Field*>(r);
    return dynamic_cast<Field*>(multImpl(f->invert()));
}