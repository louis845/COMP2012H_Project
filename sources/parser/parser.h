#ifndef PARSER_H_
#define PARSER_H_

#include "expr_ast.h"
#include "lexer.h"
#include "tokens.h"
#include "utils/math_wrapper.h"
#include <string>
#include <map>
#include <unordered_map>


// Core part of the parser function, it can parse an AsciiMath string and
// construct an anstrct syntax tree. The output is an Info class, containing
// the evaluation result of the input string in LaTaX and other related 
// information
//
// The main part is largely an LL(1) parser, with a bit of LL(*) feature
// for disambiguation. Check the docs for the concrete grammar.
// The parser also has a seperate component handling linear system.
//
// EXAMPLE:
//      Parser parser("schur[[3, 4.5], [6i, 7pi/(8e)]]")
//      const Info& result = parser.parse(0, true, "Demo")
//      if (result.success) { ... }
//      else {...}  // show users the error message
// 
class Parser
{
    typedef Token::TokName TokName;
    typedef Token::TokType TokType;

public:
    explicit Parser(const std::string& input);
    ~Parser();
    
    // 0 for auto-detection, 1 for R forced, 2 for Armadillo forced, 3 for linear system
    // save=true will save the result of current evaluation in a variable whose name 
    // is specified by var_name
    const Info& parse(int engine_type=0, bool save=false, const std::string& var_name="Ans");

    const Info& getInfo() const;

    bool assignVar(const std::string& var_name, const std::string& raw, int type);
    bool assignVar(const std::string& var_name, const ROperand& value);
    bool modifyName(const std::string& ori_name, const std::string& new_name);
    bool eraseVar(const std::string& var_name);
    std::map<std::string, std::string> retrieve_var() const;

    void print() const;
    std::string getAsciiMath() const;
    void reset_input(const std::string& input);
    
private:
    int getTokPrecedence();
    void getNextToken();
    int getNumUnknown() const;

    NumberExprAst* parseNum();
    ExprAst* parseParen();
    ExprAst* parseId();
    std::vector<ExprAst*> parseBracket();
    MatrixExprAst* parseMatrix();
    ExprAst* parsePrimary(bool inside_textbar);
    ExprAst* parseBinOpRhs(int min_precedence, ExprAst* lhs, bool inside_textbar);
    ExprAst* parseExpr(bool inside_textbar=false);
    
    std::vector<std::string> parseLinearSystem();               // EXPERIMENTAL, dedicated parser for linear system
    void parseLine(std::string line, size_t row, bool neg, std::map<std::string, std::map<int, ROperand*>>& coeffs);
    std::string findVar(ExprAst* root);
    void evalLinearSystem(std::vector<std::string> var_names);
    void modifyCoeffs(const std::string& name, size_t row, bool neg, 
                    std::map<std::string, std::map<int, ROperand*>>& coeffs, ROperand delta);

    void evalDecomp();          // decomposition will return several parallel matrices
    void evalR(bool save, const string& var_name);
    void eval(bool save, const string& var_name);
    void printAst(ExprAst* root) const;
    void autoDetect(ExprAst* root);

    static std::unordered_map<TokName, int> bin_op_precedence;
    std::map<std::string, ROperand> r_table;
    std::map<std::string, ArmaOperand> arma_table;
    std::map<std::string, int> var_table;       // 0: in expression, unassigned; 1: in expression assigned; 2: external

    Lexer tokenizer;
    std::string input;
    Token* cur_tok{nullptr};
    ExprAst* root{nullptr};
    Info res;
};


#endif /* PARSER_H_ */
