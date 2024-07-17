#ifndef PARSER_HPP
#define PARSER_HPP

#include "Grammar.hpp"
#include "ParseTree.hpp"
#include "Recognizer.hpp"
#include <vector>

class Recognizer;

class EarleyItem
{
private:
    int m_rule_idx;
    int m_start;
    int m_next;
    bool m_completed;

public:
    EarleyItem(int rule_idx, int start, int next);
    bool operator==(const EarleyItem &other);
    int ruleIndex() const;
    int start() const;
    int next() const;
    bool completed() const;
    void completed(bool value);
    EarleyItem &operator=(const EarleyItem &other);
};

class EarleyEdge
{
private:
    const EarleyItem &m_item;
    int m_end;

public:
    EarleyEdge(const EarleyItem &item, int end);
    int start() const;
    int end() const;
    int ruleIndex() const;
    EarleyEdge &operator=(const EarleyEdge &other);
};

struct SearchState
{
    const int rule_index;
    const int depth;
    const int node;
    SearchState(int rule_idx, int depth, int node);
};

class Parser
{
private:
    Grammar const &m_grammar;
    // std::vector<ASemanticAction<T> >& 		m_semantic_actions;
    std::vector<std::vector<EarleyItem> > m_earley_sets;
    std::vector<std::vector<EarleyEdge> > m_chart;
    ParseTree *m_parse_tree;
    Recognizer m_recognizer;
    void m_buildTree(ParseTree &parse_tree, const std::vector<Token> &tokens);
    bool m_searchPath(ParseTree &parse_tree, SearchState state,
                      const std::vector<Token> &tokens);
    bool m_processTerminal(ParseTree &parse_tree, SearchState state,
                           const AGrammarSymbol &symbol,
                           const std::vector<Token> &tokens);

public:
    Parser(Grammar const &grammar);
    ~Parser();
    ParseTree &parse(std::vector<Token> const &tokens);
    void print(std::vector<Token> const &tokens);
};

#endif
