#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>
#include <unordered_map>

// Check the README for more information about tokens


class Token
{
public:
    enum class TokType {DELIM, OP, NUM, ID, ERR};

    enum class TokName
    {
        COMMA = 100, // DELIM
        AST = 200, CDOT, CROSS, DIV, FRAC, PERCENT, PLUS, MINUS, 
        SUP, SUB, TEXTBAR, LP, RP, LSB, RSB, LCB, RCB, EQUAL, 
        SIN, COS, TAN, SEC, CSC, COT, ARCSIN, ARCCOS, ARCTAN, EXP, LOG, LN,
        DET, DIM, RAN, COL, KER, MOD, GCD, LCM, MIN, MAX, TR,
        RREF, NORM, ABS, SQRT, ROOT, // OPERATOR
        E = 300, PI, I, IDENTITY_MATRIX, INTEGRAL, FLOAT, // NUM
        ALPHA = 400, BETA, THETA, LAMBDA, MU, PHI, VARPHI, OMEGA, VARIABLE, // IDENTIFIER
        INVALID_TOKEN = 500, NUMERICAL_ERROR // ERR
    };

    static std::unordered_map<std::string, int> tok_map;

    explicit Token(TokType prime_type): type(prime_type) {}
    virtual ~Token() = default;

    const TokType& get_type() const { return type; }
    virtual const TokName& get_name() const = 0;
    virtual const std::string& get_raw_value() const = 0;
    virtual long double get_num_value() const { return 0.0L; };

protected:
    TokType type;
};


class TokDelim : public Token
{
public:
    TokDelim(): Token(TokType::DELIM), name(TokName::COMMA), raw_value(",") {}

    explicit TokDelim(const std::string& value): Token(TokType::DELIM)
    {
        if(tok_map.count(value) == 0)
            throw std::invalid_argument(value);
        name = static_cast<TokName>(tok_map[value]);
        raw_value = value;
    }

    const TokName& get_name() const override { return name; }
    const std::string& get_raw_value() const override { return raw_value; }

private:
    TokName name;
    std::string raw_value;
};


class TokOp : public Token
{
public:
    explicit TokOp(const std::string& value): Token(TokType::OP)
    {
        if (tok_map.count(value) == 0)
            throw std::invalid_argument(value);
        name = static_cast<TokName>(tok_map[value]);
        raw_value = value;
    }

    const TokName& get_name() const override { return name; }
    const std::string& get_raw_value() const override { return raw_value; }

private:
    TokName name;
    std::string raw_value;
};


class TokNum : public Token
{
public:
    explicit TokNum(const std::string& value): Token(TokType::NUM), raw_value(value)
    {
        if (tok_map.count(value) == 0)
        {
            if (value.find('.') == std::string::npos)
            {
                // int_value = stoll(value);   // stoll may throw an exception, catch it in Lexer::parseNum()
                name = TokName::INTEGRAL;
            }
            else
            {
                // float_value = stold(value);   // stold may throw an exception, catch it in Lexer::parseNum()
                name = TokName::FLOAT;
            }
        }
        else    name = static_cast<TokName>(tok_map[value]);
    }

    const TokName& get_name() const override { return name; }
    const std::string& get_raw_value() const override { return raw_value; }
    // long double get_float() const { return float_value; }
    // long long get_int() const { return int_value; }

private:
    TokName name;
    std::string raw_value;
    // long long int_value{0L};
    // long double float_value{0.0L};
};


class TokId : public Token
{
public:
    explicit TokId(const std::string& value): Token(TokType::ID), raw_value(value)
    {
        if (tok_map.count(value) == 0)
            name = TokName::VARIABLE;
        else
            name = static_cast<TokName>(tok_map[value]);
    }

    const TokName& get_name() const override { return name; }
    const std::string& get_raw_value() const override { return raw_value; }

private:
    TokName name;
    std::string raw_value;
};


class TokErr : public Token
{
public:
    
    TokErr(const TokName& name, std::string value):
        Token(TokType::ERR), name(name), raw_value(std::move(value)) {}

    const TokName& get_name() const override { return name; }
    const std::string& get_raw_value() const override { return raw_value; }

private:
    TokName name;
    std::string raw_value;
};

#endif /* TOKEN_H_ */