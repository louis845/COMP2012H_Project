#include "R.h"
#include <string>

long long gcd(long long, long long);

R gcd (R a, R b);

RF gcd_fast (RF a, RF b);

/**
 * The largest 'atomic formula' would be of type FRACTION POLYNOMIAL FRACTION COMPLEXIFY LONG or FRACTION POLYNOMIAL COMPLEXIFY DOUBLE
*/
class MathFormulaTree{
private:
    std::string formula;
    MathFormulaTree *left;
    MathFormulaTree *right;
    char oper; // plus minus mult etc..


    MathFormulaTree(std::string formula, char oper);
    ~MathFormulaTree();
    MathFormulaTree(const MathFormulaTree&)=delete;
    MathFormulaTree(MathFormulaTree&&)=delete;
    MathFormulaTree& operator=(const MathFormulaTree&)=delete;
    MathFormulaTree& operator=(MathFormulaTree&&)=delete;
};

/**
 * Basic parser for a single R value. The formula should be at most of the form:
 * FRACTION POLYNOMIAL FRACTION COMPLEXIFY LONG or FRACTION POLYNOMIAL COMPLEXIFY DOUBLE
*/
void parse_basic(std::string input, std::string& err_msg, R& val, bool& success);