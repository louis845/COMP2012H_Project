#include "math/polynomial/Polynomial.h"
#include <string>
#include <cmath>
#include <algorithm>
#include <sstream>

using namespace std;

char Polynomial::PY_CHAR = 't';

Polynomial* Polynomial::polynomial_no_check(const RF* const& coefficient, int length){
    Polynomial* p=new Polynomial{};
    
    int degree=-1;
    int non_zero_count=0;
    for(int i=0;i<length;++i){
        const R& cElem=coefficient[i];
        if(!cElem.is_zero()){
            degree=i;
            ++non_zero_count;
        }
    }
    if(degree==-1){
        //All coefficients given are zero.
        p->coeff=new RF[1]{RF{coefficient[0].promote(R::ZERO)}}; //Promote the ZERO element to match the given type.
        p->length=1;
        p->type->set_sub_type(& (coefficient[0].get_type()) );
        p->display_terms=1;
    }else{
        p->coeff=new RF[degree+1];
        p->length=degree+1;
        for(int i=0;i<=degree;i++){
            p->coeff[i]=coefficient[i];
        }
        p->type->set_sub_type(&(p->coeff->get_type()));
        p->display_terms=non_zero_count;
    }
    return p;
}

Polynomial* Polynomial::polynomial_no_zero_check(const RF* const& coefficient, int length, int non_zero_count){
    Polynomial* p=new Polynomial{};
    
    int degree=length-1;
    p->coeff=new RF[degree+1];
    p->length=degree+1;
    for(int i=0;i<=degree;i++){
        p->coeff[i]=coefficient[i];
    }
    p->type->set_sub_type(&(p->coeff->get_type()));
    p->display_terms=non_zero_count;
    return p;
}

Polynomial::Polynomial() : Ring(RingType::POLYNOMIAL){
    
}

Polynomial::Polynomial(const R* const &coefficient, int length) : Ring(RingType::POLYNOMIAL){
#ifdef DEBUG_MODE
    if(length==0){
        throw "Cannot initialize a polynomial with length 0!";
    }
#endif
    RF largest_type=R::ZERO; //The type of ring that includes all the rings of non-zero elements
    int degree=-1;
    int non_zero_count=0;
    for(int i=0;i<length;++i){
        const R& cElem=coefficient[i];
        if(Ring::is_type_subset( cElem.get_type(), largest_type.get_type() )){
            largest_type=cElem;
        }else if(!Ring::is_type_subset( largest_type.get_type(), cElem.get_type() )){
            throw "Types of coefficient are not compatible!";
        }
        if(!cElem.is_zero()){
            degree=i;
            ++non_zero_count;
        }
    }
    if(degree==-1){
        //All coefficients given are zero.
        coeff=new RF[1]{RF{largest_type.promote(R::ZERO)}}; //Promote the ZERO element to match the given type.
        this->length=1;
        type->set_sub_type(& (largest_type.get_type()) );
        display_terms=1;
    }else{
        coeff=new RF[degree+1];
        this->length=degree+1;
        for(int i=0;i<=degree;i++){
            coeff[i]=largest_type.promote(coefficient[i]);
        }
        type->set_sub_type(&(coeff->get_type()));
        display_terms=non_zero_count;
    }
}

Polynomial::~Polynomial(){
    delete[] coeff;
}

const Ring* Polynomial::addImpl(const Ring* r) const{
    const Polynomial *other=dynamic_cast<const Polynomial*>(r);
#if DEBUG_MODE
    if(other==nullptr){
        throw "invalid cast!";
    }
#endif

    const RF *add1, *add2; //Just redirect the coefficients of this and other to these two pointers, no need to del
    int len1, len2;

    if(length>=other->length){
        add1=coeff;
        len1=length;
        add2=other->coeff;
        len2=other->length;
    }else{
        add1=other->coeff;
        len1=other->length;
        add2=coeff;
        len2=length;
    }

    RF* add_result=new RF[len1];
    for(int i=0;i<len2;++i){
        add_result[i]=add1[i]+add2[i];
    }
    for(int i=len2;i<len1;++i){
        add_result[i]=add1[i];
    }

    const Polynomial* newp=polynomial_no_check(add_result,len1);

    delete[] add_result;

    return newp;
}

