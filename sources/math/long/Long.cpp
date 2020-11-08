#include "math/Long/Long.h"
#include <string>
#include <cmath>

Long::Long(long long val) : Ring(RingType::LONG), val(val){}

const Ring* Long::addImpl(const Ring* r) const{
    return new Long{val+(dynamic_cast<const Long*>(r)->val)};
}

const Ring* Long::multImpl(const Ring* r) const{
    return new Long{val*(dynamic_cast<const Long*>(r)->val)};
}

const Ring* Long::minusImpl(const Ring* r) const{
    return new Long{val-(dynamic_cast<const Long*>(r)->val)};
}

const Ring* Long::divImpl(const Ring* r) const{
    return new Long{val/(dynamic_cast<const Long*>(r)->val)};
}

const Ring* Long::remainderImpl(const Ring* r) const {
    return new Long{val%(dynamic_cast<const Long*>(r)->val)};
}

const Ring* Long::invert() const{
    return new Long{1/val};
}

const Long* Long::negate() const{
    return new Long{-val};
}

const Long* Long::copy() const{
    return new Long{val};
}

int Long::euclideanFuncCompare(const Ring* other) const{
    long long otherval=std::abs( static_cast<const Long*>(other)->val );

    long long thisval=std::abs(val);
    return thisval==otherval? 0 : ( thisval<otherval? -1 : 1 );
}

bool Long::equalsImpl(const Ring* other) const{
    // short circuit for the second condition, or else the cast would be invalid.
    return (other->type_shallow==RingType::SPECIAL_ZERO && val==0) ||
     (other->type_shallow!=RingType::SPECIAL_ZERO && val==(static_cast<const Long*>(other))->val);
}

string Long::to_string() const{
    return std::to_string(val);
}

string Long::to_signed_string() const{
    if(val>=0){
        return "+"+std::to_string(val);
    }
    return std::to_string(val);
}

string Long::to_latex() const{
    return std::to_string(val);
}

string Long::to_signed_latex() const{
    if(val>=0){
        return "+"+std::to_string(val);
    }
    return std::to_string(val);
}

/**
 * Type is primitive, the only subset is itself anyway
*/
const Long* Long::promote(const Ring* const& r) const{
    if(r->type_shallow==RingType::SPECIAL_ZERO){
        return new Long{0};
    }
    return static_cast<const Long*>(r)->copy();
}

bool Long::is_one() const{
    return val==1;
}

bool Long::is_unit() const{
    return val==1 || val==-1;
}

void Long::split_canonical(const Ring*& morph, const Ring*& unit) const{
    if(val==0){
        morph=new Long{0};
        unit=new Long{1};
    }else if(val>0){
        morph=new Long{val};
        unit=new Long{1};
    }else{
        morph=new Long{-val};
        unit=new Long{-1};
    }
}