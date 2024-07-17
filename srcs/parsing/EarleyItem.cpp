#include "../../includes/parsing/Parser.hpp"

EarleyItem::EarleyItem(int rule_idx, int start, int next)
{
    m_rule_idx = rule_idx;
    m_start = start;
    m_next = next;
    m_completed = false;
}

int EarleyItem::next() const { return m_next; }

int EarleyItem::ruleIndex() const { return m_rule_idx; }

int EarleyItem::start() const { return m_start; }

bool EarleyItem::completed() const { return m_completed; }

void EarleyItem::completed(bool value) { m_completed = value; }

EarleyItem &EarleyItem::operator=(const EarleyItem &other)
{
    m_rule_idx = other.m_rule_idx;
    m_start = other.m_start;
    m_next = other.m_next;
    m_completed = other.m_completed;
    return *this;
}

bool EarleyItem::operator==(const EarleyItem &other)
{
    return m_rule_idx == other.m_rule_idx && other.m_start == m_start &&
           other.m_next == m_next;
}
