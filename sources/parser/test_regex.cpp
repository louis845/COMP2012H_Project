#include <iostream>
#include <string>
#include <regex>

using namespace std;

int main()
{
    string input;
    // cin >> input;

    input = R"(4a_1 + (4//5 * 6i))";

    std::regex NUM_PATTERN((R"(^(e|i|I|pi|(\+|-)?(\.[0-9]+|[0-9]+\.?[0-9]*)((e|E)(\+|-)?[0-9]+)?))"));
    std::regex DELIM_PATTERN((R"(^,)"));
    std::regex OP_PATTERN((R"(^(\||\+|-|\*\*|\*|\/\/|xx|\/|\^|\(|\)|=|\[|]|\{|\}|_|%|sqrt|root|abs|norm|sin|cos|tan|sec|csc|cot|arcsin|arccos|arctan|exp|log|ln|det|dim|ran|Ran|col|Col|Ker|ker|mod|gcd|lcm|min|max|trace|tr|RREF|rref))"));
    std::regex ID_PATTERN((R"(^(alpha|beta|theta|lambda|mu|phi|varphi|omega|[a-zA-Z]))"));
    std::regex pattern(R"((alpha|beta|theta|lambda|mu|phi|varphi|omega|[a-zA-Z])(_\w+)?)");
    
    smatch result;
    regex_search(input, result, ID_PATTERN);
    for (auto i : result)
        cout << i << endl;
    cout << result.size() << endl << result.empty() << endl;
    cout << "suffix: " << endl << result.suffix().str() << endl;
}