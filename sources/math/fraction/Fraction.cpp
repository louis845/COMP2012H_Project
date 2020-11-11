#include "math/fraction/Fraction.h"
#include "math/tools.h"
#include <iostream>

Fraction::Fraction(const R& over,const R& under) : Field(RingType::FRACTION){
    int comp=Ring::compatibility(over.get_type(),under.get_type());

    if(comp==-2){
        throw "Fraction must have same types in over and under!";
    }
    R *ov,*ud;

    if(comp==0){
        ov=new R{over};
        ud=new R{under};
        type->set_sub_type(&(ov->get_type()));
    }else if(comp==1){
        ov=new R{over};
        ud=new R{over.promote(under)};
        type->set_sub_type(&(ov->get_type()));
    }else{
        ov=new R{under.promote(over)};
        ud=new R{under};
        type->set_sub_type(&(ov->get_type()));
    }
    
    if(ud->is_zero()){
        throw "Divide by zero!";
    }

    R cd=gcd(*ov,*ud);

    this->over=ov;
    this->under=ud;

    R &mover=*ov, &munder=*ud;

    mover=mover/cd;
    munder=munder/cd;

    R morph{nullptr}, unit{nullptr};

    munder.split(morph,unit);

    munder=morph;
    mover = mover * unit;
}

Fraction::Fraction(): Field(RingType::FRACTION){}

Fraction::~Fraction(){
    delete over;
    delete under;
}

const Ring* Fraction::addImpl(const Ring* r) const{
    const Fraction* f=dynamic_cast<const Fraction*>(r);
#if DEBUG_MODE
    if(f==nullptr){
        throw "invalid cast!";
    }
#endif

    R &this_over=*over, &this_under=*under, &f_over=*(f->over), &f_under=*(f->under);

    R nover=this_over*f_under+f_over*this_under;
    R nunder=this_under*f_under;
    return new Fraction{nover,nunder};
}

const Ring* Fraction::multImpl(const Ring* r) const{
    const Fraction* f=dynamic_cast<const Fraction*>(r);
#if DEBUG_MODE
    if(f==nullptr){
        throw "invalid cast!";
    }
#endif

    R &this_over=*over, &this_under=*under, &f_over=*(f->over), &f_under=*(f->under);

    R nover=this_over*f_over;
    R nunder=this_under*f_under;
    return new Fraction{nover,nunder};
}

const Ring* Fraction::minusImpl(const Ring* r) const{
    const Fraction* f=dynamic_cast<const Fraction*>(r);
#if DEBUG_MODE
    if(f==nullptr){
        throw "invalid cast!";
    }
#endif

    R &this_over=*over, &this_under=*under, &f_over=*(f->over), &f_under=*(f->under);

    R nover=this_over*f_under-f_over*this_under;
    R nunder=this_under*f_under;
    return new Fraction{nover,nunder};
}

const Field* Fraction::divImpl(const Ring* r) const{
    const Fraction* f=dynamic_cast<const Fraction*>(r);
#if DEBUG_MODE
    if(f==nullptr){
        throw "invalid cast!";
    }
#endif

    R &this_over=*over, &this_under=*under, &f_over=*(f->over), &f_under=*(f->under);

    R nover=this_over*f_under;
    R nunder=this_under*f_over;
    return new Fraction{nover,nunder};
}

const Field* Fraction::invert() const{
    R &this_over=*over, &this_under=*under;

    R nover=this_under;
    R nunder=this_over;
    return new Fraction{nover,nunder};
}

const Fraction* Fraction::negate() const{
    R &this_over=*over, &this_under=*under;

    R nover= -this_over;
    R nunder=this_under;
    return new Fraction{nover,nunder};
}

const Fraction* Fraction::copy() const{
    Fraction *f=new Fraction{};
    f->over=new R{*over};
    f->under=new R{*under};
    f->type->set_sub_type(&(type->get_sub_type()));
    return f;
}

bool Fraction::equalsImpl(const Ring* other) const{
    const Fraction* f=dynamic_cast<const Fraction*>(other);
#if DEBUG_MODE
    if(f==nullptr){
        throw "invalid cast!";
    }
#endif

    R &this_over=*over, &this_under=*under, &f_over=*(f->over), &f_under=*(f->under);

    return this_over.exactly_equals(f_over) && this_under.exactly_equals(f_under);
}

string Fraction::to_string() const{
    if(under->is_one()){
        return over->to_string();
    }
    if(over->needs_bracket()){
        if(under->needs_bracket()){
            return "("+over->to_string()+")/("+under->to_string()+")";
        }
        return "("+over->to_string()+")/"+under->to_string();
    }
    if(under->needs_bracket()){
        return over->to_string()+"/("+under->to_string()+")";
    }
    return over->to_string()+"/"+under->to_string();
}

string Fraction::to_signed_string() const{
    if(under->is_one()){
        return over->to_signed_string();
    }
    if(over->needs_bracket()){
        if(under->needs_bracket()){
            return "+("+over->to_string()+")/("+under->to_string()+")";
        }
        return "+("+over->to_string()+")/"+under->to_string();
    }
    if(under->needs_bracket()){
        return over->to_signed_string()+"/("+under->to_string()+")";
    }
    return over->to_signed_string()+"/"+under->to_string();
}

string Fraction::to_latex() const{
    if(under->is_one()){
        return over->to_latex();
    }
    return "\\frac{"+over->to_latex()+"}{"+under->to_latex()+"}";
}

string Fraction::to_signed_latex() const{
    if(under->is_one()){
        return over->to_signed_latex();
    }
    return "\\frac{"+over->to_latex()+"}{"+under->to_latex()+"}";
}

bool Fraction::needs_bracket() const{
    if(under->is_one()){
        return over->needs_bracket();
    }
    return true;
}

bool Fraction::needs_bracket_latex() const{
    if(under->is_one()){
        return over->needs_bracket_latex();
    }
    return false;
}

bool Fraction::is_one() const{
    return over->is_one() && under->is_one();
}

bool Fraction::is_unit() const{
    return !over->is_zero();
}

bool Fraction::is_zero() const{
    return over->is_zero();
}

void Fraction::split_canonical(const Ring*& morph, const Ring*& unit) const{
    morph=new Fraction{*over,*under};
    unit=promote_one();
}

const Ring* Fraction::promote(const Ring* const& r) const{
    const NestedRingType& mytype=get_type();
    if(mytype.deep_equals(r->get_type())){
        return r->copy();
    }

    const R& el= *over;

    if(is_type_subset(mytype.get_sub_type(),r->get_type())){ //the type of r is directly the subset given by the fraction numerator and denominator
        const R promotion= R{el.promote_exp(r)};
        const R promot_div= el.promote_one();
        const Ring* promotion2=new Fraction{promotion,promot_div};
        
        return promotion2;
    }

    //the type of r is still a fraction, with both this and r having the same numerator and denominator type
    const Fraction* f=dynamic_cast<const Fraction*>(r);
#if DEBUG_MODE
    if(f==nullptr){
        throw "invalid cast!";
    }
#endif

    const R nover=el.promote(*f->over);
    const R nunder=el.promote(*f->under);

    return new Fraction{nover,nunder};
}

const Ring* Fraction::promote_one() const{
    const R& el= *over;
    const R subring_one = el.promote_one();
    const Ring* promotion2=new Fraction{subring_one, subring_one};
    
    return promotion2;
}