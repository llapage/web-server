#include "../../includes/configuration/ConfigurationLoader.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include "../../includes/parsing/Grammar.hpp"
#include "../../includes/parsing/NonTerminalSymbol.hpp"
#include "../../includes/parsing/Parser.hpp"
#include "../../includes/parsing/TerminalSymbol.hpp"
#include <fstream>

// retrieves the list of strings from the parse tree.
static void get_values(const std::vector<Token> &tokens, ParseTree &parse_tree,
                       std::vector<std::string> &result)
{
    ParseTree *next = parse_tree[ 1 ];
    result.push_back(tokens[ (*parse_tree[ 0 ])[ 0 ]->tokenIndex() ].value);
    // goes down the tree branch
    while (next != NULL)
    {
        result.push_back(tokens[ (*(*next)[ 0 ])[ 0 ]->tokenIndex() ].value);
        next = (*next)[ 1 ];
    }
}

ConfigurationLoader::ConfigurationLoader(ILogger &logger) : m_logger(logger)
{
    m_separators.push_back(" ");
    m_separators.push_back("\n");
    m_separators.push_back("\t");

    m_reserved_symbols.push_back("{");
    m_reserved_symbols.push_back("}");
    m_reserved_symbols.push_back(";");
    m_reserved_symbols.push_back("~");

    m_config = new ConfigurationBlock(m_logger, "main", m_defaults);
    m_logger.log(VERBOSE, "ConfigurationLoader created.");
}

// todo: delete all the blocks.
ConfigurationLoader::~ConfigurationLoader() { delete m_config; }

void ConfigurationLoader::m_addBlock(const Grammar &grammar,
                                     const std::vector<Token> &tokens,
                                     ParseTree &parse_tree,
                                     ConfigurationBlock &block)
{
    ConfigurationBlock *new_block;
    const std::string rule_name =
        grammar.getRule(parse_tree[ 1 ]->ruleIndex())->getName();

    // check if it is a block with at block_parameters and retreive the
    // block_parameters.
    new_block = new ConfigurationBlock(
        &block, tokens[ parse_tree[ 0 ]->tokenIndex() ].value, m_defaults);
    if (rule_name == "block-parameters")
    {
        int start = 0;
        // NOTE: the ConfigurationBlock could have a regex mode...
        if (tokens[ (*parse_tree[ 1 ])[ 0 ]->tokenIndex() ].value == "~")
        {
            start = 1;
            new_block->isRegex(true);
        }
        std::vector<std::string> &params = new_block->setParameters();
        get_values(tokens, *(*parse_tree[ 1 ])[ start ], params);
        m_buildConfig(grammar, tokens, *parse_tree[ 3 ], *new_block);
    }
    else
    {
        // recursively add blocks or directives on the current block. (skipping
        // the open brace.)
        m_buildConfig(grammar, tokens, *parse_tree[ 2 ], *new_block);
    }
    block.addBlock(tokens[ parse_tree[ 0 ]->tokenIndex() ].value, new_block);
}

void ConfigurationLoader::m_addDirective(const Grammar &grammar,
                                         const std::vector<Token> &tokens,
                                         ParseTree &parse_tree,
                                         ConfigurationBlock &block)
{
    const std::string &directive =
        tokens[ (*parse_tree[ 0 ])[ 0 ]->tokenIndex() ].value;
    if (directive == "include")
    {
        // include configuration from the provided path.
        std::vector<std::string> params;
        get_values(tokens, *parse_tree[ 1 ], params);

        // open file provided in params, load it and parse it
        Tokenizer tokenizer(m_separators, m_reserved_symbols);
        Parser parser(grammar);
        std::ifstream conf_stream(params[ 0 ].c_str());
        if (!conf_stream.is_open())
        {
            m_logger.log(ERROR, "ConfigurationLoader: file " + params[ 0 ] +
                                    " not found");
            return;
        }
        try
        {
            const std::vector<Token> &new_tokens =
                tokenizer.tokenize(conf_stream);
            conf_stream.close();
            ParseTree &new_parse_tree = parser.parse(new_tokens);
            // recursively build configuration and add it to the parent of the
            // current block.
            m_buildConfig(grammar, new_tokens, new_parse_tree,
                          *block.getParent());
        }
        catch (const std::exception &e)
        {
            m_logger.log(ERROR,
                         "Couldn't include " + params[ 0 ] + " " + e.what());
        }
        return;
    }
    // the first sub-child is the directive name and the second is the
    // parameters list.
    std::vector<std::string> &params = block.addDirective(directive);
    get_values(tokens, *parse_tree[ 1 ], params);
}

