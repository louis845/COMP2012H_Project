#include "math/R.h"

using namespace std;

//See R.h for details.
const ZeroElmt* const R::impl0 = new ZeroElmt{};
const R R::ZERO{R::impl0};

R::R(const Ring* impl): impl(impl){
}

/**
 * Destructor, copy constructor, etc.
*/
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

/**
 * Binary operations/comparison
*/

R R::operator+(const R& other) const{
    int i=Ring::compatibility(impl->get_type(), other.impl->get_type());
    if(i==-2){
        throw "cannot use + on non-compatible types!";
    }

    if(other.get_type_shallow()==RingType::SPECIAL_ZERO){
        return R{impl->copy()};
    }

    if(i==0){
        return R{impl->addImpl(other.impl)};
    }else if(i==1){
        const Ring* pro=impl->promote(other.impl);
        const Ring* result=impl->addImpl(pro);
        delete pro;
        return R{result};
    }else{
        const Ring* pro=other.impl->promote(impl);
        const Ring* result=pro->addImpl(other.impl);
        delete pro;
        return R{result};
    }
}

R R::operator-(const R& other) const{
    int i=Ring::compatibility(impl->get_type(), other.impl->get_type());

    if(i==-2){
        throw "cannot use - on non-compatible types!";
    }

    if(other.get_type_shallow()==RingType::SPECIAL_ZERO){
        return R{impl->copy()};
    }

    if(i==0){
        return R{impl->minusImpl(other.impl)};
    }else if(i==1){
        const Ring* pro=impl->promote(other.impl);
        const Ring* result=impl->minusImpl(pro);
        delete pro;
        return R{result};
    }else{
        const Ring* pro=other.impl->promote(impl);
        const Ring* result=pro->minusImpl(other.impl);
        delete pro;
        return R{result};
    }
}

R R::operator*(const R& other) const{
    int i=Ring::compatibility(impl->get_type(), other.impl->get_type());

    if(i==-2){
        throw "cannot use * on non-compatible types!";
    }

    if(other.get_type_shallow()==RingType::SPECIAL_ZERO){
        return R{other.impl->copy()};
    }
    
    if(i==0){
        return R{impl->multImpl(other.impl)};
    }else if(i==1){
        const Ring* pro=impl->promote(other.impl);
        const Ring* result=impl->multImpl(pro);
        delete pro;
        return R{result};
    }else{
        const Ring* pro=other.impl->promote(impl);
        const Ring* result=pro->multImpl(other.impl);
        delete pro;
        return R{result};
    }
}

R R::operator/(const R& other) const{
    int i=Ring::compatibility(impl->get_type(), other.impl->get_type());

    if(i==-2){
        throw "cannot use / on non-compatible types!";
    }

    if(other.get_type_shallow()==RingType::SPECIAL_ZERO){
        throw "Divide by zero!";
    }

    if(i==0){
        return R{impl->divImpl(other.impl)};
    }else if(i==1){
        const Ring* pro=impl->promote(other.impl);
        const Ring* result=impl->divImpl(pro);
        delete pro;
        return R{result};
    }else{
        const Ring* pro=other.impl->promote(impl);
        const Ring* result=pro->divImpl(other.impl);
        delete pro;
        return R{result};
    }
}

R R::operator%(const R& other) const{
    int i=Ring::compatibility(impl->get_type(), other.impl->get_type());

    if(i==-2){
        throw "cannot use % on non-compatible types!";
    }

    if(other.get_type_shallow()==RingType::SPECIAL_ZERO){
        throw "Divide by zero!";
    }

    if(i==0){
        return R{impl->remainderImpl(other.impl)};
    }else if(i==1){
        const Ring* pro=impl->promote(other.impl);
        const Ring* result=impl->remainderImpl(pro);
        delete pro;
        return R{result};
    }else{
        const Ring* pro=other.impl->promote(impl);
        const Ring* result=pro->remainderImpl(other.impl);
        delete pro;
        return R{result};
    }
}

bool R::operator<=(const R& other) const{
    int compat=Ring::compatibility(impl->get_type(), other.impl->get_type());

    if(compat==-2){
        throw "cannot use <= on non-compatible types!";
    }

    int compare;

    if(compat==0){
        compare=impl->euclideanFuncCompare(other.impl);
    }else if(compat==1){
        const Ring* pro=impl->promote(other.impl);
        compare=impl->euclideanFuncCompare(pro);
        delete pro;
    }else{
        const Ring* pro=other.impl->promote(impl);
        compare=pro->euclideanFuncCompare(other.impl);
        delete pro;
    }

    return compare<=0;
}

