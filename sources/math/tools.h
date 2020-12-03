#include "R.h"
#include <string>

long long gcd(long long, long long);

/**
 * Notice that the input must be a,b>0. Returns c,d>0 such that ac+bd=gcd.
*/
void xgcd(const int& a, const int& b, int& gcd, int& c, int& d);

R gcd (R a, R b);

RF gcd_fast (RF a, RF b);

/**
 * Checks if the prime is supported. Prime numbers supported by this program are the primes from 1 to 100
*/
bool is_supported_prime(const int& val);

/**
 * Prints the supported primes to console.
*/
void print_primes_to_console();


/**
 * Parser for an expression of R. Success = -1 if success. If there is an error, success = the position of the character where there is an error.
*/
void parse_expression(const std::string& input,std::string& err, R& val, int& success);

/**
 * Parser for an expression of R. Success = -1 if success. If there is an error, success = the position of the character where there is an error.
*/
void parse_expression_modulo(const std::string& input, const int& modulo, std::string& err, R& val, int& success);

void parse_expression_modulo(const std::string& input,std::string& err, R& val, int& success, const int& modulo);