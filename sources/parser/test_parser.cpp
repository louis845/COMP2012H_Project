#include <iostream>
#include <string>
#include "parser.h"
#include "lexer.h"
#include "tokens.h"
using namespace std;

// type = 0 for parsing only, 1 for evaluating using R class, 2 for using Armadillo
void test_parser(const std::string& input, const std::string& name, int type = 0)
{
    Parser parser(input);
    cout << boolalpha << name << ": " << parser.parse() << endl << endl;
    cout << "raw input: " << endl << input << endl << endl;
    cout << "infix notation: " << endl;
    parser.print();
    cout << endl << endl << "AsciiMath output: " << endl << parser.getAsciiMath() << endl << endl;
    
    if (type == 1)  
    {   
        cout << "evaluation using R: " << endl; 
        parser.evalR();  
    }
    else if (type == 2)
    {
        cout << "evaluation using Armadillo: " << endl;
        parser.eval();
    }
    cout << endl << endl << string(80, '-') << endl << endl; 
}


int main()
{
    string test_1{"1 + 2 * 3 - 4 % 5 / 6 + 7 ^ 8"};

    string test_2{"4xy + e^x + sqrt(x) // 3y - 3.44 % 10e23"};  // note that sqrt(x) // 3y should be parse as sqrt(x) / 3 * y

    string test_3{"3x/5+(2+4x+7x2+9x2-4i)/(2i+(4-7i)x4+6-5x)"};

    string poly{"x^3 - x^0.5 / x^(pi+i) * 4x(x-1)(x+3x)^(10x)"};

    string matrix{"det([[3, 4], [pi, e]] xx [[ipi, sqrt(x + x^2 / 1)]])"};

    string func{"sin(cos(tan(exp(ln(min(3, 4, 5, 6) + x) - y) * pi) / 42) ** i)"};

    string greek{"sin(alpha) - cos(-beta) / sqrt(omega) * root(phi, varphi)"};

    test_parser(test_1, "test 1");
    test_parser(test_2, "test 2");
    test_parser(test_3, "test 3");
    test_parser(poly, "poly");
    test_parser(matrix, "matrix");
    test_parser(func, "func");
    test_parser(greek, "greek");

    // ------------------------- test evaluator for R below ------------------------

    string eval_test_1{"x^2 + x / (x - 1) * 4x"};

    string eval_test_2{"3 * 3i - 4 / 5 + 6 ^ 3"};

    string eval_test_3{"(-2 (2601 + 5598 x - 61921 x^2 - 10126 x^3 + 6848 x^4 + 1056 x^5)) / (x + 3)"};

    test_parser(eval_test_1, "R test 1", 1);
    test_parser(eval_test_2, "R test 2", 1);
    test_parser(eval_test_3, "R test 4", 1);


    // ------------------------- test Armadillo below ------------------------------

    string arma_test_1{"3.2 + 23i * pi / 1e-6i - 0.5"};

    string arma_test_2{"sin(2pi) + cos(-2pi) - ln(exp(3)) + pi^e - sqrt(4) - root(9, 2) + |-1| - |3 - 2i|"};

    string arma_test_3{"det[[3, 4], [5, 6]] + trace[[1, -1], [-pi, 3/sqrt(3)]]"};

    string arma_test_4{"[[sqrt(3), arcsin(4)], [ln4, exp(-4)]] + [[1, 2], [2, 3]] * [[-i, 2 - 3i], [0, 7]]"};

    string arma_test_5{"Col[[4i, 1-8i], [e, i^3]] + Ran[[0.223223090, 1e-7], [1E10, -i-1]] + norm(3 - 4i)"};

    string arma_test_6{"[[0, 1], [1, 0]]^20 - [[i, -i], [-1, 1]]^7"};

    test_parser(arma_test_1, "arma test 1", 2);
    test_parser(arma_test_2, "arma test 2", 2);
    test_parser(arma_test_3, "arma test 3", 2);
    test_parser(arma_test_4, "arma test 4", 2);
    test_parser(arma_test_5, "arma test 5", 2);
    test_parser(arma_test_6, "arma test 6", 2);

    cin.get(); cin.get();
}