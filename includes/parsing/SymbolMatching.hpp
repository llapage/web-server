#ifndef SYMBOLMATCHING_HPP
#define SYMBOLMATCHING_HPP

#include "Tokenizer.hpp"
#include <vector>

class ASymbolMatching
{
public:
    virtual bool match(const Token &token,
                       std::vector<std::string> const &values) = 0;
};

class EqualSymbolMatching : public ASymbolMatching
{
public:
    bool match(const Token &token, std::vector<std::string> const &values);
};

class SubsetSymbolMatching : public ASymbolMatching
{
public:
    bool match(const Token &token, std::vector<std::string> const &values);
};

class FalseSymbolMatching : public ASymbolMatching
{
public:
    bool match(const Token &token, std::vector<std::string> const &values);
};

class SubStringMatching : public ASymbolMatching
{
public:
    bool match(const Token &token, std::vector<std::string> const &values);
};

class DigitMatching : public ASymbolMatching
{
public:
    bool match(const Token &token, std::vector<std::string> const &values);
};

class PrintableAsciiMatching : public ASymbolMatching
{
public:
    bool match(const Token &token, std::vector<std::string> const &values);
};

class AlphabeticMatching : public ASymbolMatching
{
public:
    bool match(const Token &token, std::vector<std::string> const &values);
};

#endif
