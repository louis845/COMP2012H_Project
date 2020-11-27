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

/**
 * Binary operations/comparison. These operations compare/do operation on the underlying Ring classes stored in R.
 * The operations ensure that the types passed to the Ring* class are exactly equal, before passing into the corresponding
 * functions in the Ring class.
*/

/**
 * Helper function, promotes the type if possible, returns true if it can be done, returns false otherwise.
 * The references would be changed accordingly, and the temp_value will be assigned to a pointer referring to
 * the newly created temp object.
*/
bool R::create_temp_promotion_object(const Ring* &r1,const Ring* &r2,const Ring* &to_delete){
    int i=Ring::compatibility(r1->get_type(), r2->get_type());
    if(i==-2){
        return false;
    }
    if(i==0){
        to_delete=nullptr; //No promotion needed, no temporary obj created.
    }else if(i==1){
        to_delete=r1->promote(r2); //Promote r2 to r1
        r2=to_delete;
    }else{
        to_delete=r2->promote(r1); //Promote r1 to r2
        r1=to_delete;
    }
    return true;
}

R R::operator+(const R& other) const{
    const Ring* thisval=impl;
    const Ring* otherval=other.impl;
    const Ring* to_delete;
    if(create_temp_promotion_object(thisval,otherval,to_delete)){
        const Ring* result=thisval->addImpl(otherval);
        delete to_delete; //Delete temporary allocated value, if it exists. If it doesn't we are deleting nullptr anyway.
        return R{result};
    }else{
        throw "cannot use + on non-compatible types!";
    }
}

R R::operator-(const R& other) const{
    const Ring* thisval=impl;
    const Ring* otherval=other.impl;
    const Ring* to_delete;
    if(create_temp_promotion_object(thisval,otherval,to_delete)){
        const Ring* result=thisval->minusImpl(otherval);
        delete to_delete; //Delete temporary allocated value, if it exists. If it doesn't we are deleting nullptr anyway.
        return R{result};
    }else{
        throw "cannot use - on non-compatible types!";
    }
}

R R::operator*(const R& other) const{
    const Ring* thisval=impl;
    const Ring* otherval=other.impl;
    const Ring* to_delete;
    if(create_temp_promotion_object(thisval,otherval,to_delete)){
        const Ring* result=thisval->multImpl(otherval);
        delete to_delete; //Delete temporary allocated value, if it exists. If it doesn't we are deleting nullptr anyway.
        return R{result};
    }else{
        throw "cannot use * on non-compatible types!";
    }
}

R R::operator/(const R& other) const{
    if(other.is_zero()){
        throw "Divide by zero!";
    }
    const Ring* thisval=impl;
    const Ring* otherval=other.impl;
    const Ring* to_delete;
    if(create_temp_promotion_object(thisval,otherval,to_delete)){
        const Ring* result=thisval->divImpl(otherval);
        delete to_delete; //Delete temporary allocated value, if it exists. If it doesn't we are deleting nullptr anyway.
        return R{result};
    }else{
        throw "cannot use / on non-compatible types!";
    }
}

R R::operator%(const R& other) const{
    if(other.is_zero()){
        throw "Divide by zero!";
    }
    const Ring* thisval=impl;
    const Ring* otherval=other.impl;
    const Ring* to_delete;
    if(create_temp_promotion_object(thisval,otherval,to_delete)){
        const Ring* result=thisval->remainderImpl(otherval);
        delete to_delete; //Delete temporary allocated value, if it exists. If it doesn't we are deleting nullptr anyway.
        return R{result};
    }else{
        throw "cannot use % on non-compatible types!";
    }
}

void R::quotAndRemainder(const R& div, R& quot, R& rem) const{
    if(div.is_zero()){
        throw "Divide by zero!";
    }
    int i=Ring::compatibility(impl->get_type(), div.impl->get_type());

    if(i==-2){
        throw "cannot use % on non-compatible types!";
    }

    const Ring *q, *r;

    if(i==0){
        impl->quotAndRemainder(div.impl, q, r);
    }else if(i==1){
        const Ring* pro=impl->promote(div.impl);
        impl->quotAndRemainder(pro, q, r);
        delete pro;
    }else{
        const Ring* pro=div.impl->promote(impl);
        pro->quotAndRemainder(div.impl, q, r);
        delete pro;
    }
    quot=R{q};
    rem=R{r};
}

