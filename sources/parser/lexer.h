#ifndef LEXER_H_
#define LEXER_H_

#include <iostream>
#include <string>
#include <regex>
#include <utility>
#include "tokens.h"


// a simple lexer used to classify tokens from input
// and evaluate some immediate literals
class Lexer
{   
    typedef Token::TokType TokType;
    typedef Token::TokName TokName;

public:
    explicit Lexer(std::string input): input(std::move(input)) {}

    void reset_input(std::string input) { this->input = std::move(input); }

    Token* getNextToken(bool ignore_invalid_token=false);

private:
    Token* parseDelim();
    Token* parseOp();
    Token* parseNum();
    Token* parseId();

    static std::regex WHITESPACE_RE;
    static std::regex DELIM_RE;
    static std::regex OP_RE;
    static std::regex NUM_RE;
    static std::regex ID_RE;
    std::string input;
};

#endif /* LEXER_H_ */