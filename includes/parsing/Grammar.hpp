#ifndef GRAMMAR_HPP
#define GRAMMAR_HPP

#include "SemanticAction.hpp"
#include <cstddef>
#include <vector>

class NonTerminalSymbol;

class GrammarRule;

class Grammar
{
private:
    std::vector<GrammarRule> m_rules;
    size_t m_size;

public:
    Grammar();
    ~Grammar();
    GrammarRule *addRule(NonTerminalSymbol const &symbol);
    size_t size() const;
    const GrammarRule *getRule(int index) const;
};

#endif
