#include "math/Field.h"

Field::Field(RingType t) : Ring(t){}

/**
 * This is just multiplication by the inverse of r, with some downcast conversions.
*/
const Field* Field::divImpl(const Ring* r) const{
    const Field* f=dynamic_cast<const Field*>(r);
#if DEBUG_MODE
    if(f==nullptr){
        throw "invalid cast!";
    }
#endif
    return dynamic_cast<const Field*>(multImpl(f->invert()));
}

/**
 * There are four cases, say the elements are a,b
 * a=0, b!=0 : E(a)<E(b)
 * a=0, b=0 : E(a)=E(b)
 * a!=0, b!=0 : E(a)=E(b)
 * a!=0, b=0 : E(a)>E(b)
*/
int Field::euclideanFuncCompare(const Ring* r) const{
    if(R::impl0->equalsImpl(this)){
        return R::impl0->euclideanFuncCompare(r);
    }
    return R::impl0->equalsImpl(r)?1:0;
}

const Ring* Field::remainderImpl(const Ring* r) const{
    return promote(R::impl0);
}

bool Field::is_field() const{
    return true;
}