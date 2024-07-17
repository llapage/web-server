#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <vector>

struct Token
{
    const std::string value;
    bool reserved;
    Token(const std::string value, bool reserved);
    Token &operator=(const Token &other);
};

class Tokenizer
{
private:
    const std::vector<std::string> m_separators;
    const std::vector<std::string> m_reserved_symbols;
    std::vector<Token> m_tokens;
    void m_makeTokens(const std::string &str);

public:
    Tokenizer(const std::vector<std::string> separators,
              const std::vector<std::string> reserved_symbols);
    ~Tokenizer();
    std::vector<Token> const &tokenize(std::ifstream &stream);
    std::vector<Token> const &tokenize(const std::string &str);
};

#endif
