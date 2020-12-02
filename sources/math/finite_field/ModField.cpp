#include "math/finite_field/ModField.h"

ModField::ModField(const int& val, const int& mod) : Field(RingType::MOD_FIELD), val(val), mod(mod){}

const Ring* ModField::addImpl(const Ring* r) const{
    const ModField* d=dynamic_cast<const ModField*>(r);
#if DEBUG_MODE
    if(d==nullptr){
        throw "invalid cast!";
    }
#endif
    return new ModField{ (val+(d->val)) % mod, mod};
}

const Ring* ModField::multImpl(const Ring* r) const{
    const ModField* d=dynamic_cast<const ModField*>(r);
#if DEBUG_MODE
    if(d==nullptr){
        throw "invalid cast!";
    }
#endif
    return new ModField{ (val*(d->val)) %mod, mod};
}

const Ring* ModField::minusImpl(const Ring* r) const{
    const ModField* d=dynamic_cast<const ModField*>(r);
#if DEBUG_MODE
    if(d==nullptr){
        throw "invalid cast!";
    }
#endif
    return new ModField{ (val-(d->val)) % mod, mod};
}

const Field* ModField::divImpl(const Ring* r) const{
    const ModField* d=dynamic_cast<const ModField*>(r);
#if DEBUG_MODE
    if(d==nullptr){
        throw "invalid cast!";
    }
#endif
    return new ModField{val/(d->val)};
}

const Field* ModField::invert() const{
    return new ModField{1/val};
}

const Ring* ModField::negate() const{
    return new ModField{-val, mod};
}

const Ring* ModField::copy() const{
    return new ModField{val, mod};
}

bool ModField::equalsImpl(const Ring* other) const{
    // short circuit for the second condition, or else the cast would be invalid.
    return (other->type_shallow==RingType::SPECIAL_ZERO && val==0) ||
     (other->type_shallow!=RingType::SPECIAL_ZERO && val==(dynamic_cast<const Double*>(other))->val);
}

string ModField::to_string() const{
    return std::to_string(val);
}

string ModField::to_signed_string() const{
    return "+"+std::to_string(val);
}

string ModField::to_latex() const{
    return to_string();
}

string ModField::to_signed_latex() const{
    return to_signed_string();
}

/**
 * Type is primitive anyway, so only subset is itself
*/
const Ring* ModField::promote(const Ring* const& r) const{
    if(r->type_shallow==RingType::SPECIAL_ZERO){
        return new ModField{0,mod};
    }
    const ModField* d=dynamic_cast<const ModField*>(r);
#if DEBUG_MODE
    if(d==nullptr){
        throw "invalid cast!";
    }
#endif
    return d->copy();
}

const Ring* ModField::promote_one() const{
    return new ModField{1,mod};
}

/**
 * Complexify does nothing on non-complex rings.
*/
const Ring* ModField::complexify() const{
    return copy();
}

bool ModField::is_one() const{
    return val==1;
}

bool ModField::is_unit() const{
    return val!=0;
}

void ModField::split_canonical(const Ring*& morph, const Ring*& unit) const{
    morph=copy();
    unit=promote_one();
}