const Ring* Polynomial::multImpl(const Ring* r) const{
    const Polynomial *other=dynamic_cast<const Polynomial*>(r);
#if DEBUG_MODE
    if(other==nullptr){
        throw "invalid cast!";
    }
#endif

    const RF* const &c1=coeff;
    const RF* const &c2=other->coeff; //Just redirect the coefficients of this and other to these two pointers, no need to del
    const int &len1=length, &len2=other->length;
    const int deg1=len1-1, deg2=len2-1;

    RF* total=new RF[deg1+deg2+1];
    for(int deg=0; deg<=deg1+deg2; ++deg){
        for(int i=std::max(0, deg-deg2); i<=std::min(deg,deg1); ++i){
            total[deg] = total[deg]+c1[i]*c2[deg-i];
        }
    }

    const Polynomial* newp=polynomial_no_check(total,deg1+deg2+1);

    delete[] total;

    return newp;
}

const Ring* Polynomial::minusImpl(const Ring* r) const{
    const Polynomial *other=dynamic_cast<const Polynomial*>(r);
#if DEBUG_MODE
    if(other==nullptr){
        throw "invalid cast!";
    }
#endif

    const RF *add1, *add2; //Just redirect the coefficients of this and other to these two pointers, no need to del
    int len1, len2;

    if(length>=other->length){
        add1=coeff;
        len1=length;
        add2=other->coeff;
        len2=other->length;
    }else{
        add1=other->coeff;
        len1=other->length;
        add2=coeff;
        len2=length;
    }

    RF* minus_result=new RF[len1];
    for(int i=0;i<len2;++i){
        minus_result[i]=add1[i]-add2[i];
    }
    for(int i=len2;i<len1;++i){
        minus_result[i]=add1[i];
    }

    const Polynomial* newp=polynomial_no_check(minus_result,len1);

    delete[] minus_result;

    return newp;
}

const Ring* Polynomial::divImpl(const Ring* r) const{
    const Ring *quot, *rem;
    quotAndRemainder(r, quot, rem);
    delete rem;
    return quot;
}

const Ring* Polynomial::remainderImpl(const Ring* r) const {
    const Ring *quot, *rem;
    quotAndRemainder(r, quot, rem);
    delete quot;
    return rem;
}

const Polynomial* Polynomial::multiply_const(const RF& mult, int degree) const{
    Polynomial *n=new Polynomial{}; //No need to check and promote, just copy
    n->length=length+degree;
    n->coeff=RF::array_copy(coeff,length,degree);
    n->type->set_sub_type(&(type->get_sub_type()));
    n->display_terms=display_terms;

    for(int i=0;i<length+degree;i++){
        RF& elem=(n->coeff)[i];
        if(elem.is_zero()){
            elem= mult.promote(R::ZERO);
        }else{
            elem = mult*elem;
        }
    }

    return n;
}

/**
 * Polynomial long division.
*/
void Polynomial::quotAndRemainder(const Ring* r, const Ring*& quot, const Ring*& rem) const{
    const Polynomial* div=dynamic_cast<const Polynomial*>(r);
#if DEBUG_MODE
    if(div==nullptr){
        throw "invalid cast!";
    }
#endif
    if(div->is_zero()){
        throw "Divide by zero!";
    }
    if(is_zero()){
        quot = copy();
        rem = copy();
        return;
    }
    if(get_degree_no0check()<div->get_degree_no0check()){
        quot = promote(R::impl0);
        rem = copy();
        return;
    }
    const Polynomial* remainder=copy();
    const int newdeg=get_degree_no0check()-div->get_degree_no0check();
    R* new_coeff = new R[newdeg + 1];
    while (remainder->get_degree_no0check()>=div->get_degree_no0check() && !remainder->is_zero()) {
        R mult = remainder->get_leading_coefficient()/div->get_leading_coefficient(); //Here mult is non-zero since the leading coefficient is always non-zero (remainder has degree larger than div, hence remainder must be non-zero polynomial). 
        new_coeff[remainder->get_degree_no0check()-div->get_degree_no0check()] = mult; //The power is just the difference of the degree, and the multiple is the quotient above.

        const Polynomial* multiply=div->multiply_const(mult,remainder->get_degree_no0check()-div->get_degree_no0check());

        const Polynomial* temp = dynamic_cast<const Polynomial*>(remainder->minusImpl(multiply)); //temp = remainder - [ mult*t^(degree difference) ]*div
        #if DEBUG_MODE
            if(temp==nullptr){
                throw "invalid cast!";
            }
        #endif

        delete remainder; //avoid mem leak
        delete multiply;

        remainder=temp;
    }
    rem=remainder;
    quot=new Polynomial{new_coeff,newdeg+1};
    delete[] new_coeff; //avoid mem leak
}

