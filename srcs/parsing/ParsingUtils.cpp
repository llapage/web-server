#include "../../includes/parsing/Parser.hpp"

EarleyEdge::EarleyEdge(EarleyItem const &item, int end)
    : m_item(item), m_end(end)
{
}

int EarleyEdge::start() const { return m_item.start(); }

int EarleyEdge::end() const { return m_end; }

int EarleyEdge::ruleIndex() const { return m_item.ruleIndex(); }

EarleyEdge &EarleyEdge::operator=(const EarleyEdge &other)
{
    m_end = other.m_end;
    return *this;
}

SearchState::SearchState(int rule_index, int depth, int node)
    : rule_index(rule_index), depth(depth), node(node)
{
}
