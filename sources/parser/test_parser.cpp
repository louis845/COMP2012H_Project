#include <iostream>
#include <string>
#include <iomanip>
#include "parser.h"
#include "lexer.h"
#include "tokens.h"
using namespace std;

// type = 0 for parsing only, 1 for evaluating using R class, 2 for using Armadillo
void test_parser(const std::string& input, const std::string& name, int type = 0)
{
    Parser parser(input);
    const Info& res = parser.parse(type);

    if (res.success)
    {
        cout << boolalpha << name << "  status: " << res.success << "  engine_used: " << res.engine_used << endl << endl;
        cout << "raw input: " << endl << input << endl << endl;
        cout << "infix notation: " << endl;
        parser.print();
        cout << endl << endl << "AsciiMath output: " << endl << res.interpreted_input << endl << endl;
        cout << "computation result: " << res.eval_result << endl << endl;
        cout << "matrix num: " << res.mat_size.size() << endl << endl;

        if (type == 3)
        {
            cout << "mat: " << endl;
            for (size_t i = 0; i < res.mat_size[0].first; ++i)
            {
                for (size_t j = 0; j < res.mat_size[0].second; ++j)
                    cout << setw(14) << res.parsed_mat[0].second[i][j].to_latex() << " ";
                cout << endl;
            }
        }
    }
    else
    {
        cout << boolalpha << name << "  status: " << res.success << "  engine_used: " << res.engine_used << endl << endl;
        cout << "raw input: " << endl << input << endl << endl;
        cout << "infix notation: " << endl;
        parser.print();
        cout << endl << endl << "AsciiMath output: " << endl << res.interpreted_input << endl << endl;
        cout << res.err_msg << endl << endl;
    }

    cout << endl << endl << string(80, '-') << endl << endl;
}


