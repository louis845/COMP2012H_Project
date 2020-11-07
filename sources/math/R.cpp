#include "math/R.h"

using namespace std;

//See R.h for details.
const ZeroElmt* const R::impl0 = new ZeroElmt{};
const R R::ZERO{R::impl0};

R::R(const Ring* impl): impl(impl){
#if DEBUG_MODE
    if(impl==nullptr){
        throw "impl cannot be nullptr!";
    }
#endif
}

R::~R(){
    delete impl;
}

R::R(const R& copy_from){
    impl=copy_from.impl->copy();
}

R::R(R&& move_from){
    impl=move_from.impl;
    move_from.impl=nullptr;
}

R& R::operator=(const R& copy_from){
    delete impl;
    impl=copy_from.impl->copy();
    return *this;
}

R& R::operator=(R&& move_from){
    delete impl;
    impl=move_from.impl;
    move_from.impl=nullptr;
    return *this;
}

R R::operator+(const R& other) const{
#if DEBUG_MODE
    if(!(impl->is_type_compatible(*other.impl))){
        throw "cannot use + on non-same types!";
    }
#endif
    return R{impl->addImpl(other.impl)};
}

R R::operator-(const R& other) const{
#if DEBUG_MODE
    if(!(impl->is_type_compatible(*other.impl))){
        throw "cannot use - on non-same types!";
    }
#endif
    return R{impl->minusImpl(other.impl)};
}

R R::operator*(const R& other) const{
#if DEBUG_MODE
    if(!(impl->is_type_compatible(*other.impl))){
        throw "cannot use * on non-same types!";
    }
#endif
    return R{impl->multImpl(other.impl)};
}

R R::operator/(const R& other) const{
#if DEBUG_MODE
    if(!(impl->is_type_compatible(*other.impl))){
        throw "cannot use / on non-same types!";
    }
#endif
    return R{impl->divImpl(other.impl)};
}

R R::operator%(const R& other) const{
#if DEBUG_MODE
    if(!(impl->is_type_compatible(*other.impl))){
        throw "cannot use / on non-same types!";
    }
#endif
    return R{impl->remainderImpl(other.impl)};
}

bool R::operator<=(const R& other) const{
#if DEBUG_MODE
    if(!(impl->is_type_compatible(*other.impl))){
        throw "cannot use == on non-same types!";
    }
#endif
    return impl->euclideanFuncCompare(other.impl) <= 0;
}

bool R::operator<(const R& other) const{
#if DEBUG_MODE
    if(!(impl->is_type_compatible(*other.impl))){
        throw "cannot use == on non-same types!";
    }
#endif
    return impl->euclideanFuncCompare(other.impl) < 0;
}

bool R::operator>(const R& other) const{
#if DEBUG_MODE
    if(!(impl->is_type_compatible(*other.impl))){
        throw "cannot use == on non-same types!";
    }
#endif
    return impl->euclideanFuncCompare(other.impl) > 0;
}

bool R::operator>=(const R& other) const{
#if DEBUG_MODE
    if(!(impl->is_type_compatible(*other.impl))){
        throw "cannot use == on non-same types!";
    }
#endif
    return impl->euclideanFuncCompare(other.impl) >= 0;
}

bool R::operator==(const R& other) const{
#if DEBUG_MODE
    if(!(impl->is_type_compatible(*other.impl))){
        throw "cannot use == on non-same types!";
    }
#endif
    return impl->euclideanFuncCompare(other.impl) == 0;
}

bool R::exactly_equals(const R& other) const{
#if DEBUG_MODE
    if(!(impl->is_type_compatible(*other.impl))){
        throw "cannot use == on non-same types!";
    }
#endif
    return impl->equalsImpl(impl);
}

bool R::is_zero() const{
    return impl->type==RingType::SPECIAL_ZERO || impl->equalsImpl(R::impl0);
}

int R::euclidean_func_compare(const R& other) const{
    return impl->euclideanFuncCompare(other.impl);
}

string R::to_string() const{
    return impl->to_string();
}
    
string R::to_signed_string() const{
    return impl->to_signed_string();
}

string R::to_latex() const{
    return impl->to_latex();
}
    
string R::to_signed_latex() const{
    return impl->to_signed_latex();
}

std::ostream& operator<< (std::ostream& out, const R& val){
    out<<val.to_string();
    return out;
}
