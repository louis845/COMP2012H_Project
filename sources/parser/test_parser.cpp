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

    string matrix{"det([[3, 4], [pi, e]] xx [[ipi, sqrt(x + x^2 / 1)]])"};        // TODO: add regex for -i -I -e -pi

    string func{"sin(cos(tan(exp(ln(min(3, 4, 5, 6) + x) - y) * pi) / 42) ** i)"};

    Parser parser(test_1);
    cout << boolalpha << "test 1: " << parser.parse() << endl;
    parser.print();
    cout << endl;

    parser.reset_input(test_2);
    cout << "test 2: " << parser.parse() << endl;
    parser.print();
    cout << endl;

    parser.reset_input(test_3);
    cout << "test 3: " << parser.parse() << endl;
    parser.print();
    cout << endl;

    parser.reset_input(poly);
    cout << "poly: " << parser.parse() << endl;
    parser.print();
    cout << endl;

    parser.reset_input(matrix);
    cout << "matrix : " << parser.parse() << endl;
    parser.print();
    cout << endl;

    parser.reset_input(func);
    cout << "func : " << parser.parse() << endl;
    parser.print();
    cout << endl;

    cin.get(); cin.get();
}