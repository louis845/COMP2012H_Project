#ifndef MATH_R_H
#define MATH_R_H

#include "debug.h"
#include "math/Ring.h"
#include <string>
#include <iostream>

/**
 * Wrapper class for Rings. Use this class for computation (operators + - * / % etc).
*/
class R{
protected:
    const Ring* impl;
public:
    /**
     * ONE and ZERO. They are not necessarily unique, some other functions may create other ZeroElmt hence R with R.impl=ZeroElmt.
     * Notice the pointer is INTENDED to not be deallocated throughout the whole program execution. DO NOT use impl0 to create a new R,
     * it is already in ZERO! To do that, use the copy constructor on ZERO: R{R::ZERO}. 
    */
    static const ZeroElmt* const impl0;
    static const R ZERO;

    /**
     * Dynamically allocates and copies the array.
    */
    static R* array_copy(R* const& arr, int length);

    /**
     * Dynamically allocates and copies the array. The new array is zero until index==shift. 
     * The length of returned array will be length+shift. Notice that
     * array_copy(arr,len,0) is equivalent to array_copy(arr,len)
    */
    static R* array_copy(R* const& arr, int length, int shift);

    /**
     * Creates a ring wrapper to point to zero.
    */
    R();

    /**
     * Creates an ring wrapper, given a preallocated pointer to a ring implementation class. DO NOT
     * allow two ring wrappers (R) to contain the same pointer. Pass null pointer ONLY IF the R is used for
     * storing multivalued return functions (such as quotAndRemainder). The passed pointer has to point to a
     * new dynamically allocated value, and it will directly be used internally in R. So there is no need to
     * delete the given pointer.
    */
    R(const Ring*);

    /**
     * Destructor and copy/move constr/assignment, which handles the pointer accordingly.
    */
    virtual ~R();

    R(const R&);

    virtual R& operator= (const R&);

    R(R&&);
    
    virtual R& operator= (R&&);

    //Usual binary operations. Implemented by impl->addImpl etc.
    virtual R operator+ (const R&) const;

    virtual R operator- (const R&) const;

    virtual R operator* (const R&) const;

    virtual R operator/ (const R&) const;

    virtual R operator% (const R&) const;

    /**
     * Finds both the quotient and remainder of this/div.
    */
    virtual void quotAndRemainder(const R& div, R& quot,R& rem) const;

    /**
     * Comparsion operator for the Euclidean functions of the values. This DOES NOT function like the usual comparison operator for integer types!
     * For integers this compares the absolute value, and for polynomials this compares the degree, etc
    */
    virtual bool operator> (const R&) const;

    /**
     * Comparsion operator for the Euclidean functions of the values. This DOES NOT function like the usual comparison operator for integer types!
     * For integers this compares the absolute value, and for polynomials this compares the degree, etc
    */
    virtual bool operator< (const R&) const;

    /**
     * Comparsion operator for the Euclidean functions of the values. This DOES NOT function like the usual comparison operator for integer types!
     * For integers this compares the absolute value, and for polynomials this compares the degree, etc
    */
    virtual bool operator>= (const R&) const;

    /**
     * Comparsion operator for the Euclidean functions of the values. This DOES NOT function like the usual comparison operator for integer types!
     * For integers this compares the absolute value, and for polynomials this compares the degree, etc
    */
    virtual bool operator<= (const R&) const;

    /**
     * Comparsion operator for the Euclidean functions of the values. This DOES NOT function like the usual comparison operator for integer types!
     * For integers this compares the absolute value, and for polynomials this compares the degree, etc. Notice that -1 == 1 since their absolute
     * value are same. Use exactly_equals for an exact comparison (see exactly_equals).
    */
    virtual bool operator== (const R&) const;

    /**
     * Tests if the values are exactly equal. Note that in Double some tolerance may be needed.
    */
    virtual bool exactly_equals(const R&) const;

    virtual int euclidean_func_compare(const R&) const;

    bool is_zero() const;

    virtual R operator-() const;

    R invert() const;

    //Console and latex output. See Ring.h and Ring.cpp
    std::string to_string() const;
    
    std::string to_signed_string() const;

    std::string to_latex() const;
    
    std::string to_signed_latex() const;

