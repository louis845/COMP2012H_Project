#include "math/Ring.h"

using namespace std;
Ring::Ring(RingType t): type(t){}

bool Ring::is_type_compatible(const Ring& other) const{
    return type==other.type;
}

const Ring* ZeroElmt::copy() const{
    return this;
}

const Ring* ZeroElmt::addImpl(const Ring* other) const{
    return other; //test22
}

const Ring* ZeroElmt::minusImpl(const Ring* other) const{
    
}