int main()
{
    int engine = 0;

    /*
    Parser p{""};
    p.reset_input("[[3,4,4+t][5,6]]");
    p.parse();
    p.reset_input("[[3,4,4+t,],[5,6]]");
    p.parse();
     */

    Parser p("1 + 2");
    cout << "var name ok? : " << boolalpha << p.checkVarNameValid("a_1_2323 ") << endl;


    string linear{R"(4*5t x_alpha + (5+2i)/t y_ - 6z'_1 + 2 = 0 \\ (t^2 - 6) x_alpha - 4y_ = 3 + z'_1 \\ 5t^4z'_1 = 3 + 5/6 y_)"};

    string debug_0{"solve[[0, 0, 1], [0, 0, 2]] _ "};

    string debug_1{"sin({3, 4}, [5, 6]) * root(2, 3) + (3) + ([1, 2], [3, 4])"};

    string debug_2{"[[3, 4], [5, 6]] + ([3, 4], [5, 6]) + sin(([3, 4], [5, 6])) + cos([[3, 4], [5, 6]]) + tan[[3, 4], [5, 6]]"};

    string debug_3{"inv(inv(inv[[1, 0], [0, 2]]))"};

    string test_1{"1 + 2 * 3 - 4 % 5 / 6 + 7 ^ 8"};

    string test_2{"4xy + e^x + sqrt(x) // 3y - 3.44 % 10e23"};  // note that sqrt(x) // 3y should be parse as sqrt(x) / 3 * y

    string test_3{"3x/5+(2+4x+7x2+9x2-4i)/(2i+(4-7i)x4+6-5x)"};

    string poly{"x^3 - x^0.5 / x^(pi+i) * 4x(x-1)(x+3x)^(10x)"};

    string matrix{"det([[3, 4], [pi, e]] xx [[ipi, sqrt(x + x^2 / 1)], [sin2, exp(-2)]])"};

    string func{"sin(cos(tan(exp(ln(min(3, 4, 5, 6) + x) - y) * pi) / 42) ** i)"};

    string greek{"sin(alpha) - cos(-beta) / sqrt(omega) * root(phi, varphi)"};

    // test_parser(linear, "linear system: ", 3);

    test_parser(debug_0, "debug 0", 1);
    test_parser(debug_1, "debug 1", engine);
    test_parser(debug_2, "debug 2", engine);
    test_parser(debug_3, "debug 3", engine);

    test_parser(test_1, "test 1", engine);
    test_parser(test_2, "test 2", engine);
    test_parser(test_3, "test 3", engine);
    test_parser(poly, "poly", engine);
    test_parser(matrix, "matrix", engine);
    test_parser(func, "func", engine);
    test_parser(greek, "greek", engine);

    // ------------------------- test evaluator for R below ------------------------

    string eval_test_1{"x^2 + x / (x - 1) * 4x"};

    string eval_test_2{"3 * 3i - 4 / 5 + 6 ^ 3"};

    string eval_test_3{"(-2 (2601 + 5598 x - 61921 x^2 - 10126 x^3 + 6848 x^4 + 1056 x^5)) / (x + 3)"};

    string r_test_4{"orth[[3, 4], [5, 6]] + rref[[3, 4], [5, 6]]"};

    string r_test_5{"charpoly[[3, 4], [5, 6]] + inv[[3, 4], [5, 6]]"};

    string r_test_6{"det(inv(inv(inv([[1, 0], [0, 2]]))))"};

    test_parser(eval_test_1, "R test 1", engine);
    test_parser(eval_test_2, "R test 2", engine);
    test_parser(eval_test_3, "R test 3", engine);
    test_parser(r_test_4, "R test 4", engine);
    test_parser(r_test_5, "R test 5", engine);
    test_parser(r_test_6, "R test 6", 1);


    // ------------------------- test Armadillo below ------------------------------

    string arma_test_1{"3.2 + 23i * pi / 1e-6i - 0.5"};

    string arma_test_2{"sin(2pi) + cos(-2pi) - ln(exp(3)) + pi^e - sqrt(4) - root(9, 2) + |-1| - |3 - 2i|"};

    string arma_test_3{"det[[3, 4], [5, 6]] + trace[[1, -1], [-pi, 3/sqrt(3)]]"};

    string arma_test_4{"[[sqrt(3), arcsin(4)], [ln4, exp(-4)]] + [[1, 2], [2, 3]] * [[-i, 2 - 3i], [0, 7]]"};

    string arma_test_5{"Col[[4i, 1-8i, 56], [e, i^3, cos7], [sqrt(6e), -23, 2^0.5]]"};

    string arma_test_6{"[[0, 1], [1, 0]]^20 - [[i, -i], [-1, 1]]^7"};

    string arma_test_7{"inv([[1, 2], [3, 4]]) + pinv([[1e2, 2i+3], [3e*4, 4pi/5]])"};

    string arma_test_8{"solve([[1, 2i, 6], [3e, 4, 9pi]])"};

    string decomp_1{"schur[[3, 4], [5, 6]]"};

    string decomp_2{"qr[[3, 4], [5, 6]]"};

    string decomp_3{"svd[[3, 4], [5, 6]]"};

    string decomp_4{"eigen[[3, 4], [5, 6]]"};

    test_parser(arma_test_1, "arma test 1", engine);
    test_parser(arma_test_2, "arma test 2", engine);
    test_parser(arma_test_3, "arma test 3", engine);
    test_parser(arma_test_4, "arma test 4", engine);
    test_parser(arma_test_5, "arma test 5", engine);
    test_parser(arma_test_6, "arma test 6", engine);
    test_parser(arma_test_7, "arma test 7", engine);
    test_parser(arma_test_8, "arma test 8", engine);

    test_parser(decomp_1, "decomposition test 1", engine);
    test_parser(decomp_2, "decomposition test 2", engine);
    test_parser(decomp_3, "decomposition test 3", engine);
    test_parser(decomp_4, "decomposition test 4", engine);


    stringstream ss;
    ss.precision(4);
    ss.setf(std::ios::fixed, std::ios::floatfield);

    ss << arma::cx_double(4.4, 3.4);
    cout << ss.str();

    cin.get(); cin.get();
}