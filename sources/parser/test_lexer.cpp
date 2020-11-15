#include "lexer.h"
#include <string>
#include <iostream>
using namespace std;

int main() 
{
    string input;
    // getline(cin, input);
    input = "sin cos det[[3.3432/4e34, e^pi], [.344 + 4i // 5.34400 * 006 % 34.7], [sqrt(x^2)]] cos ";
    Lexer tokenizer(input);
    Token* cur;

    cout << "valid input: " << endl;
    while ((cur = tokenizer.getNextToken()) != nullptr)
    {
        cout << "type: " << static_cast<int>(cur->get_type()) << " name: " 
             << static_cast<int>(cur->get_name());
        cout << " raw: " << cur->get_raw_value() << " num: " << cur->get_num_value() << endl;
        delete cur;
    }

    input = "sinc cos deet[[3.3432/4e34, e^pi]&, [.344 + 4i // $5.344e0~.0 * 006 % 34.7], [#sqrt(x^2)]] cos ";
    tokenizer.reset_input(input);
    cout << "invalid input: " << endl;
    while ((cur = tokenizer.getNextToken()) != nullptr)
    {
        cout << "type: " << static_cast<int>(cur->get_type()) << " name: " 
             << static_cast<int>(cur->get_name());
        cout << " raw: " << cur->get_raw_value() << " num: " << cur->get_num_value() << endl;
        if (cur->get_type() == Token::TokType::ERR)
        {
            delete cur;
            break;
        }
        delete cur;
    }

    input = "sinc cos deet[[3.3432/4e34, e^pi]&, [.344 + 4i // $5.344e0~.0 * 006 % 34.7], [#sqrt(x^2)]] cos ";
    tokenizer.reset_input(input);
    cout << "invalid input ignored: " << endl;
    while ((cur = tokenizer.getNextToken(true)) != nullptr)
    {
        cout << "type: " << static_cast<int>(cur->get_type()) << " name: " 
             << static_cast<int>(cur->get_name());
        cout << " raw: " << cur->get_raw_value() << " num: " << cur->get_num_value() << endl;
        if (cur->get_type() == Token::TokType::ERR)
        {
            delete cur;
            break;
        }
        delete cur;
    }


    cin.get(); cin.get();
}