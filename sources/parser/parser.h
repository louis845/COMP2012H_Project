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
    
    // engine_type specify the engine to be used
    // 0 for auto-detection, 1 for R forced, 2 for Armadillo forced, 3 for linear system
    // save=true will save the result of current evaluation in a variable whose name 
    // is specified by var_name
    const Info& parse(int engine_type=0, bool save=false, const std::string& var_name="Ans");

    const Info& getInfo() const;


    // Assign values to variables by passing the literal input from the user
    // The input needs to be in AsciiMath format and the engine_type used to 
    // parsed the input
    bool assignVar(const std::string& var_name, const std::string& raw, int type);


    // Explicitly assign values to variables by passing the corresponding ROperand
    // Therefore, users can use the intermediate steps of linear operations as
    // variables to reduce the effort of input
    bool assignVar(const std::string& var_name, const ROperand& value);


    // Change the name of an existing variable to an unused name
    // NOTE: the variable name appeared in the input CANNOT be modified
    // even if they are not assigned with values
    bool modifyName(const std::string& ori_name, const std::string& new_name);

    
    // Erase a variable, which CANNOT be the variables appeared in the expression
    bool eraseVar(const std::string& var_name);


    // Check if a variable name is valid, i.e. not a number or reserved operator
    bool checkVarNameValid(const std::string& var_name) const;


    // Retrieve a map from all the variables names to their values in LaTaX
    // Variables with no values assigned will have value empty string
    std::map<std::string, std::string> retrieve_var() const;


    // Print the evaluation result in LaTaX, only used in CLI
    void print() const;


    // Get the AsciiMath representation reconstructed from the AST
    // It is useful if user wants to check whether the interpretation
    // is correct
    std::string getAsciiMath() const;


    // Reset the parser input. This will reset the Info object within the
    // parser as well. It will wipe away all the variables WITHOUT values,
    // but retain those variables already been assigned.
    void reset_input(const std::string& input);
    
private:

    // Get the precedence of binary operators to determine the binding
    // of binary expressions
    int getTokPrecedence();

    // Call the tokenizer's getNextToken() with some processing
    void getNextToken();

    // Retrieve the number of variables with unknown values in the input
    // before do the actual evaluation
    int getNumUnknown() const;


    // All the individual parsers for each type of AST node
    // Check the docs for the detailed grammar for each 
    NumberExprAst* parseNum();
    ExprAst* parseParen();
    ExprAst* parseId();
    std::vector<ExprAst*> parseBracket();
    MatrixExprAst* parseMatrix();
    ExprAst* parsePrimary(bool inside_textbar);
    ExprAst* parseBinOpRhs(int min_precedence, ExprAst* lhs, bool inside_textbar);
    ExprAst* parseExpr(bool inside_textbar=false);
    


    // EXPERIMENTAL, dedicated parsers for linear system
    // This set of parsers are used to handle input like:
    //    (4t + 1) x - (5i / 3t) y = 4z
    //    (4 * 3t) x               = 3 + 5y
    //             x + (34 / 5)  y = 4i - 7z                
    std::vector<std::string> parseLinearSystem();

    // Parse each side on the equal sign of each line
    // size_t row indicates the current row location in the linear system
    // bool neg is true if the line is at the left hand side of the equal sign, false otherwise
    // coeffs stores the coefficients of each variable by their name
    void parseLine(std::string line, size_t row, bool neg, std::map<std::string, std::map<int, ROperand*>>& coeffs);
    
    // Find the variable name in the AST subtree
    std::string findVar(ExprAst* root);

    // Driver function, it solves the linear system according to the
    // return value of parseLinearSystem(). The result will be stored in
    // the Info object and returned by the parser().
    void evalLinearSystem(std::vector<std::string> var_names);

    // Add to or create a coefficient of the corresponding variable
    void modifyCoeffs(const std::string& name, size_t row, bool neg, 
                    std::map<std::string, std::map<int, ROperand*>>& coeffs, ROperand delta);



    // Evaluation related part
    //
    // Decomposition will return several parallel matrices, hence the
    // evaluation is done separately
    void evalDecomp();          

    // Evaluate using R class, save and var_name are passed by the main parse()
    // with same functions as in the parse() function
    //
    // Internally, it will call the evaluation function of ExprAst class, and
    // do exception handling
    void evalR(bool save, const string& var_name);

    // Similar as above, differs in exception handling and the checking process
    // of whether the evaluation can be done on given expression
    void eval(bool save, const string& var_name);

    // Print the infix notation of the whole AST, only used in CLI
    void printAst(ExprAst* root) const;

    // Auto detect which engine for evaluation to be used after parsing
    void autoDetect(ExprAst* root);

    static std::unordered_map<TokName, int> bin_op_precedence;

    // name-to-value mapping for ROperand and ArmaOperand respectively
    std::map<std::string, ROperand> r_table;
    std::map<std::string, ArmaOperand> arma_table;


    // A global variable table storing variables with values assigned
    // and those appeared in the expression but without values yet
    //
    // It is a string-to-int mapping, the int may take value 0: in expression, unassigned,
    // 1: in expression, assigned, and 2: external variables, assigned
    std::map<std::string, int> var_table;

    Lexer tokenizer;
    std::string input;
    Token* cur_tok{nullptr};
    ExprAst* root{nullptr};
    Info res;
};


#endif /* PARSER_H_ */
