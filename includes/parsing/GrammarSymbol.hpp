#ifndef GRAMMARSYMBOL_HPP
#define GRAMMARSYMBOL_HPP

#include "Tokenizer.hpp"
// #include "Grammar.hpp"

struct GrammarRuleID;

class GrammarRule;

enum GrammarSymbolType
{
    TERMINAL,
    NON_TERMINAL
};

class AGrammarSymbol
{
private:
    int m_rule_idx;

public:
    virtual ~AGrammarSymbol() = 0;
    int getRuleIndex(void) const;
    void setRuleIndex(int index);
    virtual bool match(const Token &token) const;
    virtual bool matchRule(GrammarRule const &rule);
    virtual bool terminal(void) const;
    virtual int ruleID() const = 0;
    virtual const std::string &getValue(void) const = 0;
};

#endif
