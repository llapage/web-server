#include "../../includes/parsing/Parser.hpp"
#include <iostream>

/*The Parser is responsible for building the ParseTree*/

void print_chart(std::vector<std::vector<EarleyEdge> > &chart,
                 Grammar const &m_grammar)
{
    GrammarRule const *rule;
    EarleyEdge *edge;

    for (size_t i = 0; i < chart.size(); i++)
    {
        std::cout << "==========="
                  << "(" << i << ")"
                  << "===========" << std::endl
                  << std::endl;
        for (size_t j = 0; j < chart[ i ].size(); j++)
        {
            edge = &chart[ i ][ j ];
            rule = m_grammar.getRule(edge->ruleIndex());
            std::cout << rule->getName() << " -> ";
            for (size_t k = 0; k < rule->size(); k++)
            {
                std::cout << rule->getSymbol(k)->getValue() << " ";
            }
            std::cout << "(" << edge->end() << ")" << std::endl;
        }
        std::cout << std::endl;
    }
}

static void print_parse_tree(ParseTree &parse_tree, int depth,
                             const Grammar &grammar,
                             std::vector<Token> const &tokens)
{
    // Print the current node's value with indentation based on depth
    if (parse_tree.leaf())
    {
        for (int i = 0; i < depth; i++)
        {
            std::cout << "  |";
        }
        std::cout << tokens[ parse_tree.tokenIndex() ].value << std::endl;
        return;
    }
    for (int i = 0; i < depth; i++)
    {
        std::cout << "  |";
    }
    std::cout << grammar.getRule(parse_tree.ruleIndex())->getName()
              << std::endl;
    // Recursively print children
    for (size_t i = 0; i < parse_tree.size(); ++i)
    {
        print_parse_tree(*parse_tree[ i ], depth + 1, grammar, tokens);
    }
}

// inverts the earley sets and turns them into a graph, that will be used to
// guide the parsing tree building process.
static void buildChart(std::vector<std::vector<EarleyEdge> > &chart,
                       std::vector<std::vector<EarleyItem> > &sets)
{
    EarleyItem *item;

    for (size_t i = 0; i < sets.size(); i++)
    {
        chart.push_back(std::vector<EarleyEdge>());
    }
    for (size_t i = 0; i < sets.size(); i++)
    {
        for (size_t j = 0; j < sets[ i ].size(); j++)
        {
            item = &sets[ i ][ j ];
            if (item->completed())
                chart[ item->start() ].push_back(EarleyEdge(*item, i));
        }
    }
}

// looks for the longest edge and sets it to be the starting point of our tree.
static void setRootNode(ParseTree &tree, const Grammar &grammar,
                        std::vector<std::vector<EarleyEdge> > &chart)
{
    EarleyEdge *longest = &chart[ 0 ][ 0 ];

    for (size_t i = 0; i < chart[ 0 ].size(); i++)
    {
        if (chart[ 0 ][ i ].end() >= longest->end())
        {
            longest = &chart[ 0 ][ i ];
        }
    }
    tree.ruleID(grammar.getRule(longest->ruleIndex())->ruleID());
    tree.setSpan(longest->start(), longest->end());
}

Parser::Parser(Grammar const &grammar) : m_grammar(grammar)
{
    m_earley_sets = std::vector<std::vector<EarleyItem> >();
    m_chart = std::vector<std::vector<EarleyEdge> >();
    m_recognizer = Recognizer();
    m_parse_tree = new ParseTree();
}

Parser::~Parser() { delete m_parse_tree; }

// attempts to match the current symbol with the token
bool Parser::m_processTerminal(ParseTree &parse_tree, SearchState state,
                               const AGrammarSymbol &symbol,
                               const std::vector<Token> &tokens)
{
    if (state.node >= parse_tree.end())
        return false;
    if (!symbol.match(tokens[ state.node ]))
        return false;
    parse_tree.addSubtree(state.node, state.node, state.depth,
                          *m_grammar.getRule(state.rule_index));
    parse_tree[ state.depth ]->tokenIndex(state.node);
    // advance one step in the symbols and the tokens then continue the search
    // (the symbol can be either at the end or in-between 2 non-terminal
    // symbols).
    return m_searchPath(
        parse_tree,
        SearchState(state.rule_index, state.depth + 1, state.node + 1), tokens);
}

bool Parser::m_searchPath(ParseTree &parse_tree, SearchState state,
                          const std::vector<Token> &tokens)
{
    AGrammarSymbol *symbol;

    symbol = m_grammar.getRule(state.rule_index)->getSymbol(state.depth);
    // reached the end of the rules
    if (!symbol)
        return true;
    if (symbol->terminal())
        return m_processTerminal(parse_tree, state, *symbol, tokens);
    std::vector<EarleyEdge> &edges = m_chart[ state.node ];
    // for each edge, perform searches for rules that match the symbol.
    for (size_t i = 0; i < edges.size(); i++)
    {
        if (m_grammar.getRule(edges[ i ].ruleIndex())->ruleID() !=
            symbol->ruleID())
            continue;
        // advance one step in the rule symbols and perform a new search.
        SearchState new_state(state.rule_index, state.depth + 1,
                              edges[ i ].end());
        // if the search is successful, add the subtree.
        if (m_searchPath(parse_tree, new_state, tokens))
        {
            parse_tree.addSubtree(state.node, new_state.node, state.depth,
                                  *m_grammar.getRule(edges[ i ].ruleIndex()));
        }
        // sub-parse was completed
        // if (new_state.node >= parse_tree.end()) {
        // 	return true;
        // }
    }
    return true;
}

// recursively builds the parse tree using depth-first search.
void Parser::m_buildTree(ParseTree &parse_tree,
                         const std::vector<Token> &tokens)
{
    std::vector<EarleyEdge> &edges = m_chart[ parse_tree.start() ];
    EarleyEdge *edge;

    // reached leaf
    if (parse_tree.leaf())
        return;
    for (size_t i = 0; i < edges.size(); i++)
    {
        edge = &edges[ i ];
        if (edge->end() == parse_tree.end() &&
            m_grammar.getRule(edge->ruleIndex())->ruleID() ==
                parse_tree.ruleID())
        {
            parse_tree.ruleIndex(edge->ruleIndex());
            // if a successful path was found, stop search.
            if (m_searchPath(
                    parse_tree,
                    SearchState(edge->ruleIndex(), 0, parse_tree.start()),
                    tokens))
                break;
        }
    }
    // recursively perform searches on subtrees...
    for (size_t i = 0; i < parse_tree.size(); i++)
    {
        m_buildTree(*parse_tree[ i ], tokens);
    }
}

ParseTree &Parser::parse(std::vector<Token> const &tokens)
{
    m_recognizer.recognize(tokens, m_grammar, m_earley_sets);
    buildChart(m_chart, m_earley_sets);
    setRootNode(*m_parse_tree, m_grammar, m_chart);
    m_buildTree(*m_parse_tree, tokens);
    return *m_parse_tree;
}

void Parser::print(std::vector<Token> const &tokens)
{
    m_recognizer.print(m_grammar, m_earley_sets);
    print_parse_tree(*m_parse_tree, 0, m_grammar, tokens);
}