    std::string to_leading_coeff() const;

    std::string to_coeff() const;

    std::string to_latex_leading_coeff() const;

    std::string to_latex_coeff() const;

    bool needs_bracket() const;

    bool needs_bracket_latex() const;

    const RingType& get_type_shallow() const;

    const NestedRingType& get_type() const;

    bool is_type_compatible(const R&) const;

    /**
     * Promotes other to match the type of this.
    */
    R promote(const R& other) const;

    /**
     * For internal use of Ring subclasses. Calls impl->promote(r)
    */
    const Ring* promote_exp(const Ring* const& r) const;

    /**
     * Returns the element with value 1, matching the type of this.
    */
    R promote_one() const;

    bool is_one() const;

    bool is_unit() const;

    /**
     * Splits the element into a product this*unit=morph, where unit is invertible.
    */
    void split(R& morph, R& unit) const;

    friend class RF;
};

/**
 * Fast R class, which does not do type checking/promotion on the operators/comparison function.
 * Make sure the type is exactly the same ( in the sense of get_type().deep_equals ) ,  and note that
 * SPECIAL_ZERO cannot be used for operations with other types (since it is not exactly same).
*/
class RF final: public R{
public:
/**
 * Usual constructors and destructors, doing the same thing as in R
*/
    /**
     * DANGER - the default constructor sets the value to zero, with type SPECIAL_ZERO. Set the value to other types before 
     * doing operations with other RFs. The default constructor is here for the initializing arrays (e.g. new RF[]).
    */
    RF();
    RF(const Ring* r);
    RF(const R&);
    RF(const RF&);
    RF(RF&&);
    RF& operator=(const R&) override;
    RF& operator=(R&&) override;
    RF& operator=(const RF&);
    RF& operator=(RF&&);
    //Note that the destructor can be simply inherited from R, since there are no additional members.

/**
 * Override operators/comparsion functions, do not perform type checking/promotion
*/
    virtual R operator+ (const R&) const override;

    virtual R operator- (const R&) const override;

    virtual R operator* (const R&) const override;

    virtual R operator/ (const R&) const override;

    virtual R operator% (const R&) const override;

    /**
     * Finds both the quotient and remainder of this/div.
    */
    void quotAndRemainder(const R& div, R& quot,R& rem) const override;

    /**
     * Comparsion operator for the Euclidean functions of the values. This DOES NOT function like the usual comparison operator for integer types!
     * For integers this compares the absolute value, and for polynomials this compares the degree, etc
    */
    virtual bool operator> (const R&) const override;

    /**
     * Comparsion operator for the Euclidean functions of the values. This DOES NOT function like the usual comparison operator for integer types!
     * For integers this compares the absolute value, and for polynomials this compares the degree, etc
    */
    virtual bool operator< (const R&) const override;

    /**
     * Comparsion operator for the Euclidean functions of the values. This DOES NOT function like the usual comparison operator for integer types!
     * For integers this compares the absolute value, and for polynomials this compares the degree, etc
    */
    virtual bool operator>= (const R&) const override;

    /**
     * Comparsion operator for the Euclidean functions of the values. This DOES NOT function like the usual comparison operator for integer types!
     * For integers this compares the absolute value, and for polynomials this compares the degree, etc
    */
    virtual bool operator<= (const R&) const override;

    /**
     * Comparsion operator for the Euclidean functions of the values. This DOES NOT function like the usual comparison operator for integer types!
     * For integers this compares the absolute value, and for polynomials this compares the degree, etc. Notice that -1 == 1 since their absolute
     * value are same. Use exactly_equals for an exact comparison (see exactly_equals).
    */
    virtual bool operator== (const R&) const override;

    /**
     * Tests if the values are exactly equal. Note that in Double some tolerance may be needed.
    */
    virtual bool exactly_equals(const R&) const override;

    virtual int euclidean_func_compare(const R&) const override;


    virtual R operator- () const override;

    static RF* array_copy(const RF* const& arr, int len, int shift);
};

/**
 * Overloading to allow cout to output the contents of R. The output is R.to_string(). 
*/
std::ostream& operator<< (std::ostream&, const R&);

std::ostream& operator<< (std::ostream&, const RF&);

#endif