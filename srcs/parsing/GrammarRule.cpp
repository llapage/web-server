#include "../../includes/parsing/GrammarRule.hpp"

GrammarRule::GrammarRule(NonTerminalSymbol const &symbol) : m_symbol(symbol) {}

GrammarRule::~GrammarRule() {}

int GrammarRule::getRuleIndex() const { return m_rule_idx; }

const std::string &GrammarRule::getName() const { return m_symbol.getValue(); }

void GrammarRule::setRuleIndex(int rule_idx) { m_rule_idx = rule_idx; }

void GrammarRule::addSymbol(AGrammarSymbol *symbol)
{
    m_symbols.push_back(symbol);
}

bool GrammarRule::operator==(const GrammarRule &other)
{
    return m_symbol.ruleID() == other.ruleID() &&
           m_rule_idx == other.m_rule_idx;
}

AGrammarSymbol *GrammarRule::getSymbol(int index) const
{
    if (static_cast<size_t>(index) >= m_symbols.size())
        return NULL;
    return m_symbols[ index ];
}

int GrammarRule::ruleID(void) const { return m_symbol.ruleID(); }

size_t GrammarRule::size(void) const { return m_symbols.size(); }
