#ifndef NONTERMINALSYMBOL_HPP
#define NONTERMINALSYMBOL_HPP

#include "Grammar.hpp"
#include "GrammarSymbol.hpp"

class GrammarRule;

class NonTerminalSymbol : public AGrammarSymbol
{
private:
    std::string const m_name;
    const int m_id;

public:
    NonTerminalSymbol(std::string const name, int id);
    bool matchRule(const GrammarRule &rule);
    int ruleID() const;
    const std::string &getValue(void) const;
};

#endif
