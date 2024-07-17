#include "../../includes/parsing/NonTerminalSymbol.hpp"
#include "../../includes/parsing/GrammarRule.hpp"

NonTerminalSymbol::NonTerminalSymbol(std::string const name, int id)
    : m_name(name), m_id(id)
{
}

int NonTerminalSymbol::ruleID() const { return m_id; }

bool NonTerminalSymbol::matchRule(const GrammarRule &rule)
{
    return m_id == rule.ruleID();
}

const std::string &NonTerminalSymbol::getValue(void) const { return m_name; }