bool R::operator<=(const R& other) const{
    const Ring* thisval=impl;
    const Ring* otherval=other.impl;
    const Ring* to_delete;
    if(create_temp_promotion_object(thisval,otherval,to_delete)){
        int compare=thisval->euclideanFuncCompare(otherval);
        delete to_delete; //Delete temporary allocated value, if it exists. If it doesn't we are deleting nullptr anyway.
        return compare<=0;
    }else{
        throw "cannot use <= on non-compatible types!";
    }
}

bool R::operator<(const R& other) const{
    const Ring* thisval=impl;
    const Ring* otherval=other.impl;
    const Ring* to_delete;
    if(create_temp_promotion_object(thisval,otherval,to_delete)){
        int compare=thisval->euclideanFuncCompare(otherval);
        delete to_delete; //Delete temporary allocated value, if it exists. If it doesn't we are deleting nullptr anyway.
        return compare<0;
    }else{
        throw "cannot use < on non-compatible types!";
    }
}

bool R::operator>(const R& other) const{
    const Ring* thisval=impl;
    const Ring* otherval=other.impl;
    const Ring* to_delete;
    if(create_temp_promotion_object(thisval,otherval,to_delete)){
        int compare=thisval->euclideanFuncCompare(otherval);
        delete to_delete; //Delete temporary allocated value, if it exists. If it doesn't we are deleting nullptr anyway.
        return compare>0;
    }else{
        throw "cannot use > on non-compatible types!";
    }
}

bool R::operator>=(const R& other) const{
    const Ring* thisval=impl;
    const Ring* otherval=other.impl;
    const Ring* to_delete;
    if(create_temp_promotion_object(thisval,otherval,to_delete)){
        int compare=thisval->euclideanFuncCompare(otherval);
        delete to_delete; //Delete temporary allocated value, if it exists. If it doesn't we are deleting nullptr anyway.
        return compare>=0;
    }else{
        throw "cannot use >= on non-compatible types!";
    }
}

bool R::operator==(const R& other) const{
    const Ring* thisval=impl;
    const Ring* otherval=other.impl;
    const Ring* to_delete;
    if(create_temp_promotion_object(thisval,otherval,to_delete)){
        int compare=thisval->euclideanFuncCompare(otherval);
        delete to_delete; //Delete temporary allocated value, if it exists. If it doesn't we are deleting nullptr anyway.
        return compare==0;
    }else{
        throw "cannot use == on non-compatible types!";
    }
}

bool R::exactly_equals(const R& other) const{
    const Ring* thisval=impl;
    const Ring* otherval=other.impl;
    const Ring* to_delete;
    if(create_temp_promotion_object(thisval,otherval,to_delete)){
        bool compare=thisval->equalsImpl(otherval);
        delete to_delete; //Delete temporary allocated value, if it exists. If it doesn't we are deleting nullptr anyway.
        return compare;
    }else{
        throw "cannot use exactly_equals on non-compatible types!";
    }
}

/**
 * Unary operations/functions. Simpler implementation here since no type promotions are needed.
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

/**
 * Returns the type of the R element.
*/
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
 * Promotes the other to match the type of this. Assumes that the type of this is directly a superset of other.
*/
R R::promote(const R& other) const{
    return R{impl->promote(other.impl)};
}

/**
 * Promotes, with allocating a new Ring*. Used internally in Fraction/Polynomial classes
*/
const Ring* R::promote_exp(const Ring* const& r) const{
    return impl->promote(r);
}

/**
 * Returns and R with the same value as one, with the same type.
*/
R R::promote_one() const{
    return R{impl->promote_one()};
}

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

RF** RF::matrix_copy_into_equal_types(const R* const* const matrix, int rows, int cols){
    RF** mat=new RF*[rows];
    for(int i=0;i<rows;i++){
        mat[i]=new RF[cols];
        for(int j=0;j<cols;j++){
            mat[i][j]=matrix[i][j];
        }
    }

    if(ensure_types_equal(mat,rows,cols)){
        //possible
        return mat;
    }else{
        //not possible, deallocate and return nullptr
        dealloc_matrix(mat,rows);
        return nullptr;
    }
}

void RF::promote_all_elements_into_field(RF* const* const matrix, int rows, int cols){
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(!matrix[i][j].is_field()){
                R one=matrix[i][j].promote_one();
                matrix[i][j]=RF{new Fraction{matrix[i][j],one}};
            }
        }
    }
}

void RF::dealloc_matrix(RF **matrix, int rows){
    for(int i=0;i<rows;i++){
        delete[] matrix[i];
    }
    delete[] matrix;
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