void ConfigurationLoader::m_buildConfig(const Grammar &grammar,
                                        const std::vector<Token> &tokens,
                                        ParseTree &parse_tree,
                                        ConfigurationBlock &block)
{
    const GrammarRule *rule = grammar.getRule(parse_tree.ruleIndex());
    const std::string rule_name = rule->getName();
    if (rule_name == "block")
    {
        m_addBlock(grammar, tokens, parse_tree, block);
        return;
    }
    else if (rule_name == "directive")
    {
        m_addDirective(grammar, tokens, parse_tree, block);
        return;
    }
    // go down the parse tree and build sub-blocks.
    for (size_t i = 0; i < parse_tree.size(); i++)
    {
        m_buildConfig(grammar, tokens, *parse_tree[ i ], block);
    }
}

IConfiguration &ConfigurationLoader::loadConfiguration(const std::string &path)
{
    std::ifstream conf_stream(path.c_str());
    if (!conf_stream.is_open())
        throw InvalidConfigFileError();

    // Log the loading of the configuration file.
    m_logger.log(INFO, "Loading configuration file: '" + path + "'.");
    Grammar grammar;
    SubsetSymbolMatching subset_matching;
    EqualSymbolMatching equal_matching;
    DigitMatching digit_matching;
    SubStringMatching substring_matching;
    AlphabeticMatching alpha_matching;
    PrintableAsciiMatching printable_ascii_matching;

    NonTerminalSymbol configuration("configuration", 0);
    NonTerminalSymbol block_element("configuration-element", 1);
    NonTerminalSymbol block("block", 2);
    NonTerminalSymbol block_parameters("block-parameters", 3);
    NonTerminalSymbol directive("directive", 4);
    NonTerminalSymbol parameters("parameters", 5);
    NonTerminalSymbol comment_list("comment-list", 6);
    NonTerminalSymbol comment("comment", 7);
    NonTerminalSymbol value("value", 8);
    NonTerminalSymbol directive_name("directive-name", 9);
    NonTerminalSymbol text("text", 10);

    TerminalSymbol curl_open("{", 11);
    TerminalSymbol curl_close("}", 12);
    TerminalSymbol semicolon(";", 13);
    TerminalSymbol regex("~", 14);
    std::vector<std::string> bools;
    bools.push_back("on");
    bools.push_back("off");
    std::vector<std::string> empty;
    TerminalSymbolSet boolean("boolean", 15, bools, substring_matching);
    TerminalSymbolSet letters("letters", 16, empty, alpha_matching);
    TerminalSymbolSet digits("digits", 17, empty, digit_matching);
    TerminalSymbolSet string_("string", 18, empty, printable_ascii_matching);

    // configuration
    GrammarRule *rule = grammar.addRule(configuration);
    rule->addSymbol(&block_element);
    rule = grammar.addRule(configuration);
    rule->addSymbol(&block_element);
    rule->addSymbol(&configuration);

    // configuration element
    rule = grammar.addRule(block_element);
    rule->addSymbol(&directive);
    rule = grammar.addRule(block_element);
    rule->addSymbol(&block);

    // directive
    rule = grammar.addRule(directive);
    rule->addSymbol(&directive_name);
    rule->addSymbol(&parameters);
    rule->addSymbol(&semicolon);

    // block
    rule = grammar.addRule(block);
    rule->addSymbol(&string_);
    rule->addSymbol(&curl_open);
    rule->addSymbol(&configuration);
    rule->addSymbol(&curl_close);

    rule = grammar.addRule(block);
    rule->addSymbol(&string_);
    rule->addSymbol(&block_parameters);
    rule->addSymbol(&curl_open);
    rule->addSymbol(&configuration);
    rule->addSymbol(&curl_close);

    rule = grammar.addRule(block_parameters);
    rule->addSymbol(&regex);
    rule->addSymbol(&parameters);

    rule = grammar.addRule(block_parameters);
    rule->addSymbol(&parameters);

    // parameters
    rule = grammar.addRule(parameters);
    rule->addSymbol(&value);
    rule = grammar.addRule(parameters);
    rule->addSymbol(&value);
    rule->addSymbol(&parameters);

    // value
    rule = grammar.addRule(value);
    rule->addSymbol(&string_);
    rule = grammar.addRule(value);
    rule->addSymbol(&boolean);

    // directive_name
    rule = grammar.addRule(directive_name);
    rule->addSymbol(&string_);

    // text
    rule = grammar.addRule(text);
    rule->addSymbol(&string_);
    rule = grammar.addRule(text);
    rule->addSymbol(&string_);
    rule->addSymbol(&text);

    Tokenizer tokenizer(m_separators, m_reserved_symbols);
    Parser parser(grammar);
    const std::vector<Token> &tokens = tokenizer.tokenize(conf_stream);
    if (tokens.size() == 0)
        return *m_config;
    ParseTree &parse_tree = parser.parse(tokens);
    if (m_config)
        delete m_config;
    // initial block.
    m_config = new ConfigurationBlock(m_logger, "main", m_defaults);

    for (size_t i = 0; i < parse_tree.size(); i++)
    {
        m_buildConfig(grammar, tokens, *parse_tree[ i ], *m_config);
    }
    conf_stream.close();

    // Log the end of loading of the configuration file.
    m_logger.log(VERBOSE, "Configuration file loaded successfully.");
    return *m_config;
}
