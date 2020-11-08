#include "math/Ring.h"

using namespace std;

/**
 * RING TYPE FUNCTIONS
*/

NestedRingType::NestedRingType(const RingType& cur_type){
    current_type=cur_type;
    sub_type=nullptr;
}

NestedRingType::NestedRingType(const RingType& cur_type,NestedRingType* type){
    current_type=cur_type;
    sub_type=type;
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
    sub_type=type.sub_type;
    type.sub_type=nullptr;
}

NestedRingType& NestedRingType::operator=(const NestedRingType& type){
    current_type=type.current_type;
    set_sub_type(type.sub_type);
    return *this;
}

NestedRingType& NestedRingType::operator=(NestedRingType&& type){
    current_type=type.current_type;
    delete sub_type;
    sub_type=type.sub_type;
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

NestedRingType& NestedRingType::set_sub_type(NestedRingType* type){
    delete sub_type;

    if(type!=nullptr){
        sub_type=type->deep_copy();
    }else{
        sub_type=nullptr;
    }
    return *this;
}

NestedRingType& NestedRingType::set_sub_type_no_copy(NestedRingType* type){
    delete sub_type;

    sub_type=type;
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

/**
 * RING FUNCTIONS
*/

Ring::Ring(RingType t): type_shallow(t){
    type=new NestedRingType{t};
}

Ring::~Ring(){
    delete type;
}

const NestedRingType& Ring::get_type() const{
    return *type;
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

const Ring* ZeroElmt::promote(const Ring* const& r) const{
    throw "It should not be needed to promote any element to match the type of ZeroElmt!";
}