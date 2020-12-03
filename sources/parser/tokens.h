#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>
#include <unordered_map>


// Base class for all the tokens
class Token
{
public:
    // The general type of the token: delimiters, operators
    // numbers, identifiers and errors
    enum class TokType {DELIM, OP, NUM, ID, ERR};

    // The specific name of each token
    // Check the token list in docs for more details
    enum class TokName
    {
        ANSWER = 42,        // You know, 42 is the Answer to the Ultimate Question of Life, the Universe, and Everything ;)
        COMMA = 100, ENDL,  // DELIM
        AST = 200, CDOT, CROSS, DIV, FRAC, PERCENT, PLUS, MINUS,
        SUP, SUB, TEXTBAR, LP, RP, LSB, RSB, LCB, RCB, EQUAL, 
        SIN, COS, TAN, SEC, CSC, COT, ARCSIN, ARCCOS, ARCTAN, EXP, LOG, LN,
        DET, RANK, ORTH, TRANS, KER, MOD, GCD, LCM, MIN, MAX, TR,
        RREF, NORM, ABS, SQRT, ROOT, NEG, INV, PINV, EIGEN, SCHUR,
        SVD, SOLVE, QR, ROOTS, CHAR_POLY, NA,  // OPERATOR
        E = 300, PI, I, IDENTITY_MATRIX, INTEGRAL, FLOAT, // NUM
        ALPHA = 400, BETA, THETA, LAMBDA, MU, PHI, VARPHI, OMEGA, VARIABLE, // IDENTIFIER
        INVALID_TOKEN = 500, NUMERICAL_ERROR // ERR
    };

    // For lexer to quickly translate the string into a token stream
    // int will be static cast to enum class TokName type
    static std::unordered_map<std::string, int> tok_map;

    explicit Token(TokType prime_type, std::string raw_value): 
        type(prime_type), raw_value(raw_value) {}
    virtual ~Token() = default;

    const TokType& get_type() const { return type; }

    virtual const TokName& get_name() const { return name; };

    // Access the raw string representation of the token
    virtual const std::string& get_raw_value() const { return raw_value; };

    // Unused after the modification of the architecture
    virtual long double get_num_value() const { return 0.0L; };

protected:
    TokType type;
    TokName name;
    std::string raw_value;
};


// Class for delimiters
class TokDelim : public Token
{
public:
    TokDelim(): Token(TokType::DELIM, ",") { this->name = TokName::COMMA; }

    explicit TokDelim(const std::string& value);
};


// Class for operators
//
// NOTE: functions, parentheses and binary operators are all
// classified as operators since they will change the parser
// behaviour
class TokOp : public Token
{
public:
    explicit TokOp(const std::string& value);

};


// Class for numbers, scientific constants, imaginary number i and
// identity matrix I
//
// The string to long/double conversion is handled by NumExprAst now
// This class only determines the concrete type of the token
class TokNum : public Token
{
public:
    explicit TokNum(const std::string& value);

};


// Class for identifiers i.e. variable names
class TokId : public Token
{
public:
    explicit TokId(const std::string& value);

};


class TokErr : public Token
{
public:
    TokErr(const TokName& name, std::string value):
        Token(TokType::ERR, std::move(value)) {}

};

#endif /* TOKEN_H_ */