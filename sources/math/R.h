#ifndef MATH_R_H
#define MATH_R_H

#include "debug.h"
#include "math/Ring.h"
#include <string>
#include <iostream>

/**
 * Wrapper class for Rings. Use this class for computation (operators + - * / % etc).
*/
class R final{
private:
    const Ring* impl;
public:

    /**
     * ONE and ZERO. They are not necessarily unique, some other functions may create other ZeroElmt hence R with R.impl=ZeroElmt.
    */
    static const ZeroElmt* const impl0;
    static const R ZERO;

    /**
     * Creates an ring wrapper, given a preallocated pointer to a ring implementation class. DO NOT
     * allow two ring wrappers (R) to contain the same pointer. Pass null pointer ONLY IF the R is used for
     * storing multivalued return functions (such as quotAndRemainder)
    */
    R(const Ring*);


    /**
     * Destructor and copy/move constr/assignment, which handles the pointer accordingly.
    */
    ~R();

    R(const R&);

    R& operator= (const R&);

    R(R&&);
    
    R& operator= (R&&);

    //Usual binary operations. Implemented by impl->addImpl etc.
    R operator+ (const R&) const;

    R operator- (const R&) const;

    R operator* (const R&) const;

    R operator/ (const R&) const;

    R operator% (const R&) const;

    /**
     * Comparsion operator for the Euclidean functions of the values. This DOES NOT function like the usual comparison operator for integer types!
     * For integers this compares the absolute value, and for polynomials this compares the degree, etc
    */
    bool operator> (const R&) const;

    /**
     * Comparsion operator for the Euclidean functions of the values. This DOES NOT function like the usual comparison operator for integer types!
     * For integers this compares the absolute value, and for polynomials this compares the degree, etc
    */
    bool operator< (const R&) const;

    /**
     * Comparsion operator for the Euclidean functions of the values. This DOES NOT function like the usual comparison operator for integer types!
     * For integers this compares the absolute value, and for polynomials this compares the degree, etc
    */
    bool operator>= (const R&) const;

    /**
     * Comparsion operator for the Euclidean functions of the values. This DOES NOT function like the usual comparison operator for integer types!
     * For integers this compares the absolute value, and for polynomials this compares the degree, etc
    */
    bool operator<= (const R&) const;

    /**
     * Comparsion operator for the Euclidean functions of the values. This DOES NOT function like the usual comparison operator for integer types!
     * For integers this compares the absolute value, and for polynomials this compares the degree, etc. Notice that -1 == 1 since their absolute
     * value are same. Use exactly_equals for an exact comparison (see exactly_equals).
    */
    bool operator== (const R&) const;

    /**
     * Tests if the values are exactly equal. Note that in Double some tolerance may be needed.
    */
    bool exactly_equals(const R&) const;

    int euclidean_func_compare(const R&) const;

    bool is_zero() const;

    R operator-() const;

    //Console and latex output. See Ring.h and Ring.cpp
    std::string to_string() const;
    
    std::string to_signed_string() const;

    std::string to_latex() const;
    
    std::string to_signed_latex() const;

    const RingType& get_type_shallow() const;

    const NestedRingType& get_type() const;

    bool is_type_compatible(const R&) const;

    /**
     * Promotes other to match the type of this.
    */
    R promote(const R& other) const;

    bool is_one() const;

    bool is_unit() const;

    /**
     * Splits the element into a product this*unit=morph, where unit is invertible.
    */
    void split(R& morph, R& unit) const;

    /**
     * Finds both the quotient and remainder of this/div.
    */
    void quotAndRemainder(const R& div, R& quot,R& rem) const;
};

/**
 * Overloading to allow cout to output the contents of R. The output is R.to_string(). 
*/
std::ostream& operator<< (std::ostream&, const R&);

#endif