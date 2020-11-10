#include "math/Ring.h"
#include <sstream>

using namespace std;

/**
 * RING TYPE FUNCTIONS
*/

NestedRingType::NestedRingType(const RingType& cur_type){
    current_type=cur_type;
    sub_type=nullptr;
    is_complex=cur_type==RingType::COMPLEXIFY;
    no_fraction = cur_type==RingType::FRACTION? 1 : 0;
    no_polynomial = cur_type==RingType::POLYNOMIAL? 1 : 0;
}

NestedRingType::NestedRingType(const RingType& cur_type,NestedRingType* type){
    current_type=cur_type;
    sub_type=nullptr;
    set_sub_type_no_copy(type);
}

NestedRingType::~NestedRingType(){
    delete sub_type;
}

NestedRingType::NestedRingType(const NestedRingType& type){
    current_type=type.current_type;
    sub_type=nullptr;
    set_sub_type(type.sub_type);
}

NestedRingType::NestedRingType(NestedRingType&& type){
    current_type=type.current_type;
    sub_type=nullptr;
    set_sub_type_no_copy(type.sub_type);
    type.sub_type=nullptr;
}

NestedRingType& NestedRingType::operator=(const NestedRingType& type){
    current_type=type.current_type;
    set_sub_type(type.sub_type);
    return *this;
}

NestedRingType& NestedRingType::operator=(NestedRingType&& type){
    current_type=type.current_type;
    set_sub_type_no_copy(type.sub_type);
    type.sub_type=nullptr;
    return *this;
}

bool NestedRingType::has_sub_type() const{
    return sub_type!=nullptr;
}

const NestedRingType& NestedRingType::get_sub_type() const{
    return *sub_type;
}

bool NestedRingType::deep_equals(const NestedRingType& other) const{
    if(current_type!=other.current_type){
        return false;
    }
    if(sub_type!=nullptr){
        if(other.sub_type==nullptr){
            //this->sub_type is not nullptr but other is
            return false;
        }
        return sub_type->deep_equals(*other.sub_type);
    }else{
        if(other.sub_type!=nullptr){
            //this->sub_type is nullptr but other isn't
            return false;
        }
        //both nullptr
        return true;
    }
}

NestedRingType& NestedRingType::set_sub_type(const NestedRingType* type){
    delete sub_type;

    is_complex= current_type==RingType::COMPLEXIFY || (type!=nullptr && type->is_complex);
    if(type!=nullptr){
        no_fraction=type->no_fraction;
        no_polynomial=type->no_polynomial;
        sub_type=type->deep_copy();
    }else{
        no_fraction=0;
        no_polynomial=0;
        sub_type=nullptr;
    }
    if(current_type==RingType::FRACTION){
        ++no_fraction;
    }
    if(current_type==RingType::POLYNOMIAL){
        ++no_polynomial;
    }
    return *this;
}

NestedRingType& NestedRingType::set_sub_type_no_copy(NestedRingType* type){
    delete sub_type;

    sub_type=type;
    is_complex= current_type==RingType::COMPLEXIFY || (type!=nullptr && type->is_complex);
    if(type!=nullptr){
        no_fraction=type->no_fraction;
        no_polynomial=type->no_polynomial;
    }else{
        no_fraction=0;
        no_polynomial=0;
    }
    if(current_type==RingType::FRACTION){
        ++no_fraction;
    }
    if(current_type==RingType::POLYNOMIAL){
        ++no_polynomial;
    }
    
    return *this;
}

const RingType& NestedRingType::get_current_type() const{
    return current_type;
}

NestedRingType* NestedRingType::deep_copy() const{
    NestedRingType* n=new NestedRingType{current_type};

    if(sub_type!=nullptr){
        n->sub_type=sub_type->deep_copy();
    }
    return n;
}

bool Ring::is_type_compatible_shallow(const RingType& r1,const RingType& r2){
    return r1==r2 || r1==RingType::SPECIAL_ZERO || r2==RingType::SPECIAL_ZERO;
}

bool Ring::is_type_compatible(const Ring& other) const{
    return is_type_compatible(*type,*other.type);
}

bool Ring::is_type_subset(const NestedRingType& supset,const NestedRingType& subset){
    if(subset.get_current_type()==RingType::SPECIAL_ZERO){
        return true;
    }
    if(supset.has_sub_type()){
        if(is_type_subset(supset.get_sub_type(),subset)){
            return true;
        }

        if(subset.has_sub_type()){
            if(supset.get_current_type() == subset.get_current_type()){
                if(is_type_subset(supset.get_sub_type(), subset.get_sub_type())){
                    return true;
                }
            }
        }
    }

    if( (!supset.has_sub_type()) && (!subset.has_sub_type())){
        return is_type_compatible_shallow(supset.get_current_type(),subset.get_current_type());
    }
    return false;
}

