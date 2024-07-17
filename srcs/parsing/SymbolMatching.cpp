#include "../../includes/parsing/SymbolMatching.hpp"

bool SubsetSymbolMatching::match(const Token &token,
                                 std::vector<std::string> const &values)
{
    for (size_t i = 0; i < values.size(); i++)
    {
        if (values[ i ] == token.value)
            return true;
    }
    return false;
}

bool EqualSymbolMatching::match(const Token &token,
                                std::vector<std::string> const &values)
{
    if (values.size() > 1 || values.size() == 0)
        return false;
    if (values[ 0 ] != token.value)
        return false;
    return true;
}

bool DigitMatching::match(const Token &token,
                          std::vector<std::string> const &values)
{
    (void)values;
    for (size_t i = 0; i < token.value.size(); i++)
    {
        if (!std::isdigit(token.value[ i ]))
            return false;
    }
    return true;
}

// TODO: match ascii characters
bool PrintableAsciiMatching::match(const Token &token,
                                   std::vector<std::string> const &values)
{
    (void)values;
    if (token.reserved)
        return false;
    for (size_t i = 0; i < token.value.size(); i++)
    {
        if (!std::isprint(token.value[ i ]))
            return false;
    }
    return true;
}

bool SubStringMatching::match(const Token &token,
                              std::vector<std::string> const &values)
{
    for (size_t i = 0; i < values.size(); i++)
    {
        if (token.value == values[ i ])
            return true;
    }
    return false;
}

bool AlphabeticMatching::match(const Token &token,
                               std::vector<std::string> const &values)
{
    (void)values;
    for (size_t i = 0; i < token.value.size(); i++)
    {
        if (!std::isalpha(token.value[ i ]))
            return false;
    }
    return true;
}
