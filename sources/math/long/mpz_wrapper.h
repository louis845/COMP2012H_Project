#include "gmp.h"
#include <string>

#ifndef MATH_LONG_MPZ_WRAPPER_H
#define MATH_LONG_MPZ_WRAPPER_H

class mpz_wrapper{
    mpz_t value;
public:
    mpz_wrapper(const long& l);
    mpz_wrapper(const std::string& s);
    ~mpz_wrapper();
    mpz_wrapper(const mpz_wrapper&);
    mpz_wrapper(mpz_wrapper&&);
    mpz_wrapper& operator=(const mpz_wrapper&);
    mpz_wrapper& operator=(mpz_wrapper&&);

    std::string get_str() const;

    mpz_wrapper operator+ (const mpz_wrapper& o) const;
    mpz_wrapper operator- (const mpz_wrapper& o) const;
    mpz_wrapper operator* (const mpz_wrapper& o) const;
    mpz_wrapper operator/ (const mpz_wrapper& o) const;
    mpz_wrapper operator% (const mpz_wrapper& o) const;

    void operator++();
    void operator--();
    mpz_wrapper operator-() const;

    bool operator== (const mpz_wrapper& o) const;
    bool operator!= (const mpz_wrapper& o) const;
    bool operator> (const mpz_wrapper& o) const;
    bool operator< (const mpz_wrapper& o) const;
    bool operator>= (const mpz_wrapper& o) const;
    bool operator<= (const mpz_wrapper& o) const;

    bool operator== (const long& i) const;
    bool operator!= (const long& i) const;
    bool operator> (const long& i) const;
    bool operator< (const long& i) const;
    bool operator>= (const long& i) const;
    bool operator<= (const long& i) const;

    friend mpz_wrapper abs(const mpz_wrapper& val);
    friend mpz_wrapper gcd(const mpz_wrapper& a, const mpz_wrapper& b);
    friend mpz_wrapper operator/ (long l,const mpz_wrapper& o);
};

mpz_wrapper gcd(const mpz_wrapper& a, const mpz_wrapper& b);

mpz_wrapper abs(const mpz_wrapper& val);

mpz_wrapper operator/ (long l,const mpz_wrapper& o);

#endif
