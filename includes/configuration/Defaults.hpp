#ifndef DEFAULTS_HPP
#define DEFAULTS_HPP

#include <map>
#include <string>
#include <vector>

class Defaults
{
private:
    std::map<const std::string, std::vector<std::string> >
        m_directive_parameters;
    std::map<const std::string, std::vector<std::string> > m_block_parameters;

public:
    Defaults();
    ~Defaults();
    std::vector<std::string> &getBlockParameters(const std::string &key);
    std::vector<std::string> &getDirectiveParameters(const std::string &key);
    std::string getDirectiveParameter(const std::string &key, int index);
};

#endif // DEFAULTS_HPP
