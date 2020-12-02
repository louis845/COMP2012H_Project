#include "tokens.h"
#include <algorithm>
#include <stdexcept>
#include <cctype>


std::unordered_map<std::string, int> Token::tok_map = 
{
    {"answer", 42},
    {",", 100}, {R"(\\)", 101},
    {"**", 200}, {"*", 201}, {"xx", 202}, {"//", 203}, {"/", 204}, {"%", 205}, {"+", 206}, {"-", 207},
    {"^", 208}, {"_", 209}, {"|", 210}, {"(", 211}, {")", 212}, {"[", 213}, {"]", 214}, {"{", 213}, {"}", 214}, {"=", 217},
    {"sin", 218}, {"cos", 219}, {"tan", 220}, {"sec", 221}, {"csc", 222}, {"cot", 223}, {"arcsin", 224}, {"arccos", 225}, 
    {"arctan", 226}, {"exp", 227}, {"log", 228}, {"ln", 229}, {"det", 230}, {"rank", 231}, {"ran", 232}, {"col", 232}, {"orth", 232},
    {"ker", 234}, {"null", 234}, {"mod", 235}, {"gcd", 236}, {"lcm", 237}, {"min", 238}, {"max", 239}, {"trace", 240}, {"tr", 240}, 
    {"rref", 241}, {"norm", 242}, {"abs", 243}, {"sqrt", 244}, {"root", 245}, {"inv", 247}, {"pinv", 248}, {"eigen", 249}, {"schur", 250},
    {"svd", 251}, {"solve", 252}, {"qr", 253}, {"charpoly", 255},
    {"e", 300}, {"pi", 301}, {"i", 302}, {"I", 303},
    {"alpha", 400}, {"beta", 401}, {"theta", 402}, {"lambda", 403}, {"mu", 404}, {"phi", 405}, {"varphi", 406}, {"omega", 407}
};


TokDelim::TokDelim(const std::string& value): Token(TokType::DELIM)
{
    if(tok_map.count(value) == 0)
        throw std::invalid_argument(value);
    name = static_cast<TokName>(tok_map[value]);
    raw_value = value;
}


TokOp::TokOp(const std::string& value): Token(TokType::OP)
{
    std::string lower_case_value = value;
    std::transform(value.begin(), value.end(), lower_case_value.begin(),
                    [](unsigned char c){ return std::tolower(c); });

    if (tok_map.count(lower_case_value) == 0)
        throw std::invalid_argument(value);
    name = static_cast<TokName>(tok_map[lower_case_value]);
    raw_value = value;
}


TokNum::TokNum(const std::string& value): Token(TokType::NUM), raw_value(value)
{
    if (tok_map.count(value) == 0)
    {
        name = TokName::INTEGRAL;
        if (value.find('.') != std::string::npos)   name = TokName::FLOAT;
        else
        {
            auto pos = value.find('e');
            if (pos != std::string::npos && value[pos + 1] == '-')  
                name = TokName::FLOAT;      // 1e-0 will be treated as float
            else
            {
                pos = value.find('E');
                if (pos != std::string::npos && value[pos + 1] == '-') 
                    name = TokName::FLOAT;
            }
        }
    }
    else    name = static_cast<TokName>(tok_map[value]);
}


TokId::TokId(const std::string& value): Token(TokType::ID), raw_value(value)
{
    if (tok_map.count(value) == 0)
        name = TokName::VARIABLE;
    else
        name = static_cast<TokName>(tok_map[value]);
}
