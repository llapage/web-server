#include "../../includes/parsing/ParseTree.hpp"

ParseTree::ParseTree(void)
{
    m_subtrees = std::vector<ParseTree *>();
    // m_subtrees.reserve(4);
    for (size_t i = 0; i < 8; i++)
    {
        m_subtrees.push_back(NULL);
    }
    m_start = 0;
    m_end = 0;
    m_is_leaf = false;
    m_rule_idx = 0;
    m_rule_id = 0;
    m_size = 0;
    m_token_idx = 0;
}

ParseTree::ParseTree(ParseTree const &other) { *this = other; }

ParseTree::~ParseTree(void)
{
    for (size_t i = 0; i < m_subtrees.size(); i++)
        delete m_subtrees[ i ];
}

void ParseTree::addSubtree(int start, int end, int index,
                           const GrammarRule &rule)
{
    ParseTree *tree;
    tree = m_subtrees[ index ];
    if (!tree)
        m_subtrees[ index ] = new ParseTree();
    tree = m_subtrees[ index ];
    tree->setSpan(start, end);
    tree->ruleID(rule.ruleID());
    if (start == end)
        tree->ruleIndex(0);
    else
        tree->ruleIndex(rule.getRuleIndex());
    if (index >= m_size)
        m_size = index + 1;
}

void ParseTree::setSpan(int start, int end)
{
    m_start = start;
    m_end = end;
    if (start == end)
        m_is_leaf = true;
}

int ParseTree::start(void) const { return m_start; }

int ParseTree::end(void) const { return m_end; }

ParseTree *ParseTree::operator[](int index)
{
    if (static_cast<size_t>(index) >= m_subtrees.size())
        return NULL;
    return m_subtrees[ index ];
}

ParseTree &ParseTree::operator=(ParseTree const &other)
{
    // todo: copy the children;
    m_end = other.m_end;
    m_is_leaf = other.m_is_leaf;
    m_subtrees = other.m_subtrees;
    m_rule_id = other.m_rule_id;
    m_start = other.m_start;
    m_size = other.m_size;
    return *this;
}

bool ParseTree::leaf(void) const { return m_is_leaf; }

void ParseTree::leaf(bool value) { m_is_leaf = value; }

void ParseTree::ruleIndex(int value) { m_rule_idx = value; }

int ParseTree::ruleIndex(void) const { return m_rule_idx; }

int ParseTree::tokenIndex(void) const { return m_token_idx; }

void ParseTree::tokenIndex(int value) { m_token_idx = value; }

int ParseTree::ruleID(void) const { return m_rule_id; }

void ParseTree::ruleID(int value) { m_rule_id = value; }

size_t ParseTree::size() const { return m_size; }
