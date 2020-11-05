#ifndef MATH_R_H
#define MATH_R_H

#include "debug.h"
#include "math/Ring.h"
#include <string>

/**
 * Wrapper class for Rings. Use this class for computation (operators + - * / % etc).
*/
class R final{
private:
    const Ring* impl;
public:
    R(Ring*);

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

    //Console and latex output. See Ring.h and Ring.cpp
    std::string to_string() const;
    
    std::string to_signed_string() const;

    std::string to_latex() const;
    
    std::string to_signed_latex() const;
};

#endif