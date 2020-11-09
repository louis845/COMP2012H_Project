#include "math/fraction/Fraction.h"
#include "math/tools.h"

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

Fraction::~Fraction(){
    delete over;
    delete under;
}

const Ring* Fraction::addImpl(const Ring* r) const{
    const Fraction* f=dynamic_cast<const Fraction*>(r);

    R &this_over=*over, &this_under=*under, &f_over=*(f->over), &f_under=*(f->under);

    R nover=this_over*f_under+f_over*this_under;
    R nunder=this_under*f_under;
    return new Fraction{nover,nunder};
}

const Ring* Fraction::multImpl(const Ring* r) const{
    const Fraction* f=dynamic_cast<const Fraction*>(r);

    R &this_over=*over, &this_under=*under, &f_over=*(f->over), &f_under=*(f->under);

    R nover=this_over*f_over;
    R nunder=this_under*f_under;
    return new Fraction{nover,nunder};
}

const Ring* Fraction::minusImpl(const Ring* r) const{
    const Fraction* f=dynamic_cast<const Fraction*>(r);

    R &this_over=*over, &this_under=*under, &f_over=*(f->over), &f_under=*(f->under);

    R nover=this_over*f_under-f_over*this_under;
    R nunder=this_under*f_under;
    return new Fraction{nover,nunder};
}

const Field* Fraction::divImpl(const Ring* r) const{
    const Fraction* f=dynamic_cast<const Fraction*>(r);

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
    return new Fraction{*over,*under};
}

bool Fraction::equalsImpl(const Ring* other) const{
    const Fraction* f=dynamic_cast<const Fraction*>(other);

    R &this_over=*over, &this_under=*under, &f_over=*(f->over), &f_under=*(f->under);

    return this_over.exactly_equals(f_over) && this_under.exactly_equals(f_under);
}

string Fraction::to_string() const{
    if(under->is_one()){
        return over->to_string();
    }
    return "("+over->to_string()+")/("+under->to_string()+")";
}

string Fraction::to_signed_string() const{
    if(under->is_one()){
        return over->to_string();
    }
    return "+("+over->to_string()+")/("+under->to_string()+")";
}

string Fraction::to_latex() const{
    if(under->is_unit()){
        return over->to_string();
    }
    return "\\frac{"+over->to_string()+"}{"+under->to_string()+"}";
}

string Fraction::to_signed_latex() const{
    if(under->is_unit()){
        return over->to_string();
    }
    return "+\\frac{"+over->to_string()+"}{"+under->to_string()+"}";
}

bool Fraction::is_one() const{
    return over->is_one() && under->is_one();
}

bool Fraction::is_unit() const{
    return !over->is_zero();
}

void Fraction::split_canonical(const Ring*& morph, const Ring*& unit) const{
    throw "not implemented yet";
    /*morph=new Fraction{val};
    unit=new Double{1.0};*/
}

const Fraction* Fraction::promote(const Ring* const& r) const{
    throw "not implemented yet";
}