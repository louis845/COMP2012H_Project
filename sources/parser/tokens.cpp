#include "tokens.h"
#include <algorithm>
#include <stdexcept>
#include <cctype> //Possible header to use std::tolower?


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
