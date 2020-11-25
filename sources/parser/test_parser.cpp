#include <bits/stdc++.h>
#include "parser.h"
#include "lexer.h"
#include "tokens.h"
using namespace std;

int main()
{
    string test_1{"1 + 2 * 3 - 4 % 5 / 6 + 7 ^ 8"};

    string test_2{"4xy + e^x + sqrt(x) // 3y - 3.44 % 10e23"};  // note that sqrt(x) // 3y should be parse as sqrt(x) / 3 * y

    string test_3{"3x/5+(2+4x+7x2+9x2-4i)/(2i+(4-7i)x4+6-5x)"};

    string poly{"x^3 - x^0.5 / x^(pi+i) * 4x(x-1)(x+3x)^(10x)"};

    string matrix{"det([[3, 4], [pi, e]] xx [[ipi, sqrt(x + x^2 / 1)]])"};

    string func{"sin(cos(tan(exp(ln(min(3, 4, 5, 6) + x) - y) * pi) / 42) ** i)"};

    string greek{"sin(alpha) - cos(-beta) / sqrt(omega) * root(phi, varphi)"};

    Parser parser(test_1);
    cout << boolalpha << "test 1: " << parser.parse() << endl;
    parser.print();
    cout << endl << endl;

    parser.reset_input(test_2);
    cout << "test 2: " << parser.parse() << endl;
    parser.print();
    cout << endl << endl;

    parser.reset_input(test_3);
    cout << "test 3: " << parser.parse() << endl;
    parser.print();
    cout << endl << endl;

    parser.reset_input(poly);
    cout << "poly: " << parser.parse() << endl;
    parser.print();
    cout << endl << endl;

    parser.reset_input(matrix);
    cout << "matrix: " << parser.parse() << endl;
    parser.print();
    cout << endl << endl;

    parser.reset_input(func);
    cout << "func: " << parser.parse() << endl;
    parser.print();
    cout << endl << endl;

    parser.reset_input(greek);
    cout << "greek: " << parser.parse() << endl;
    parser.print();
    cout << endl << endl;

    // ------------------------- test evaluator for R below ------------------------

    string eval_test_1{"x^2 + x / (x - 1) * 4x"};

    string eval_test_2{"3 * 3i - 4 / 5 + 6 ^ 3"};

    string eval_test_3{"(-2 (2601 + 5598 x - 61921 x^2 - 10126 x^3 + 6848 x^4 + 1056 x^5)) / (x + 3)"};

    parser.reset_input(eval_test_1);
    cout << "eval_test_1: " << parser.parse() << endl;
    parser.print(); cout << endl << endl;
    parser.evalR();  cout << endl << endl;

    parser.reset_input(eval_test_2);
    cout << "eval_test_2: " << parser.parse() << endl;
    parser.print(); cout << endl << endl;
    parser.evalR();  cout << endl << endl;

    parser.reset_input(eval_test_3);
    cout << "eval_test_3: " << parser.parse() << endl;
    parser.print(); cout << endl << endl;
    parser.evalR();  cout << endl << endl;


    // ------------------------- test Armadillo below ------------------------------

    string arma_test_1{"3.2 + 23i * pi / 1e-6i - 0.5"};

    string arma_test_2{"sin(2pi) + cos(-2pi) - ln(exp(3)) + pi^e - sqrt(4) - root(9, 2) + |-1| - |3 - 2i|"};

    string arma_test_3{"det[[3, 4], [5, 6]] + trace[[1, -1], [-pi, 3/sqrt(3)]]"};

    string arma_test_4{"[[sqrt(3), arcsin(4)], [ln4, exp(-4)]] + [[1, 2], [2, 3]] * [[-i, 2 - 3i], [0, 7]]"};

    string arma_test_5{"Col[[4i, 1-8i], [e, i^3]] + Ran[[0.223223090, 1e-7], [1E10, -i-1]] + norm(3 - 4i)"};

    string arma_test_6{"[[0, 1], [1, 0]]^20 - [[i, -i], [-1, 1]]^7"};

    parser.reset_input(arma_test_1);
    cout << "arma_test_1: " << parser.parse() << endl;
    parser.print(); cout << endl << endl;
    parser.eval();  cout << endl << endl;

    parser.reset_input(arma_test_2);
    cout << "arma_test_2: " << parser.parse() << endl;
    parser.print(); cout << endl << endl;
    parser.eval();  cout << endl << endl;

    parser.reset_input(arma_test_3);
    cout << "arma_test_3: " << parser.parse() << endl;
    parser.print(); cout << endl << endl;
    parser.eval();  cout << endl << endl;

    parser.reset_input(arma_test_4);
    cout << "arma_test_4: " << parser.parse() << endl;
    parser.print(); cout << endl << endl;
    parser.eval();  cout << endl << endl;

    parser.reset_input(arma_test_5);
    cout << "arma_test_5: " << parser.parse() << endl;
    parser.print(); cout << endl << endl;
    parser.eval();  cout << endl << endl;

    parser.reset_input(arma_test_6);
    cout << "arma_test_6: " << parser.parse() << endl;
    parser.print(); cout << endl << endl;
    parser.eval();  cout << endl << endl;

    cin.get(); cin.get();
}