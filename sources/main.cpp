/**
 * Temporary main. Includes console for debugging.
 * 
*/
#include <iostream>
#include <string>
#include <algorithm>
#include "console.h"
#include "math/R.h"
#include "math/tools.h"
#include "math/long/LongComplex.h"
#include "math/fraction/Fraction.h"
#include "math/polynomial/Polynomial.h"
#include "math/finite_field/ModField.h"
#include "math/linear/LinearOperations.h"
#include "debug_stress_test.h"

using namespace std;

int main(){
    R a=R::parse_string("3.0"); //For polynomials the variable must be t
    R b=R::parse_string("4");
    cout << a.is_type_compatible(b) << "\n"; //False
    cout << (a+b) << "\n";
    //console_main_loop();
    return 0;
}
