#ifndef PARSETREE_HPP
#define PARSETREE_HPP

#include "GrammarRule.hpp"
#include <vector>

class EarleyEdge;

class ParseTree
{
private:
    int m_rule_id;
    bool m_is_leaf;
    int m_start;
    int m_end;
    int m_rule_idx;
    int m_size;
    int m_token_idx;
    std::vector<ParseTree *> m_subtrees;

public:
    ParseTree(void);
    ParseTree(ParseTree const &other);
    ~ParseTree(void);
    ParseTree *operator[](int index);
    ParseTree &operator=(ParseTree const &other);
    void setSpan(int start, int end);
    int start(void) const;
    int end(void) const;
    int ruleIndex(void) const;
    void ruleIndex(int value);
    int tokenIndex(void) const;
    void tokenIndex(int value);
    bool leaf(void) const;
    void leaf(bool value);
    void addSubtree(int start, int end, int index, const GrammarRule &rule);
    int ruleID(void) const;
    void ruleID(int id);
    size_t size(void) const;
};

#endif
