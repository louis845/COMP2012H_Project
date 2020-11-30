#include "math/fraction/Fraction.h"
#include "math/tools.h"
#include <iostream>

Fraction::Fraction(const R& over,const R& under) : Field(RingType::FRACTION){
    int comp=Ring::compatibility(over.get_type(),under.get_type());

    if(comp==-2){
        throw "Fraction must have same types in over and under!";
    }
    if(under.is_zero()){
        throw "Divide by zero!";
    }
    RF *ov,*ud;

    if(comp==0){
        ov=new RF{over};
        ud=new RF{under};
        type->set_sub_type(&(ov->get_type()));
    }else if(comp==1){
        ov=new RF{over};
        ud=new RF{over.promote(under)};
        type->set_sub_type(&(ov->get_type()));
    }else{
        ov=new RF{under.promote(over)};
        ud=new RF{under};
        type->set_sub_type(&(ov->get_type()));
    }
    //now ov and ud are the values of over,under with the same type.

    this->over=*ov;
    this->under=*ud; //value copy.

    delete ov;
    delete ud;

    RF cd=gcd_fast(this->over,this->under);

    this->over = (this->over)/cd;
    this->under = (this->under)/cd;

    RF morph{}, unit{};

    (this->under).split(morph,unit);

    this->under=morph;
    this->over = this->over * unit;
}

Fraction::Fraction(): Field(RingType::FRACTION){}

Fraction* Fraction::fraction_no_gcd(const RF& over, const RF& under){
    Fraction* f=new Fraction{};
    f->over=over;
    f->under=under;
    f->type->set_sub_type(& (over.get_type()));

    RF morph{}, unit{};

    (f->under).split(morph,unit);

    f->under=morph;
    f->over = f->over * unit;

    return f;
}

Fraction* Fraction::fraction_no_check(const RF& over, const RF& under){
    Fraction* f=new Fraction{};
    f->over=over;
    f->under=under;
    f->type->set_sub_type(& (over.get_type()));

    RF cd=gcd_fast(f->over,f->under);

    f->over = (f->over)/cd;
    f->under = (f->under)/cd;

    RF morph{}, unit{};

    (f->under).split(morph,unit);

    f->under=morph;
    f->over = f->over * unit;

    return f;
}

const Ring* Fraction::addImpl(const Ring* r) const{
    const Fraction* f=dynamic_cast<const Fraction*>(r);
#if DEBUG_MODE
    if(f==nullptr){
        throw "invalid cast!";
    }
#endif

    const RF &this_over=over, &this_under=under, &f_over=(f->over), &f_under=(f->under);

    RF nover=this_over*f_under+f_over*this_under;
    RF nunder=this_under*f_under;
    return fraction_no_check(nover,nunder);
}

const Ring* Fraction::multImpl(const Ring* r) const{
    const Fraction* f=dynamic_cast<const Fraction*>(r);
#if DEBUG_MODE
    if(f==nullptr){
        throw "invalid cast!";
    }
#endif

    const RF &this_over=over, &this_under=under, &f_over=(f->over), &f_under=(f->under);

    RF nover=this_over*f_over;
    RF nunder=this_under*f_under;
    return fraction_no_check(nover,nunder);
}

const Ring* Fraction::minusImpl(const Ring* r) const{
    const Fraction* f=dynamic_cast<const Fraction*>(r);
#if DEBUG_MODE
    if(f==nullptr){
        throw "invalid cast!";
    }
#endif

    const RF &this_over=over, &this_under=under, &f_over=(f->over), &f_under=(f->under);

    RF nover=this_over*f_under-f_over*this_under;
    RF nunder=this_under*f_under;
    return fraction_no_check(nover,nunder);
}

const Field* Fraction::divImpl(const Ring* r) const{
    const Fraction* f=dynamic_cast<const Fraction*>(r);
#if DEBUG_MODE
    if(f==nullptr){
        throw "invalid cast!";
    }
#endif
    if(f->is_zero()){
        throw "Divide by zero!";
    }

    const RF &this_over=over, &this_under=under, &f_over=(f->over), &f_under=(f->under);

    RF nover=this_over*f_under;
    RF nunder=this_under*f_over;
    return fraction_no_check(nover,nunder);
}

const Field* Fraction::invert() const{
    RF nover=under;
    RF nunder=over;
    return fraction_no_gcd(nover,nunder); //no need to gcd
}

