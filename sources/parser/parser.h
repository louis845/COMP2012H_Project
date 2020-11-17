#ifndef PARSER_H_
#define PARSER_H_

#include "expr_ast.h"
#include "lexer.h"
#include "tokens.h"
#include <string>
#include <unordered_map>


class Parser
{
    typedef Token::TokName TokName;
    typedef Token::TokType TokType;

public:
    Parser(const std::string& input);
    
    bool parse();
    ExprAst* get_root();
    
private:
    int getTokPrecedence();
    Token* getNextToken();

    NumberExprAst* parseNum();
    ExprAst* parseParen();
    ExprAst* parseId();
    MatrixExprAst* parseBracket();
    MatrixExprAst* parseMatrix();
    ExprAst* parsePrimary();
    ExprAst* parseBinOpRhs();
    ExprAst* parseExpr();

    static std::unordered_map<TokName, int> bin_op_precedence;
    std::string input;
    Token* cur_tok;
    ExprAst* root{nullptr};
};


#endif /* PARSER_H_ */