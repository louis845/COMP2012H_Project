#include "math/Long/Long.h"
#include <string>
#include <cmath>

Long::Long(PRIMITIVE_LONG_TYPE val) : Ring(RingType::LONG), val(val){}

const Ring* Long::addImpl(const Ring* r) const{
    const Long* l=dynamic_cast<const Long*>(r);
#if DEBUG_MODE
    if(l==nullptr){
        throw "invalid cast!";
    }
#endif
    return new Long{val+(l->val)};
}

const Ring* Long::multImpl(const Ring* r) const{
    const Long* l=dynamic_cast<const Long*>(r);
#if DEBUG_MODE
    if(l==nullptr){
        throw "invalid cast!";
    }
#endif
    return new Long{val*(l->val)};
}

const Ring* Long::minusImpl(const Ring* r) const{
    const Long* l=dynamic_cast<const Long*>(r);
#if DEBUG_MODE
    if(l==nullptr){
        throw "invalid cast!";
    }
#endif
    return new Long{val-(l->val)};
}

const Ring* Long::divImpl(const Ring* r) const{
    const Long* l=dynamic_cast<const Long*>(r);
#if DEBUG_MODE
    if(l==nullptr){
        throw "invalid cast!";
    }
#endif
    return new Long{val/(l->val)};
}

const Ring* Long::remainderImpl(const Ring* r) const {
    const Long* l=dynamic_cast<const Long*>(r);
#if DEBUG_MODE
    if(l==nullptr){
        throw "invalid cast!";
    }
#endif
    return new Long{val%(l->val)};
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
    const Long* l=dynamic_cast<const Long*>(other);
#if DEBUG_MODE
    if(l==nullptr){
        throw "invalid cast!";
    }
#endif
    PRIMITIVE_LONG_TYPE otherval=abs( l->val );

    PRIMITIVE_LONG_TYPE thisval=abs(val);
    return thisval==otherval? 0 : ( thisval<otherval? -1 : 1 );
}

bool Long::equalsImpl(const Ring* other) const{
    const Long* l=dynamic_cast<const Long*>(other);
#if DEBUG_MODE
    if(l==nullptr){
        throw "invalid cast!";
    }
#endif
    return val==l->val;
}

string Long::to_string() const{
    return CONVERT_TO_STRING(val);
}

string Long::to_signed_string() const{
    if(val>=0){
        return "+"+CONVERT_TO_STRING(val);
    }
    return CONVERT_TO_STRING(val);
}

string Long::to_latex() const{
    return CONVERT_TO_STRING(val);
}

string Long::to_signed_latex() const{
    if(val>=0){
        return "+"+CONVERT_TO_STRING(val);
    }
    return CONVERT_TO_STRING(val);
}

/**
 * Type is primitive, the only subset is itself anyway
*/
const Long* Long::promote(const Ring* const& r) const{
    if(r->type_shallow==RingType::SPECIAL_ZERO){
        return new Long{0};
    }
    const Long* l=dynamic_cast<const Long*>(r);
#if DEBUG_MODE
    if(l==nullptr){
        throw "invalid cast!";
    }
#endif
    return l->copy();
}

const Long* Long::promote_one() const{
    return new Long{1};
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