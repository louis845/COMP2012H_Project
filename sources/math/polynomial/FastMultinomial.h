/**
 * Fast multinomial class, for resolving linear equations. The degree of the variables is at most degree 1.
 * Stores at most 26 variables (should be enough).
*/

#ifndef MATH_POLYNOMIAL_FAST_MULTINOMIAL_H
#define MATH_POLYNOMIAL_FAST_MULTINOMIAL_H

#include "math/R.h"
class FastMultinomial{
public:
    FastMultinomial();
    ~FastMultinomial();
    FastMultinomial(const FastMultinomial&) = delete;
    FastMultinomial(FastMultinomial&&) = delete;
    FastMultinomial& operator=(const FastMultinomial&) = delete;
    FastMultinomial& operator=(FastMultinomial&&) = delete;

    /**
     * Operations to change the value of the FastMultinomial. If there is a type compatibility error,
     * or the degree of the multinomial exceeds 1, returns false, and nothing is changed internally.
     * 
     * If the operation succeeds, returns true.
     * 
     * There is no need to do type promotion or type checking when using this class, just check the bool
     * return values to see if the operation is successful.
    */

    //Adds by a constant.
    bool add_to_self(const R& value);
    //Multiplies by a constant.
    bool multiply_to_self(const R& value);
    //Divides by a constant
    bool divide_to_self(const R& value);

    //Adds by a constant times the variable. For example, value=(3t^2+2), and variable=3, then
    //adds by (3t^2+2)x_3. Make sure that 1 <= variable <=26
    bool add_to_self(const R& value, int variable);

    //Multiplies by a constant times the variable. See above.
    bool multiply_to_self(const R& value, int variable);

    bool add_to_self(const FastMultinomial& other);

    bool multiply_to_self(const FastMultinomial& other);

    bool subtract_from_self(const FastMultinomial& to_subtract);

    const R& get_coefficient(int variable);

    bool is_non_zero_coefficient(int variable);

    const R& get_constant();
};
#endif