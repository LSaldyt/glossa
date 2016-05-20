#include <string>
#include <vector>

class Parser
{
public:
    Parser();
    ~Parser();

    std::vector<std::string> tokenize(std::string sentence);
    //NamedResult parser(std::vector<std::string> tokens);
};
