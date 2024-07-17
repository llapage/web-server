#ifndef CONFIGURATIONBLOCK_HPP
#define CONFIGURATIONBLOCK_HPP

#include "../../includes/configuration/BlockList.hpp"
#include "../../includes/configuration/IConfiguration.hpp"
#include "../configuration/Defaults.hpp"
#include "../logger/ILogger.hpp"
#include <map>
#include <string>

class ConfigurationBlock : public IConfiguration, public BlockList
{
private:
    ILogger &m_logger;
    const std::string m_name;
    std::map<std::string, BlockList> m_blocks;
    std::map<std::string, std::vector<std::string> *> m_directives;
    std::vector<std::string> m_parameters;
    Defaults &m_defaults;
    bool m_is_regex;
    ConfigurationBlock *m_parent;

public:
    ConfigurationBlock(ILogger &logger, const std::string name,
                       Defaults &defaults);
    ConfigurationBlock(ConfigurationBlock *parent, const std::string name,
                       Defaults &defaults);
    ~ConfigurationBlock();
    virtual const BlockList &getBlocks(const std::string &key);
    virtual const std::vector<std::string> &
    getStringVector(const std::string &key) const;
    virtual const std::string &getString(const std::string &parameter,
                                         size_t index) const;
    virtual int getInt(const std::string &parameter, size_t index) const;
    virtual size_t getSize_t(const std::string &parameter, size_t index) const;
    virtual bool getBool(const std::string &parameter, size_t index) const;
    virtual void addBlock(const std::string &name, IConfiguration *block);
    std::vector<std::string> &addDirective(const std::string &name);
    virtual bool isRegex(void) const;
    virtual void isRegex(bool value);
    virtual const std::string &getName() const;
    virtual ConfigurationBlock *getParent() const;
    virtual std::vector<std::string> &getParameters(void);
    std::vector<std::string> &setParameters(void);
    void print(size_t depth) const;
};

#endif // CONFIGURATIONBLOCK_HPP