bool R::operator<(const R& other) const{
    int compat=Ring::compatibility(impl->get_type(), other.impl->get_type());

    if(compat==-2){
        throw "cannot use < on non-compatible types!";
    }

    int compare;

    if(compat==0){
        compare=impl->euclideanFuncCompare(other.impl);
    }else if(compat==1){
        const Ring* pro=impl->promote(other.impl);
        compare=impl->euclideanFuncCompare(pro);
        delete pro;
    }else{
        const Ring* pro=other.impl->promote(impl);
        compare=pro->euclideanFuncCompare(other.impl);
        delete pro;
    }

    return compare<0;
}

bool R::operator>(const R& other) const{
    int compat=Ring::compatibility(impl->get_type(), other.impl->get_type());

    if(compat==-2){
        throw "cannot use > on non-compatible types!";
    }

    int compare;

    if(compat==0){
        compare=impl->euclideanFuncCompare(other.impl);
    }else if(compat==1){
        const Ring* pro=impl->promote(other.impl);
        compare=impl->euclideanFuncCompare(pro);
        delete pro;
    }else{
        const Ring* pro=other.impl->promote(impl);
        compare=pro->euclideanFuncCompare(other.impl);
        delete pro;
    }

    return compare>0;
}

bool R::operator>=(const R& other) const{
    int compat=Ring::compatibility(impl->get_type(), other.impl->get_type());

    if(compat==-2){
        throw "cannot use >= on non-compatible types!";
    }

    int compare;

    if(compat==0){
        compare=impl->euclideanFuncCompare(other.impl);
    }else if(compat==1){
        const Ring* pro=impl->promote(other.impl);
        compare=impl->euclideanFuncCompare(pro);
        delete pro;
    }else{
        const Ring* pro=other.impl->promote(impl);
        compare=pro->euclideanFuncCompare(other.impl);
        delete pro;
    }

    return compare>=0;
}

bool R::operator==(const R& other) const{
    int compat=Ring::compatibility(impl->get_type(), other.impl->get_type());

    if(compat==-2){
        throw "cannot use == on non-compatible types!";
    }

    int compare;

    if(compat==0){
        compare=impl->euclideanFuncCompare(other.impl);
    }else if(compat==1){
        const Ring* pro=impl->promote(other.impl);
        compare=impl->euclideanFuncCompare(pro);
        delete pro;
    }else{
        const Ring* pro=other.impl->promote(impl);
        compare=pro->euclideanFuncCompare(other.impl);
        delete pro;
    }

    return compare==0;
}

bool R::exactly_equals(const R& other) const{

    int i=Ring::compatibility(impl->get_type(), other.impl->get_type());

    if(i==-2){
        throw "cannot use exactly_equals on non-compatible types!";
    }

    if(i==0){
        return impl->equalsImpl(other.impl);
    }else if(i==1){
        const Ring* pro=impl->promote(other.impl);
        bool result=impl->equalsImpl(pro);
        delete pro;
        return result;
    }else{
        const Ring* pro=other.impl->promote(impl);
        bool result=pro->equalsImpl(other.impl);
        delete pro;
        return result;
    }
}

R R::operator-() const{
    return R{impl->negate()};
}

bool R::is_zero() const{
    return impl->type_shallow==RingType::SPECIAL_ZERO || impl->equalsImpl(R::impl0);
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

const RingType& R::get_type_shallow() const{
    return impl->type_shallow;
}

const NestedRingType& R::get_type() const{
    return impl->get_type();
}

bool R::is_type_compatible(const R& o) const{
    return Ring::is_type_compatible(get_type(),o.get_type());
}

R R::promote(const R& other) const{
    return R{impl->promote(other.impl)};
}

void R::split(R& morph, R& unit) const{
    const Ring *m, *u;
    impl->split_canonical(m,u);
    morph=R{m};
    unit=R{u};
}

void R::quotAndRemainder(const R& div, R& quot, R& rem) const{
    const Ring *q, *r;
    impl->quotAndRemainder(div.impl, q, r);
    quot=R{q};
    rem=R{r};
}

bool R::is_one() const{
    return impl->is_one();
}

bool R::is_unit() const{
    return impl->is_unit();
}

std::ostream& operator<< (std::ostream& out, const R& val){
    out<<val.to_string();
    return out;
}