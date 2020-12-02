#ifndef LEXER_H_
#define LEXER_H_

#include <iostream>
#include <string>
#include <regex>
#include "tokens.h"


// A simple lexer used to tokenize the AsciiMath input
// It uses regular expressions to parse the input for simplicity
// EXAMPLE:
//          Lexer tokenizer("x + y = z");
//          Token* cur_token = tokenizer.getNextToken();
//          tokenizer.reset_input("[[1, 0], [0, 1]]");
class Lexer
{   
    typedef Token::TokType TokType;
    typedef Token::TokName TokName;

public:
    explicit Lexer(std::string input): input(std::move(input)) {}

    std::string get_input() const { return input; }
    void reset_input(std::string input) { this->input = std::move(input); }

    Token* getNextToken(bool ignore_invalid_token=false);

private:
    Token* parseDelim();
    Token* parseOp();
    Token* parseNum();
    Token* parseId();

    // The regex patterns for corresponding tokens
    static std::regex WHITESPACE_RE;
    static std::regex DELIM_RE;
    static std::regex OP_RE;
    static std::regex NUM_RE;
    static std::regex ID_RE;
    std::string input;
};

#endif /* LEXER_H_ */