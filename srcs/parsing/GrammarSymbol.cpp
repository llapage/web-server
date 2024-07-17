#include "../../includes/parsing/GrammarSymbol.hpp"
#include "../../includes/parsing/Grammar.hpp"

AGrammarSymbol::~AGrammarSymbol() {}

int AGrammarSymbol::getRuleIndex() const { return m_rule_idx; }

void AGrammarSymbol::setRuleIndex(int index) { m_rule_idx = index; }

bool AGrammarSymbol::match(const Token &token) const
{
    (void)token;
    return false;
}

bool AGrammarSymbol::terminal(void) const { return false; }

bool AGrammarSymbol::matchRule(const GrammarRule &rule)
{
    (void)rule;
    return false;
}
