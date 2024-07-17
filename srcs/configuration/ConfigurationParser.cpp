#include "../../includes/configuration/ConfigurationParser.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include "../../includes/parsing/Grammar.hpp"
#include "../../includes/parsing/NonTerminalSymbol.hpp"
#include "../../includes/parsing/Parser.hpp"
#include "../../includes/parsing/TerminalSymbol.hpp"
#include <fstream>

ConfigurationParser::ConfigurationParser()
{
    m_separators.push_back(" ");
    m_separators.push_back("\n");
    m_separators.push_back("\t");

    m_reserved_symbols.push_back("#");
    m_reserved_symbols.push_back("{");
    m_reserved_symbols.push_back("}");
    m_reserved_symbols.push_back(";");
    m_reserved_symbols.push_back("~");
}

ConfigurationParser::~ConfigurationParser() {}

// returns a pair with the first element being the tokens and second element
// being the parse
Pair<const std::vector<Token> &, ParseTree &>
ConfigurationParser::parse(const std::string &file_path, const Grammar &grammar)
{
    std::ifstream conf_stream(file_path.c_str());
    if (!conf_stream.is_open())
        throw InvalidConfigFileError();
    Tokenizer tokenizer(m_separators, m_reserved_symbols);
    Parser parser(grammar);
    const std::vector<Token> &tokens = tokenizer.tokenize(conf_stream);
    conf_stream.close();
    return Pair<const std::vector<Token> &, ParseTree &>(tokens,
                                                         parser.parse(tokens));
}
