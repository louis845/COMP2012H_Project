# COMP2012H_Project TODO

This list is not exhaustive, we can talk about it if there are new ideas or alternative suggestions.

## TODO-Everyone
* The whole process of GUI selection with parser into mathematical objects.
<pre>
/**
 * Parser for an expression of R. Success = -1 if success. If there is an error, success = the position of the character where there is an error.
*/
void parse_expression(const std::string& input,std::string& err, R& val, int& success); // inside math/tools.h
</pre>

String parser. How to translate from selection to image, and then from image to Mathpix string, and then string to math object?

It is possible to test the parser in the console now. Build and run the current program and select the COMPUTE option. Do try to input long and complex statements (such as '3x/5+(2+4x+7x2+9x2-4i)/(2i+(4-7i)x4+6-5x)'). Example statements:
<pre>
3+5/7-5+2i
3t+2t2-6t3
(t3/4+6t)(t-1)+(t2-1)/(t+1)
3*5*6(t)+(2+4i)/(3+6i)+100t3/(t2+2t+1)
(x5+x2-3)/(x2-1)+6x+5/(2t2-7)
(2+4i)x+(5-6i)+(7+(8/2)i)x2
</pre>
Notice that the 'variable' used for the polynomial does not matter.
## How the console string parser works (already implemented)
* Test with running console program -> type COMPUTE
* Split expression into 'atoms' e.g. (3-2i)4+6i+5t2 will be split into 3,2i,4,6i,5t2
* Parse atom according to the rule: <number part>[polynomial variable char][positive integer degree]
* <> is mandatory, while [] is optional (since not all atoms are polynomials). 

## Possible adaptation for OCR
* Image -> Ascii Math (string) -> Parser input (string) -> Math object (R class)
* Possible implementation: Omit all other characters not suiting our parser, then prompt user if it is correct
* Example 1: In ascii math 3x^(20)+4/x-2+3i -> 3x20+4/x-2+3i
* Notice the bracket () and ^ can be dropped since the parser directly treats the number after a variable as the power.
* Example 2: 3x^(11)(4x+3+5ix^2) -> 3x11(4x+3+5ix2)
* Notice the second bracket cannot be dropped due to it being needed to group the additions together.
* Example 3: nnn_(i=1)^10 A_i3=5+4=7 -> i1+Ai35+4=7
* Obviously after transformation the formula means totally different from the original, but these math concepts/operations are not supported anyway, so we drop it.
* Either give error (cannot scan) or try to parse and let user decide in this case.

### Drop brackets after and inside exponents (possible method)
* As stated, t^(10) -> t10, and t^(10)(5)-> t10(5).
* Drop the bracket (...) into ... if and only if (...) immediately follows '^' i.e. ^(...) -> (...), but <other>(...) -> <other>(...)
* It is best to enable t^(10+3) -> t13, but possible t^(10+3) -> t10+3, and then let user decide and edit manually.
* If possible translate all brackets in exponents t^(<integer expression>) -> t<evaluated integer expression>
* No need to evaluate non-constant expression, t^(3x) -> t3x, not supported anyway.

### Matrix OCR
* Finish expression OCR first (above)
* Possible to get spatial position? Sort by spatial position
1. Determine num rows, cols, make sure it is square
2. Parse expression, and lift types so it is compatible (R type), I have not yet created documentation
3. Pass arguments into dynamically allocated 2D array (RF**, fast class).

# Louis
* Math algorithms and base classes
* Console IO
* String parser for mathematical objects
* Use LATEX to create images for output
* Combine string parser with OCR

# Benson
* OCR recognition
* Combine string parser with OCR
* API key input in GUI

# Cora
* GUI code and design
* Load and display images of mathematical functions 
