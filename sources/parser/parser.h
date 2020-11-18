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
    ~Parser();
    
    bool parse();
    ExprAst* get_root();
    void print() const;
    void reset_input(const std::string& input);
    
private:
    int getTokPrecedence();
    void getNextToken();

    NumberExprAst* parseNum();
    ExprAst* parseParen();
    ExprAst* parseId();
    MatrixExprAst* parseBracket();
    MatrixExprAst* parseMatrix();
    ExprAst* parsePrimary(bool inside_textbar);
    ExprAst* parseBinOpRhs(int min_precedence, ExprAst* lhs, bool inside_textbar);
    ExprAst* parseExpr(bool inside_textbar=false);

    void printAst(ExprAst* root) const;

    static std::unordered_map<TokName, int> bin_op_precedence;
    Lexer tokenizer;
    std::string input;
    Token* cur_tok{nullptr};
    ExprAst* root{nullptr};
};


#endif /* PARSER_H_ */