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

It is possible to test the parser in the console now. Build and run the current program and select the COMPUTE option. Do try to input long and complex statements (such as '3x/5+(2+4x+7x2+9x2-4i)/(2i+(4-7i)x4+6-5x)').

## Louis
* Math algorithms and base classes
* Console IO
* String parser for mathematical objects
* Use LATEX to create images for output
* Combine string parser with OCR

## Benson
* OCR recognition
* Combine string parser with OCR
* API key input in GUI

## Cora
* GUI code and design
* Load and display images of mathematical functions 
