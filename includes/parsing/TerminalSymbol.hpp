#ifndef TERMINALSYMBOL_HPP
#define TERMINALSYMBOL_HPP

#include "GrammarSymbol.hpp"
#include "SymbolMatching.hpp"
#include <vector>

// represents a symbols to be matched by the aparser
class TerminalSymbol : public AGrammarSymbol
{
protected:
    std::string const m_value;
    int const m_id;

public:
    TerminalSymbol();
    TerminalSymbol(std::string const name, int id);
    ~TerminalSymbol();
    bool terminal(void) const;
    bool match(const Token &token) const;
    const std::string &getValue(void) const;
    int ruleID() const;
};

// groups a set of symbols to be matched when parsing.
class TerminalSymbolSet : public TerminalSymbol
{
protected:
    std::string const m_value;
    int const m_id;

private:
    std::vector<std::string> const m_values;
    ASymbolMatching &m_matching;

public:
    TerminalSymbolSet(std::string const name, int id,
                      std::vector<std::string> const values,
                      ASymbolMatching &matching);
    ~TerminalSymbolSet();
    bool terminal(void) const;
    bool match(const Token &token) const;
    const std::string &getValue() const;
    int ruleID() const;
};

#endif
