#include "math/finite_field/ModField.h"
#include "math/tools.h"

//Take modulo, but make sure the modulo is always positive.
ModField::ModField(const int& val, const int& mod) : Field(RingType::MOD_FIELD), val( val%mod + ((val%mod)<0 ? mod : 0) ), mod(mod){
    type->set_extra_info(mod); //Type of mod fields is equal iff the modulo is the same.
}

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
    if(d->val==0){
        throw "Divide by zero!";
    }
    int gcd,dis; //useless
    int inverse;

    xgcd(d->val,mod,gcd,inverse,dis);

    return new ModField{inverse*val, mod};
}

const Field* ModField::invert() const{
    int gcd,dis; //useless
    int inverse;

    xgcd(val,mod,gcd,inverse,dis);

    return new ModField{inverse, mod};
}

const Ring* ModField::negate() const{
    return new ModField{-val, mod};
}

const Ring* ModField::copy() const{
    return new ModField{val, mod};
}

bool ModField::equalsImpl(const Ring* other) const{
    return val==(dynamic_cast<const ModField*>(other))->val;
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