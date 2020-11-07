#include "math/Ring.h"

using namespace std;
Ring::Ring(RingType t): type(t){}

Ring::~Ring(){
    
}

bool Ring::is_type_compatible(const Ring& other) const{
    return type==other.type || type==RingType::SPECIAL_ZERO || type==RingType::SPECIAL_ONE || other.type==RingType::SPECIAL_ZERO || other.type==RingType::SPECIAL_ONE;
}

/**
 * Implementation of ZeroElmt. The return values are straightforward (operations by 0 and 1). Notice that a copy (new allocation) has to be returned for the functions,
 * since the functions are used internally by the R class, and a new R object (no matter statically or dynamically allocated) is created with
 * the new Ring impl stored dynamically in the new R object. Making a copy avoids the problem of two different R having the dynamically allocated
 * Ring implementation, thus creating errors when one R is deconstructed (deallocates the Ring impl).
*/

ZeroElmt::ZeroElmt(): Ring(RingType::SPECIAL_ZERO){

}

const Ring* ZeroElmt::copy() const{
    return new ZeroElmt{};
}

const Ring* ZeroElmt::addImpl(const Ring* r) const{
    return r->copy();
}

const Ring* ZeroElmt::minusImpl(const Ring* r) const{
    return r->negate();
}

const Ring* ZeroElmt::multImpl(const Ring* r) const{
    return new ZeroElmt{};
}

const Ring* ZeroElmt::divImpl(const Ring* r) const{
    return new ZeroElmt{};
}

const Ring* ZeroElmt::remainderImpl(const Ring* r) const{
    return new ZeroElmt{};
}

const Ring* ZeroElmt::negate() const{
    return new ZeroElmt{};
}

bool ZeroElmt::equalsImpl(const Ring* compare) const{
    //Added comparison not equal to ONE, to avoid infinite loop.
    return compare->type!=RingType::SPECIAL_ONE && (compare->type==RingType::SPECIAL_ZERO || compare->equalsImpl(this));
}

const Ring* ZeroElmt::invert() const{
    throw "Cannot invert zero element!";
}

int ZeroElmt::euclideanFunc() const{
    return -1;
}

std::string ZeroElmt::to_string() const{
    return "0";
}
    
std::string ZeroElmt::to_signed_string() const{
    return "+0";
}

std::string ZeroElmt::to_latex() const{
    return "0";
}

std::string ZeroElmt::to_signed_latex() const{
    return "+0";
}
