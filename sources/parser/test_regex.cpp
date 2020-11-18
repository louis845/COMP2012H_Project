#include <iostream>
#include <string>
#include <regex>

using namespace std;

void test()
{
    string input;
    // cin >> input;

    input = "sinc cos det[[3.3432/4e34, e^pi], [.344 + 4i // 5.34400 * 006 % 34.7], [sqrt(x^2)]] cos ";
    input = "c";

    std::regex NUM_PATTERN((R"(^(e|i|I|pi|(\+|-)?(\.[0-9]+|[0-9]+\.?[0-9]*)((e|E)(\+|-)?[0-9]+)?))"));
    std::regex DELIM_PATTERN((R"(^,)"));
    std::regex OP_PATTERN((R"(^(\||\+|-|\*\*|\*|\/\/|xx|\/|\^|\(|\)|=|\[|]|\{|\}|_|%|sqrt|root|abs|norm|sin|cos|tan|sec|csc|cot|arcsin|arccos|arctan|exp|log|ln|det|dim|ran|Ran|col|Col|Ker|ker|mod|gcd|lcm|min|max|trace|tr|RREF|rref))"));
    std::regex ID_PATTERN((R"(^(alpha|beta|theta|lambda|mu|phi|varphi|omega|[a-zA-Z]))"));
    
    smatch result;
    regex_search(input, result, ID_PATTERN);
    for (auto i : result)
        cout << i << endl;
    cout << result.size() << endl << result.empty() << endl;
    cout << "suffix: " << endl << result.suffix().str() << endl;
}