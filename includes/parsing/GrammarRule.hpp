#ifndef GRAMMARRULE_HPP
#define GRAMMARRULE_HPP

#include "GrammarSymbol.hpp"
#include "NonTerminalSymbol.hpp"
#include "SemanticAction.hpp"

class AGrammarSymbol;
class NonTerminalSymbol;

class GrammarRule
{
private:
    int m_rule_idx;
    NonTerminalSymbol const &m_symbol;
    std::vector<AGrammarSymbol *> m_symbols;

public:
    GrammarRule(NonTerminalSymbol const &symbol);
    ~GrammarRule();
    int getRuleIndex(void) const;
    int ruleID(void) const;
    const std::string &getName(void) const;
    void setRuleIndex(int rule_idx);
    void addSymbol(AGrammarSymbol *symbol);
    AGrammarSymbol *getSymbol(int index) const;
    bool operator==(const GrammarRule &other);
    size_t size(void) const;
};

#endif
