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
     * allow two ring wrappers (R) to contain the same pointer.
    */
    R(const Ring*);


    /**
     * Destructor and copy/move constr/assignment, which handles the pointer accordingly.
    */
    ~R();

    R(const R&);

    R& operator= (const R&);

    R(const R&&);
    
    R& operator= (const R&&);

    //Usual binary operations. Implemented by impl->addImpl etc.
    R operator+ (const R&) const;

    R operator- (const R&) const;

    R operator* (const R&) const;

    R operator/ (const R&) const;

    R operator% (const R&) const;

    bool operator== (const R&) const;

    int euclideanFunc() const;

    //Console and latex output. See Ring.h and Ring.cpp
    std::string to_string() const;
    
    std::string to_signed_string() const;

    std::string to_latex() const;
    
    std::string to_signed_latex() const;
};

/**
 * Overloading to allow cout to output the contents of R. The output is R.to_string(). 
*/
std::ostream& operator<< (std::ostream&, const R&);

#endif