const Ring* Polynomial::invert() const{
    if(is_zero()){
        throw "Divide by zero!";
    }
    if(length==1){
        const R& unit=coeff[0];
        R elmInv=unit.invert(); //No need to delete since it is statically allocated
        return new Polynomial{&elmInv, 1};
    }
    return promote(R::impl0); //polynomial divide 1/p(x) gives zero always
}

const Polynomial* Polynomial::negate() const{
    Polynomial *n=new Polynomial{}; //No need to check and promote, just copy
    n->length=length;
    n->coeff=RF::array_copy(coeff,length,0);
    n->type->set_sub_type(&(type->get_sub_type()));
    n->display_terms=display_terms;

    for(int i=0;i<length;i++){
        RF& elem=(n->coeff)[i];
        elem = -elem;
    }

    return n;
}

const Polynomial* Polynomial::copy() const{
    Polynomial *n=new Polynomial{}; //No need to check and promote, just copy
    n->length=length;
    n->coeff=RF::array_copy(coeff,length,0);
    n->type->set_sub_type(&(type->get_sub_type()));
    n->display_terms=display_terms;
    return n;
}

int Polynomial::euclideanFuncCompare(const Ring* other) const{
    const Polynomial *py=dynamic_cast<const Polynomial*>(other);
#if DEBUG_MODE
    if(py==nullptr){
        throw "invalid cast!";
    }
#endif

    int otherval=py->get_degree();
    int thisval=get_degree();
    return thisval==otherval? 0 : ( thisval<otherval? -1 : 1 );
}

bool Polynomial::equalsImpl(const Ring* other) const{
    const Polynomial *py=dynamic_cast<const Polynomial*>(other);
#if DEBUG_MODE
    if(py==nullptr){
        throw "invalid cast!";
    }
#endif

    if(get_degree()!=py->get_degree()){
        return false;
    }

    for(int i=0;i<length;i++){
        if(!coeff[i].exactly_equals(py->coeff[i])){
            return false;
        }
    }
    return true;
}

string Polynomial::to_string() const{
    if(is_zero()){
        return "0";
    }
    ostringstream str;
    
    bool head=true;
    for(int i=length-1;i>=0;--i){
        const R& elem=coeff[i];
        const int &deg=i;
        if(deg==0){
            if(head){
                str<<elem.to_string();
            }else if(!elem.is_zero()){
                str<<elem.to_signed_string();
            }
        }else if(!elem.is_zero()){
            if(head){
                head=false;
                str<<elem.to_leading_coeff();
            }else{
                str<<elem.to_coeff();
            }
            if(deg==1){
                str<<PY_CHAR;
            }else if(deg>1){
                str<<PY_CHAR;
                str<<deg;
            }
        }
    }

    return str.str();
}

string Polynomial::to_signed_string() const{
    if(is_zero()){
        return "+0";
    }
    ostringstream str;
    bool head=true;

    for(int i=length-1;i>=0;--i){
        const R& elem=coeff[i];
        const int &deg=i;

        if(deg==0){
            if(head || !elem.is_zero()){
                str<<elem.to_signed_string();
            }
        }else if(!elem.is_zero()){
            head=false;
            str<<elem.to_coeff();
            if(deg==1){
                str<<PY_CHAR;
            }else if(deg>1){
                str<<PY_CHAR;
                str<<deg;
            }
        }
    }

    return str.str();
}