bool Ring::is_type_compatible(const NestedRingType& r1,const NestedRingType& r2){
    return is_type_subset(r1,r2) || is_type_subset(r2,r1);
}

int Ring::compatibility(const NestedRingType& a,const NestedRingType& b){
    if(a.deep_equals(b)){
        return 0;
    }
    if(is_type_subset(a,b)){
        return 1;
    }
    if(is_type_subset(b,a)){
        return -1;
    }
    return -2;
}

string ring_type_to_string(const RingType& type){
    switch(type){
    case SPECIAL_ZERO:
        return "ZERO";
    case DOUBLE:
        return "DOUBLE";
    case LONG:
        return "LONG";
    case FRACTION:
        return "FRACTION";
    case POLYNOMIAL:
        return "POLYNOMIAL";
    case COMPLEXIFY:
        return "COMPLEX";
    }
    return "ERROR UNKNOWN TYPE";
}

string NestedRingType::to_string() const{
    ostringstream str;

    str<<ring_type_to_string(current_type);

    NestedRingType* itr=sub_type;
    while(itr!=nullptr){
        str<<" "<<ring_type_to_string(itr->current_type);
        itr=itr->sub_type;
    }

    return str.str();
}

std::ostream& operator<< (std::ostream& out, const NestedRingType& type){
    out<<type.to_string();
    return out;
}

/**
 * RING FUNCTIONS
*/

Ring::Ring(RingType t): type_shallow(t){
    type=new NestedRingType{t};
#if DEBUG_MODE
    RING_DEBUG_NUM_CREATE=new int{0};
#endif
}

Ring::~Ring(){
    delete type;
#if DEBUG_MODE
    delete RING_DEBUG_NUM_CREATE;
#endif
}

const NestedRingType& Ring::get_type() const{
    return *type;
}

void Ring::quotAndRemainder(const Ring* div, const Ring*& quot, const Ring*& rem) const{
    quot=divImpl(div);
    rem=remainderImpl(div);
}

bool Ring::is_zero() const{
    const Ring* pro=promote(R::impl0);
    bool result=equalsImpl(pro);
    delete pro;
    return result;
}

bool Ring::is_neg_one() const{
    const Ring* r=negate();
    bool result=r->is_one();
    delete r;
    return result;
}

std::string Ring::to_coeff() const{
    if(is_one()){
        return "+";
    }
    if(is_neg_one()){
        return "-";
    }
    if(needs_bracket()){
        return "+("+to_string()+")";
    }
    return to_signed_string();
}
    
std::string Ring::to_leading_coeff() const{
    if(is_one()){
        return "";
    }
    if(is_neg_one()){
        return "-";
    }
    if(needs_bracket()){
        return "("+to_string()+")";
    }
    return to_string();
}

std::string Ring::to_latex_coeff() const{
    if(is_one()){
        return "+";
    }
    if(is_neg_one()){
        return "-";
    }
    if(needs_bracket_latex()){
        return "+("+to_latex()+")";
    }
    return to_signed_latex();
}

std::string Ring::to_latex_leading_coeff() const{
    if(is_one()){
        return "";
    }
    if(is_neg_one()){
        return "-";
    }
    if(needs_bracket_latex()){
        return "("+to_latex()+")";
    }
    return to_latex();
}

bool Ring::needs_bracket() const{
    return false;
}

bool Ring::needs_bracket_latex() const{
    return false;
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
    return compare->type_shallow==RingType::SPECIAL_ZERO || compare->equalsImpl(this);
}

const Ring* ZeroElmt::invert() const{
    throw "Cannot invert zero element!";
}

/**
 * The euclidean function are equal if and only if both are zero, otherwise zero must be smaller than other elements.
*/
int ZeroElmt::euclideanFuncCompare(const Ring* other) const{
    return equalsImpl(other)? 0 : -1;
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

std::string ZeroElmt::to_coeff() const{
    return "+0";
}
    
std::string ZeroElmt::to_leading_coeff() const{
    return "0";
}

std::string ZeroElmt::to_latex_coeff() const{
    return "+0";
}

std::string ZeroElmt::to_latex_leading_coeff() const{
    return "0";
}

const Ring* ZeroElmt::promote(const Ring* const& r) const{
    throw "It should not be needed to promote any element to match the type of ZeroElmt!";
}

const Ring* ZeroElmt::promote_one() const{
    throw "It should not be needed to promote the unit element to match the type of ZeroElmt!";
}

bool ZeroElmt::is_unit() const{
    return false;
}

bool ZeroElmt::is_one() const{
    return false;
}

bool ZeroElmt::is_zero() const{
    return true;
}

bool ZeroElmt::is_neg_one() const{
    return false;
}

void ZeroElmt::split_canonical(const Ring*& r1, const Ring*& r2) const{
    throw "Cannot split the zero element!";
}