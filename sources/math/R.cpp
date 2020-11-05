#include "math/R.h"

using namespace std;

R::R(Ring* impl): impl(impl){
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

R::R(const R&& move_from){
    impl=move_from.impl;
}

R& R::operator=(const R& copy_from){
    delete impl;
    impl=copy_from.impl->copy();
    return *this;
}

R& R::operator=(const R&& move_from){
    delete impl;
    impl=move_from.impl;
    return *this;
}

R R::operator+(const R& other) const{
    return R{impl->addImpl(other.impl)};
}

R R::operator-(const R& other) const{
    return R{impl->minusImpl(other.impl)};
}

R R::operator*(const R& other) const{
    return R{impl->multImpl(other.impl)};
}

R R::operator/(const R& other) const{
    return R{impl->divImpl(other.impl)};
}

R R::operator%(const R& other) const{
    return R{impl->remainderImpl(other.impl)};
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