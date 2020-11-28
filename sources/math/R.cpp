#include "math/R.h"
#include "math/fraction/Fraction.h"

using namespace std;

//See R.h for details.
const ZeroElmt* const R::impl0 = new ZeroElmt{};
const R R::ZERO{R::impl0};

R::R(): impl(new ZeroElmt{}){
    
}

R::R(const Ring* impl): impl(impl){
#if DEBUG_MODE
    if(impl!=nullptr){
        ++(*(impl->RING_DEBUG_NUM_CREATE));
        if(*(impl->RING_DEBUG_NUM_CREATE) > 1){
            throw "Ring impl was in multiple wrappers!";
        }
    }
#endif
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
    if(this==&copy_from){
        return *this;
    }
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

bool R::internal_type_compatibility(const Ring* &r1, const Ring* &r2, const Ring* &to_delete){
    int i=Ring::compatibility(r1->get_type(), r2->get_type());
    if(i==-2){
        to_delete=nullptr;
        return false; //not compatible
    }else if(i==0){
        to_delete=nullptr; //compatible and no promotion needed.
    }else if(i==1){
        to_delete=r1->promote(r2); //creates newly allocated ring and stores into to_delete
        r2=to_delete;
    }else{
        to_delete=r2->promote(r1); //similar
        r1=to_delete;
    }
    return true;
}

/**
 * Binary operations/comparison. Internally, dynamically allocates a temporary Ring object if necessary to ensure the types are 
 * exactly equal before passing into the functions in Ring (and its subclasses)
*/

R R::operator+(const R& other) const{
    const Ring* thisval=impl;
    const Ring* otherval=other.impl;
    const Ring* to_delete;
    if(internal_type_compatibility(thisval, otherval, to_delete)){
        const Ring* result=thisval->addImpl(otherval);
        delete to_delete;
        return R{result};
    }else{
        throw "cannot use + on non-compatible types!";
    }
}

R R::operator-(const R& other) const{
    const Ring* thisval=impl;
    const Ring* otherval=other.impl;
    const Ring* to_delete;
    if(internal_type_compatibility(thisval, otherval, to_delete)){
        const Ring* result=thisval->minusImpl(otherval);
        delete to_delete;
        return R{result};
    }else{
        throw "cannot use - on non-compatible types!";
    }
}

R R::operator*(const R& other) const{
    const Ring* thisval=impl;
    const Ring* otherval=other.impl;
    const Ring* to_delete;
    if(internal_type_compatibility(thisval, otherval, to_delete)){
        const Ring* result=thisval->multImpl(otherval);
        delete to_delete;
        return R{result};
    }else{
        throw "cannot use * on non-compatible types!";
    }
}

R R::operator/(const R& other) const{
    if(other.is_zero()){
        throw "divide by zero!";
    }
    const Ring* thisval=impl;
    const Ring* otherval=other.impl;
    const Ring* to_delete;
    if(internal_type_compatibility(thisval, otherval, to_delete)){
        const Ring* result=thisval->divImpl(otherval);
        delete to_delete;
        return R{result};
    }else{
        throw "cannot use / on non-compatible types!";
    }
}

R R::operator%(const R& other) const{
    if(other.is_zero()){
        throw "divide by zero!";
    }
    const Ring* thisval=impl;
    const Ring* otherval=other.impl;
    const Ring* to_delete;
    if(internal_type_compatibility(thisval, otherval, to_delete)){
        const Ring* result=thisval->remainderImpl(otherval);
        delete to_delete;
        return R{result};
    }else{
        throw "cannot use % on non-compatible types!";
    }
}

void R::quotAndRemainder(const R& div, R& quot, R& rem) const{
    if(div.is_zero()){
        throw "divide by zero!";
    }
    const Ring* thisval=impl;
    const Ring* otherval=div.impl;
    const Ring* to_delete;
    if(internal_type_compatibility(thisval, otherval, to_delete)){
        const Ring *q,*r;
        thisval->quotAndRemainder(otherval, q, r);
        delete to_delete;
        quot=R{q};
        rem=R{r};
    }else{
        throw "cannot use quot and remainder on non-compatible types!";
    }
}

bool R::operator<=(const R& other) const{
    const Ring* thisval=impl;
    const Ring* otherval=other.impl;
    const Ring* to_delete;
    if(internal_type_compatibility(thisval, otherval, to_delete)){
        int result=thisval->euclideanFuncCompare(otherval);
        delete to_delete;
        return result<=0;
    }else{
        throw "cannot use <= on non-compatible types!";
    }
}

bool R::operator<(const R& other) const{
    const Ring* thisval=impl;
    const Ring* otherval=other.impl;
    const Ring* to_delete;
    if(internal_type_compatibility(thisval, otherval, to_delete)){
        int result=thisval->euclideanFuncCompare(otherval);
        delete to_delete;
        return result<0;
    }else{
        throw "cannot use < on non-compatible types!";
    }
}

bool R::operator>(const R& other) const{
    const Ring* thisval=impl;
    const Ring* otherval=other.impl;
    const Ring* to_delete;
    if(internal_type_compatibility(thisval, otherval, to_delete)){
        int result=thisval->euclideanFuncCompare(otherval);
        delete to_delete;
        return result>0;
    }else{
        throw "cannot use > on non-compatible types!";
    }
}

bool R::operator>=(const R& other) const{
    const Ring* thisval=impl;
    const Ring* otherval=other.impl;
    const Ring* to_delete;
    if(internal_type_compatibility(thisval, otherval, to_delete)){
        int result=thisval->euclideanFuncCompare(otherval);
        delete to_delete;
        return result>=0;
    }else{
        throw "cannot use >= on non-compatible types!";
    }
}

bool R::operator==(const R& other) const{
    const Ring* thisval=impl;
    const Ring* otherval=other.impl;
    const Ring* to_delete;
    if(internal_type_compatibility(thisval, otherval, to_delete)){
        int result=thisval->euclideanFuncCompare(otherval);
        delete to_delete;
        return result==0;
    }else{
        throw "cannot use == on non-compatible types!";
    }
}

int R::euclidean_func_compare(const R& other) const{
    const Ring* thisval=impl;
    const Ring* otherval=other.impl;
    const Ring* to_delete;
    if(internal_type_compatibility(thisval, otherval, to_delete)){
        int result=thisval->euclideanFuncCompare(otherval);
        delete to_delete;
        return result;
    }else{
        throw "cannot use == on non-compatible types!";
    }
}

bool R::exactly_equals(const R& other) const{
    const Ring* thisval=impl;
    const Ring* otherval=other.impl;
    const Ring* to_delete;
    if(internal_type_compatibility(thisval, otherval, to_delete)){
        bool result=thisval->equalsImpl(otherval);
        delete to_delete;
        return result;
    }else{
        throw "cannot use exactly_equals on non-compatible types!";
    }
}

/**
 * Unary operations.
*/
R R::operator-() const{
    return R{impl->negate()};
}

R R::invert() const{
    return R{impl->invert()};
}

bool R::is_zero() const{
    return impl->is_zero();
}

bool R::is_field() const{
    return impl->is_field();
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

string R::to_leading_coeff() const{
    return impl->to_leading_coeff();
}
    
string R::to_coeff() const{
    return impl->to_coeff();
}

string R::to_latex_leading_coeff() const{
    return impl->to_latex_leading_coeff();
}
    
string R::to_latex_coeff() const{
    return impl->to_latex_coeff();
}

bool R::needs_bracket() const{
    return impl->needs_bracket();
}

bool R::needs_bracket_latex() const{
    return impl->needs_bracket_latex();
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

/**
 * Promotion functions, used to obtain an R, that has the same type as this, but keeps the same value as other.
*/
R R::promote(const R& other) const{
    return R{impl->promote(other.impl)};
}

/**
 * Used internally for polynomimals and fractions.
*/
const Ring* R::promote_exp(const Ring* const& r) const{
    return impl->promote(r);
}

/**
 * Returns the element having value one with same type as this
*/
R R::promote_one() const{
    return R{impl->promote_one()};
}

/**
 * Splits into a product so that fractions can be expressed in a 'canonical way'.
*/
void R::split(R& morph, R& unit) const{
    const Ring *m, *u;
    impl->split_canonical(m,u);
    morph=R{m};
    unit=R{u};
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

R* R::array_copy(R* const& arr, int len){
    R* newarr = new R[len];
    for(int i=0;i<len;i++){
        newarr[i]=arr[i];
    }
    return newarr;
}

R* R::array_copy(R* const& arr, int len, int shift){
    R* newarr = new R[len+shift];
    for(int i=0;i<len;i++){
        newarr[i+shift]=arr[i];
    }
    return newarr;
}

R R::complexify() const{
    if(get_type().complex()){
        return *this;
    }
    return R{impl->complexify()};
}

bool R::complexify_if_needed(R& a, R& b){
    if(a.is_type_compatible(b)){
        return true;
    }
    R cpx=a.complexify();
    if(cpx.is_type_compatible(b)){
        a=cpx;
        return true;
    }
    cpx=b.complexify();
    if(cpx.is_type_compatible(a)){
        b=cpx;
        return true;
    }
    return false;
}


/**
 * Destructor, copy constructor, etc.
*/

RF::RF(const R& copy_from) : R{copy_from}{
    //calls super copy constructor
}

RF::RF(const RF& copy_from) : R{copy_from}{
    //calls super copy constructor
}

RF::RF(RF&& move_from) : R{}{
    //calls default super constructor first, so we need to delete impl, as it is assigned to be zero.
    delete impl;
    impl=move_from.impl;
    move_from.impl=nullptr;
}

RF::RF(const Ring* r) : R{r}{

}

RF::RF() : R{} {

}

RF& RF::operator=(const R& copy_from){
    if(this==&copy_from){
        return *this;
    }
    delete impl;
    impl=copy_from.impl->copy();
    return *this;
}

RF& RF::operator=(R&& move_from){
    delete impl;
    impl=move_from.impl;
    move_from.impl=nullptr;
    return *this;
}

RF& RF::operator=(const RF& copy_from){
    if(this==&copy_from){
        return *this;
    }
    delete impl;
    impl=copy_from.impl->copy();
    return *this;
}

RF& RF::operator=(RF&& move_from){
    delete impl;
    impl=move_from.impl;
    move_from.impl=nullptr;
    return *this;
}


/**
 * Fast R operators, do not perform type checking/promotion
*/

R RF::operator+(const R& other) const{
    return R{impl->addImpl(other.impl)};
}

R RF::operator-(const R& other) const{
    return R{impl->minusImpl(other.impl)};
}

R RF::operator-() const{
    return R::operator-();
}

R RF::operator*(const R& other) const{
    return R{impl->multImpl(other.impl)};
}

R RF::operator/(const R& other) const{
    return R{impl->divImpl(other.impl)};
}

R RF::operator%(const R& other) const{
    return R{impl->remainderImpl(other.impl)};
}

void RF::quotAndRemainder(const R& div, R& quot, R& rem) const{
    const Ring *q, *r;
    impl->quotAndRemainder(div.impl, q, r);

    quot=R{q};
    rem=R{r};
}

bool RF::operator<=(const R& other) const{
    int compare=impl->euclideanFuncCompare(other.impl);
    
    return compare<=0;
}

bool RF::operator<(const R& other) const{
    int compare=impl->euclideanFuncCompare(other.impl);

    return compare<0;
}

bool RF::operator>(const R& other) const{
    int compare=impl->euclideanFuncCompare(other.impl);

    return compare>0;
}

bool RF::operator>=(const R& other) const{
    int compare=impl->euclideanFuncCompare(other.impl);

    return compare>=0;
}

bool RF::operator==(const R& other) const{
    int compare=impl->euclideanFuncCompare(other.impl);

    return compare==0;
}

bool RF::exactly_equals(const R& other) const{
    return impl->equalsImpl(other.impl);
}

int RF::euclidean_func_compare(const R& other) const{
    return impl->euclideanFuncCompare(other.impl);
}

RF* RF::array_copy(const RF* const& arr, int len, int shift){
    RF* newarr = new RF[len+shift];
    for(int i=0;i<len;i++){
        newarr[i+shift]=arr[i];
    }
    return newarr;
}

RF* RF::subarray_copy(const RF* const& arr, int start, int end){
    RF* newarr = new RF[end-start];
    for(int i=start;i<end;i++){
        newarr[i-start]=arr[i];
    }
    return newarr;
}

bool RF::ensure_types_equal(RF* const* const arr, int rows, int cols){
    R largest=R::ZERO;
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(i==0 && j==0){
                largest=arr[i][j];
                continue;
            }
            if(R::complexify_if_needed(largest, arr[i][j])){
                const NestedRingType& mattype=arr[i][j].get_type();
                const NestedRingType& largetype=largest.get_type();
                if(Ring::is_type_subset(mattype,largetype)){
                    largest=arr[i][j];
                }
            }else{
                return false;
            }
        }
    }

    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            arr[i][j]=largest.promote(arr[i][j]);
        }
    }

    return true;
}

RF** RF::copy_and_promote_if_compatible(const R* const* const mat, int rows, int cols){
    RF **new_mat=new RF*[rows];
    for(int i=0;i<rows;i++){
        new_mat[i]=new RF[cols];
        for(int j=0;j<cols;j++){
            new_mat[i][j]=mat[i][j];
        }
    }
    if(ensure_types_equal(new_mat,rows,cols)){
        return new_mat;
    }else{
        deallocate_matrix(new_mat,rows); //not possible, return nullptr
        return nullptr;
    }
}

void RF::deallocate_matrix(RF** matrix, int rows){
    for(int i=0;i<rows;i++){
        delete[] matrix[i];
    }
    delete[] matrix;
}

void RF::promote_to_field(RF* const* const mat, int rows, int cols){
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(!mat[i][j].is_field()){
                R one=mat[i][j].promote_one();
                mat[i][j]=RF{new Fraction{mat[i][j],one}};
            }
        }
    }
}