#include "../../includes/parsing/TerminalSymbol.hpp"

TerminalSymbol::TerminalSymbol() : m_value(""), m_id(0) {}

TerminalSymbol::TerminalSymbol(std::string const value, int const id)
    : m_value(value), m_id(id)
{
}

TerminalSymbol::~TerminalSymbol() {}

int TerminalSymbol::ruleID() const { return m_id; }

bool TerminalSymbol::match(const Token &token) const
{
    return token.value == m_value;
}

bool TerminalSymbol::terminal(void) const { return true; }

const std::string &TerminalSymbol::getValue(void) const { return m_value; }

TerminalSymbolSet::TerminalSymbolSet(std::string const name, int id,
                                     std::vector<std::string> const values,
                                     ASymbolMatching &matching)
    : m_value(name), m_id(id), m_values(values), m_matching(matching)
{
}

bool TerminalSymbolSet::terminal(void) const { return true; }

bool TerminalSymbolSet::match(Token const &token) const
{
    return m_matching.match(token, m_values);
}

const std::string &TerminalSymbolSet::getValue() const { return m_value; }

int TerminalSymbolSet::ruleID() const { return m_id; }

TerminalSymbolSet::~TerminalSymbolSet() {}
