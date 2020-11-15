#include "lexer.h"
using std::iostream;
using std::pair;
using std::string;
using std::regex;
using std::smatch;


std::unordered_map<std::string, int> Token::tok_map = 
{
    {",", 100}, 
    {"**", 200}, {"*", 201}, {"xx", 202}, {"//", 203}, {"/", 204}, {"%", 205}, {"+", 206}, {"-", 207},
    {"^", 208}, {"_", 209}, {"|", 210}, {"(", 211}, {")", 212}, {"[", 213}, {"]", 214}, {"{", 215}, {"}", 216}, {"=", 217},
    {"sin", 218}, {"cos", 219}, {"tan", 220}, {"sec", 221}, {"csc", 222}, {"cot", 223}, {"arcsin", 224}, {"arccos", 225}, 
    {"arctan", 226}, {"exp", 227}, {"log", 228}, {"ln", 229}, {"det", 230}, {"dim", 231}, {"ran", 232}, {"col", 233}, {"ker", 234}, 
    {"mod", 235}, {"gcd", 236}, {"lcm", 237}, {"min", 238}, {"max", 239}, {"trace", 240}, {"tr", 240}, {"rref", 241}, {"norm", 242},
    {"abs", 243}, {"sqrt", 244}, {"root", 245}, 
    {"e", 300}, {"pi", 301}, {"i", 302}, {"I", 303},
    {"alpha", 400}, {"beta", 401}, {"theta", 402}, {"lambda", 403}, {"mu", 404}, {"phi", 405}, {"varphi", 406}, {"omega", 407}
};


std::regex Lexer::WHITESPACE_RE = std::regex((R"(\s+)"));
std::regex Lexer::DELIM_RE = std::regex((R"(^\s*,)"));
std::regex Lexer::OP_RE = std::regex((R"(^\s*(\||\+|-|\*\*|\*|\/\/|xx|\/|\^|\(|\)|=|\[|]|\{|\}|_|%|sqrt|root|abs|norm|sin|cos|tan|sec|csc|cot|arcsin|arccos|arctan|exp|log|ln|det|dim|ran|Ran|col|Col|Ker|ker|mod|gcd|lcm|min|max|trace|tr|RREF|rref))"));
std::regex Lexer::NUM_RE = std::regex((R"(^\s*(e|i|I|pi|(\+|-)?(\.[0-9]+|[0-9]+\.?[0-9]*)((e|E)(\+|-)?[0-9]+)?))"));
std::regex Lexer::ID_RE = std::regex((R"(^\s*(alpha|beta|theta|lambda|mu|phi|varphi|omega|[a-zA-Z]))"));


// 1) if all meaningful characters are exhausted, return nullptr
// 2) otherwise if no valid token is found, return TokErr
// 3) otherwise return the corresponding valid token
// if ignore_invalid_token is enabled, will keep advancing the cursor for the 2) case
// in this case, only a nullptr or a valid token will be returned
Token* Lexer::getNextToken(bool ignore_invalid_token)
{
    while (input.length() > 0)
    {
        if (regex_match(input, WHITESPACE_RE))   return nullptr;

        // each parser function will handle exceptions and return TokErr
        // if no pattern matches, the Token* returned will be a nullptr
        // otherwise memory will be allocated for the new token
        // the parser which receives the pointer has the responsibility to deallocate it
        Token* temp = parseDelim();
        if (temp == nullptr)    temp = parseOp();
        if (temp == nullptr)    temp = parseNum();
        if (temp == nullptr)    temp = parseId();
        if (temp == nullptr)
        {
            if (ignore_invalid_token)   
            {
                input.erase(0, 1);
                continue;
            }
            else
            {
                Token* err = new TokErr(TokName::INVALID_TOKEN, input);
                return err;
            }
        }
        return temp;
    }
    return nullptr;   
}


Token* Lexer::parseDelim()
{
    smatch result;
    regex_search(input, result, DELIM_RE);
    if (result.empty()) return nullptr;

    Token* temp = new TokDelim();
    input = std::move(result.suffix().str());
    return temp;
}


Token* Lexer::parseOp()
{
    smatch result;
    regex_search(input, result, OP_RE);
    if (result.empty()) return nullptr;

    try
    {
        Token* temp = new TokOp(result[1]);
        input = std::move(result.suffix().str());
        return temp;
    }
    catch (const std::invalid_argument& e)
    {
        std::cerr << "Invalid operator: " << e.what() 
                  << " is not a supported operator." << std::endl;
        Token* temp = new TokErr(TokName::INVALID_TOKEN, e.what());
        return temp;
    }
}


Token* Lexer::parseNum()
{
    smatch result;
    regex_search(input, result, NUM_RE);
    if (result.empty()) return nullptr;

    try
    {
        Token* temp = new TokNum(result[1]);
        input = std::move(result.suffix().str());
        return temp;
    }
    catch (const std::invalid_argument& e)
    {
        std::cerr << "Invalid numerical value: " << result[1] 
                  << " is not a valid or supported numerical value." << std::endl;
        Token* temp = new TokErr(TokName::INVALID_TOKEN, result[1]);
        return temp;
    }
    catch (const std::out_of_range& e)
    {
        std::cerr << "Invalid numerical value: " << result[1] 
                  << " is out of range of long double type." << std::endl;
        Token* temp = new TokErr(TokName::NUMERICAL_ERROR, result[1]);
        return temp;
    }
}


Token* Lexer::parseId()
{
    smatch result;
    regex_search(input, result, ID_RE);
    if (result.empty()) return nullptr;

    Token* temp = new TokId(result[1]);
    input = std::move(result.suffix().str());
    return temp;   
}