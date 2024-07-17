#ifndef RECOGNIZER_HPP
#define RECOGNIZER_HPP

#include "GrammarSymbol.hpp"

class Grammar;

class EarleyItem;

class Recognizer
{
private:
    int m_state_idx;
    AGrammarSymbol *m_symbol;
    void m_scan(std::vector<std::vector<EarleyItem> > &sets, Token const &token,
                EarleyItem const &item);
    void m_complete(const Grammar &grammar,
                    std::vector<std::vector<EarleyItem> > &sets,
                    std::vector<EarleyItem> &current_set, int item_index);
    void m_predict(const Grammar &grammar,
                   std::vector<EarleyItem> &current_set);

public:
    Recognizer();
    ~Recognizer();
    void recognize(std::vector<Token> const &tokens, Grammar const &grammar,
                   std::vector<std::vector<EarleyItem> > &sets);
    void print(Grammar const &grammar,
               std::vector<std::vector<EarleyItem> > &sets);
};

#endif
