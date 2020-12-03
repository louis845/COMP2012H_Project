#include "lexer.h"
using std::iostream;
using std::string;
using std::regex;
using std::smatch;


std::regex Lexer::WHITESPACE_RE = std::regex((R"(^\s+)"));
std::regex Lexer::DELIM_RE = std::regex((R"(^\s*(,|\\\\))"));
std::regex Lexer::OP_RE = std::regex((R"(^\s*(\||\+|-|\*\*|\*|\/\/|xx|\/|\^|\(|\)|=|\[|\]|\{|\}|_|%|sqrt|root|abs|norm|sin|cos|tan|sec|csc|cot|arcsin|arccos|arctan|exp|log|ln|det|rank|ran|Ran|col|Col|orth|Ker|ker|mod|gcd|lcm|min|max|trace|tr|RREF|rref|inv|pinv|eigen|schur|svd|solve|qr|charpoly|answer))"));
std::regex Lexer::NUM_RE = std::regex((R"(^\s*(e|i|I|pi|((\+|-)?(\.[0-9]+|[0-9]+\.?[0-9]*)((e|E)(\+|-)?[0-9]+)?)))"));
std::regex Lexer::ID_RE = std::regex((R"(^\s*(alpha|beta|theta|lambda|mu|phi|varphi|omega|[a-zA-Z]+)(')?(_\w*)?)"));


// Get next token in the input stream
//
// It follows the rules below
// 1) if all meaningful characters are exhausted, return nullptr
// 2) otherwise if no valid token is found, return TokErr
// 3) otherwise return the corresponding valid token
// If ignore_invalid_token is enabled, will keep advancing the cursor for the 2) case
// In this case, only a nullptr or a valid token will be returned
//
// NOTE: ignore_invalid_token is unused in parser so far
Token* Lexer::getNextToken(bool ignore_invalid_token)
{
    while (input.length() > 0)
    {
        if (regex_match(input, WHITESPACE_RE))   return nullptr;
        input = std::regex_replace(input, WHITESPACE_RE, "");

        // Each parser function will handle exceptions and return TokErr
        // If no pattern matches, the Token* returned will be a nullptr
        // otherwise memory will be allocated for the new token
        // The parser which receives the pointer has the responsibility to deallocate it
        Token* temp = parseDelim();
        if (temp == nullptr)    temp = parseOp();
        if (temp == nullptr)    temp = parseNum();
        if (temp == nullptr)    temp = parseId();
        if (temp == nullptr)
        {
            if (ignore_invalid_token)   
            {
                // Skip one character and search again
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

    Token* temp = new TokDelim(result[1]);
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

    Token* temp = new TokNum(result[1]);
    input = std::move(result.suffix().str());
    return temp;
}


Token* Lexer::parseId()
{
    smatch result;
    regex_search(input, result, ID_RE);
    if (result.empty()) return nullptr;

    Token* temp = new TokId(result[0]);
    input = std::move(result.suffix().str());
    return temp;   
}