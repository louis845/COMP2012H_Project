#ifndef PARSER_H_
#define PARSER_H_

#include "expr_ast.h"
#include "lexer.h"
#include "tokens.h"
#include "utils/math_wrapper.h"
#include <string>
#include <unordered_map>
#include <unordered_set>


class Parser
{
    typedef Token::TokName TokName;
    typedef Token::TokType TokType;

public:
    Parser(const std::string& input);
    ~Parser();
    
    const Info& parse(int engine_type=0);      // 0 for auto-detection, 1 for R forced, 2 for Armadillo forced
    ExprAst* get_root();
    void print() const;
    std::string getAsciiMath() const { return root->genAsciiMath(); }
    void reset_input(const std::string& input);
    
private:
    int getTokPrecedence();
    void getNextToken();

    NumberExprAst* parseNum();
    ExprAst* parseParen();
    ExprAst* parseId();
    std::vector<ExprAst*> parseBracket();
    MatrixExprAst* parseMatrix();
    ExprAst* parsePrimary(bool inside_textbar);
    ExprAst* parseBinOpRhs(int min_precedence, ExprAst* lhs, bool inside_textbar);
    ExprAst* parseExpr(bool inside_textbar=false);

    void evalR();
    void eval();
    void printAst(ExprAst* root) const;
    void autoDetect(ExprAst* root);

    static std::unordered_map<TokName, int> bin_op_precedence;
    std::unordered_set<std::string> var_table;
    Lexer tokenizer;
    std::string input;
    Token* cur_tok{nullptr};
    ExprAst* root{nullptr};
    Info res;
};


#endif /* PARSER_H_ */
