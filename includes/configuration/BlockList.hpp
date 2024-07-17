#ifndef CONFIGURATIONLIST
#define CONFIGURATIONLIST

#include "../../includes/configuration/Defaults.hpp"
#include "../../includes/configuration/IConfiguration.hpp"
#include "../../includes/logger/ILogger.hpp"
#include <vector>

class ConfigurationBlock;

class BlockList : public std::vector<IConfiguration *>
{
private:
    ConfigurationBlock *m_parent;
    ILogger *m_logger;

public:
    BlockList();
    virtual ~BlockList();
    void setParent(ConfigurationBlock *block);
    void setLogger(ILogger *logger);
    virtual IConfiguration *operator[](size_t index) const;
    virtual IConfiguration *at(size_t index);
    // todo: an iterator that returns the parent each time it is called.
};

#endif // CONFIGURATIONLIST
