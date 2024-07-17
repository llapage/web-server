#ifndef REGEXMATCHER_HPP
#define REGEXMATCHER_HPP

#include "IURIMatcher.hpp"

class RegexMatcher : public IURIMatcher
{
private:
    const std::string &m_pattern;

public:
    RegexMatcher(const std::string &pattern);
    ~RegexMatcher();
    virtual bool match(const std::string &uri);
};

#endif // REGEXMATCHER_HPP