string Polynomial::to_latex() const{
    if(is_zero()){
        return "0";
    }
    ostringstream str;

    bool head=true;
    for(int i=length-1;i>=0;--i){
        const R& elem=coeff[i];
        const int &deg=i;

        if(deg==0){
            if(head){
                str<<elem.to_latex();
            }else if(!elem.is_zero()){
                str<<elem.to_signed_latex();
            }
        }else if(!elem.is_zero()){
            if(head){
                head=false;
                str<<elem.to_latex_leading_coeff();
            }else{
                str<<elem.to_latex_coeff();
            }
            if(deg==1){
                str<<PY_CHAR;
            }else if(deg>1){
                str<<PY_CHAR;
                str<<deg;
            }
        }
    }

    return str.str();
}

string Polynomial::to_signed_latex() const{
    if(is_zero()){
        return "0";
    }
    ostringstream str;
    bool head=true;
    for(int i=length-1;i>=0;--i){
        const R& elem=coeff[i];
        const int &deg=i;

        if(deg==0){
            if(head || !elem.is_zero())
                str<<elem.to_signed_latex();
        }else if(!elem.is_zero()){
            head=false;
            str<<elem.to_latex_coeff();
            if(deg==1){
                str<<PY_CHAR;
            }else if(deg>1){
                str<<PY_CHAR;
                str<<deg;
            }
        }
    }

    return str.str();
}

bool Polynomial::needs_bracket() const{
    return display_terms!=1;
}

bool Polynomial::needs_bracket_latex() const{
    return display_terms!=1;
}

/**
 * See Ring.cpp is_type_subset
*/
const Ring* Polynomial::promote(const Ring* const& r) const{
    const NestedRingType& mytype=get_type();
    if(mytype.deep_equals(r->get_type())){
        return r->copy();
    }
    const R& el=coeff[0];
    if(is_type_subset(mytype.get_sub_type(),r->get_type())){ //the type is directly the subset given by the coefficients
        const R* promotion= new R{el.promote_exp(r)};
        const Ring* promotion2=new Polynomial{promotion,1};
        
        delete promotion;
        return promotion2;
    }

    //the type of r is still a polynomial, with both this and r having the same coefficient type
    const Polynomial* py=dynamic_cast<const Polynomial*>(r);
#if DEBUG_MODE
    if(py==nullptr){
        throw "invalid cast!";
    }
#endif
    R* ncoeff=new R[py->length];
    
    for(int i=0;i<py->length;++i){
        ncoeff[i]=el.promote(py->coeff[i]);
    }

    const Polynomial* result=new Polynomial{ncoeff,py->length};
    delete[] ncoeff;
    return result;
}

const Ring* Polynomial::promote_one() const{
    const R& el=coeff[0];
    const R* promotion= new R{el.promote_one()};
    const Ring* promotion2=new Polynomial{promotion,1};

    delete promotion;
    return promotion2;
}

const Ring* Polynomial::complexify() const{
    if(get_type().complex()){
        return copy();
    }
    Polynomial *n=new Polynomial{}; //No need to check and promote, just copy
    n->length=length;
    n->coeff=RF::array_copy(coeff,length,0);
    const NestedRingType* sub_type_new; //we copy it
    for(int i=0;i<length;i++){
        RF& elem=(n->coeff)[i];
        elem = elem.complexify();
        sub_type_new=&elem.get_type();
    }
    n->type->set_sub_type(sub_type_new); //copy here
    n->display_terms=display_terms;

    return n;
}

bool Polynomial::is_one() const{
    return length==1 && coeff[0].is_one();
}

bool Polynomial::is_unit() const{
    return length==1 && (!coeff[0].is_zero()); //Constant polynomial, with non-zero constant
}

bool Polynomial::is_zero() const{
    return length==1 && coeff[0].is_zero(); //Constant polynomial with zero constant
}

/**
 * Splits into morph=monic polynomial, where this*unit=morph.
*/
void Polynomial::split_canonical(const Ring*& morph, const Ring*& unit) const{
    if(is_zero()){
        morph=copy();
        unit=promote_one();
    }else{
        const R &leading=coeff[length-1];
        const R mult = leading.invert(); //this is in the subring. no need to delete since it is statically allocated

        unit=new Polynomial{&mult, 1};

        morph=multImpl(unit);
    }
}

int Polynomial::get_degree() const{
    return is_zero()? -1 : (length-1);
}

int Polynomial::get_degree_no0check() const{
    return length-1;
}

const RF& Polynomial::get_leading_coefficient() const{
    return coeff[length-1];
}