const Fraction* Fraction::negate() const{
    RF nover= -over;
    RF nunder= under;
    return fraction_no_gcd(nover,nunder); //no need to gcd
}

const Fraction* Fraction::copy() const{
    return fraction_no_check(over,under);
}

bool Fraction::equalsImpl(const Ring* other) const{
    const Fraction* f=dynamic_cast<const Fraction*>(other);
#if DEBUG_MODE
    if(f==nullptr){
        throw "invalid cast!";
    }
#endif

    const RF &this_over=over, &this_under=under, &f_over=(f->over), &f_under=(f->under);

    return this_over.exactly_equals(f_over) && this_under.exactly_equals(f_under);
}

string Fraction::to_string() const{
    if(under.is_one()){
        return over.to_string();
    }
    if(over.needs_bracket()){
        if(under.needs_bracket()){
            return "("+over.to_string()+")/("+under.to_string()+")";
        }
        return "("+over.to_string()+")/"+under.to_string();
    }
    if(under.needs_bracket()){
        return over.to_string()+"/("+under.to_string()+")";
    }
    return over.to_string()+"/"+under.to_string();
}

string Fraction::to_signed_string() const{
    if(under.is_one()){
        return over.to_signed_string();
    }
    if(over.needs_bracket()){
        if(under.needs_bracket()){
            return "+("+over.to_string()+")/("+under.to_string()+")";
        }
        return "+("+over.to_string()+")/"+under.to_string();
    }
    if(under.needs_bracket()){
        return over.to_signed_string()+"/("+under.to_string()+")";
    }
    return over.to_signed_string()+"/"+under.to_string();
}

string Fraction::to_latex() const{
    if(under.is_one()){
        return over.to_latex();
    }
    return "\\frac{"+over.to_latex()+"}{"+under.to_latex()+"}";
}

string Fraction::to_signed_latex() const{
    if(under.is_one()){
        return over.to_signed_latex();
    }
    return "+\\frac{"+over.to_latex()+"}{"+under.to_latex()+"}";
}

bool Fraction::needs_bracket() const{
    if(under.is_one()){
        return over.needs_bracket();
    }
    return true;
}

bool Fraction::needs_bracket_latex() const{
    if(under.is_one()){
        return over.needs_bracket_latex();
    }
    return false;
}

bool Fraction::is_one() const{
    return over.is_one() && under.is_one();
}

bool Fraction::is_unit() const{
    return !over.is_zero();
}

bool Fraction::is_zero() const{
    return over.is_zero();
}

void Fraction::split_canonical(const Ring*& morph, const Ring*& unit) const{
    morph=copy();
    unit=promote_one();
}

const Ring* Fraction::promote(const Ring* const& r) const{
    const NestedRingType& mytype=get_type();
    if(mytype.deep_equals(r->get_type())){
        return r->copy();
    }

    const RF& el= over;

    if(is_type_subset(mytype.get_sub_type(),r->get_type())){ //the type of r is directly the subset given by the fraction numerator and denominator
        const RF promotion= RF{el.promote_exp(r)};
        const RF promot_div= el.promote_one();
        const Ring* promotion2=fraction_no_gcd(promotion,promot_div); //create a fraction with element types same as this->over. 
        //no need to perform gcd since it is dividing by one.
        
        return promotion2;
    }

    //the type of r is still a fraction, with both this and r having the same numerator and denominator type
    const Fraction* f=dynamic_cast<const Fraction*>(r);
#if DEBUG_MODE
    if(f==nullptr){
        throw "invalid cast!";
    }
#endif

    const RF nover=el.promote(f->over);
    const RF nunder=el.promote(f->under);

    //not sure if f->over and f->under has gcd 1 (in the subring), would imply gcd 1 in the larger ring.
    return fraction_no_check(nover,nunder);
}

const Ring* Fraction::promote_one() const{
    const R subring_one = over.promote_one();
    const Ring* promotion2=fraction_no_gcd(subring_one, subring_one);
    
    return promotion2;
}

const Ring* Fraction::complexify() const{
    if(is_complex()){
        return copy();
    }
    RF ov=over.complexify();
    RF ud=under.complexify();
    return fraction_no_check(ov,ud);
}

const Ring* Fraction::conjugate() const{
    RF ov=over.conjugate();
    RF ud=under.conjugate();
    return new Fraction{ov,ud};
}