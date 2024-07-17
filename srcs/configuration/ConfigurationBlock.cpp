#include "../../includes/configuration/ConfigurationBlock.hpp"
#include "../../includes/utils/Converter.hpp"
#include <iomanip>
#include <iostream>

ConfigurationBlock::ConfigurationBlock(ILogger &logger, const std::string name,
                                       Defaults &defaults)
    : m_logger(logger), m_name(name), m_defaults(defaults), m_is_regex(false)
{
    this->push_back(this);
    m_parent = NULL;
}

ConfigurationBlock::ConfigurationBlock(ConfigurationBlock *parent,
                                       const std::string name,
                                       Defaults &defaults)
    : m_logger(parent->m_logger), m_name(name), m_defaults(defaults),
      m_is_regex(false)
{
    this->push_back(this);
    m_parent = parent;
}

ConfigurationBlock::~ConfigurationBlock()
{
    for (std::map<std::string, std::vector<std::string> *>::const_iterator it =
             m_directives.begin();
         it != m_directives.end(); ++it)
    {
        delete it->second;
    }
    for (std::map<std::string, BlockList>::iterator it = m_blocks.begin();
         it != m_blocks.end(); ++it)
    {
        for (size_t i = 0; i < it->second.size(); i++)
        {
            delete it->second[ i ];
        }
    }
}

const BlockList &ConfigurationBlock::getBlocks(const std::string &key)
{
    try
    {
        return m_blocks.at(key);
    }
    catch (std::exception &e)
    {
        m_logger.log(DEBUG, "ConfigurationBlock::getBlocks: " + key +
                                " not found using default");
        BlockList *blk = &m_blocks[ key ];
        blk->push_back(new ConfigurationBlock(this, key, m_defaults));
        return *blk;
    }
}

const std::vector<std::string> &
ConfigurationBlock::getStringVector(const std::string &key) const
{
    try
    {
        return *m_directives.at(key);
    }
    catch (const std::exception &e)
    {
        m_logger.log(DEBUG, "ConfigurationBlock::getString: " + key +
                                " not found using default");
        return m_defaults.getDirectiveParameters(key);
    }
}

const std::string &ConfigurationBlock::getString(const std::string &key,
                                                 size_t index = 0) const
{
    try
    {
        return m_directives.at(key)->at(index);
    }
    catch (std::exception &e)
    {
        const std::string &res =
            m_defaults.getDirectiveParameters(key)[ index ];
        m_logger.log(DEBUG, "ConfigurationBlock::getString: " + key + "[" +
                                Converter::toString(index) +
                                "] not found defaulting to " + res);
        return res;
    }
}

int ConfigurationBlock::getInt(const std::string &key, size_t index = 0) const
{
    try
    {
        const std::string &value_str = this->getString(key, index);
        return Converter::toInt(value_str);
    }
    catch (const std::out_of_range &e)
    {
        m_logger.log(DEBUG,
                     "ConfigurationBlock::getInt: " + key + " not found");
    }
    catch (const std::invalid_argument &e)
    {
        m_logger.log(DEBUG, "ConfigurationBlock::getInt: " + key + " " +
                                m_directives.at(key)->at(index) +
                                " not an integer");
    }
    return Converter::toInt(m_defaults.getDirectiveParameters(key)[ index ]);
}

size_t ConfigurationBlock::getSize_t(const std::string &key,
                                     size_t index = 0) const
{
    try
    {
        return Converter::toUInt(m_directives.at(key)->at(index));
    }
    catch (const std::out_of_range &e)
    {
        m_logger.log(DEBUG,
                     "ConfigurationBlock::getSize_t: " + key + " not found");
    }
    catch (const std::invalid_argument &e)
    {
        m_logger.log(DEBUG, "ConfigurationBlock::getSize_t: " + key + " " +
                                m_directives.at(key)->at(index) +
                                " not an unsigned long");
    }
    return Converter::toUInt(m_defaults.getDirectiveParameters(key)[ index ]);
}

bool ConfigurationBlock::getBool(const std::string &key, size_t index = 0) const
{
    try
    {
        std::string &value = m_directives.at(key)->at(index);
        if (value == "on")
            return true;
        else if (value == "off")
            return false;
        else
        {
            throw std::invalid_argument("not a bool");
        }
    }
    catch (const std::out_of_range &e)
    {
        m_logger.log(DEBUG,
                     "ConfigurationBlock::getBool: " + key + " not found");
    }
    catch (const std::invalid_argument &e)
    {
        m_logger.log(DEBUG, "ConfigurationBlock::getBool: " + key + " " +
                                m_directives.at(key)->at(index) +
                                " not a bool");
    }
    return false;
}

void ConfigurationBlock::addBlock(const std::string &name,
                                  IConfiguration *block)
{
    BlockList *blc = &m_blocks[ name ];
    blc->push_back(block);
    blc->setLogger(&m_logger);
    blc->setParent(this);
}

std::vector<std::string> &
ConfigurationBlock::addDirective(const std::string &name)
{
    if (m_directives.find(name) == m_directives.end())
    {
        m_directives[ name ] = new std::vector<std::string>();
    }
    return *m_directives[ name ];
}

void ConfigurationBlock::print(size_t depth = 0) const
{
    for (std::map<std::string, std::vector<std::string> *>::const_iterator it =
             m_directives.begin();
         it != m_directives.end(); ++it)
    {
        std::cout << std::setw(depth + 2) << "• " << it->first << " ";
        for (size_t i = 0; i < it->second->size(); i++)
        {
            std::cout << (*it->second)[ i ] << " ";
        }
        std::cout << std::endl;
    }
    for (std::map<std::string, BlockList>::const_iterator it = m_blocks.begin();
         it != m_blocks.end(); ++it)
    {
        for (size_t i = 0; i < it->second.size(); i++)
        {
            std::cout << std::setw(depth + 2) << "• " << it->first << " ";
            std::vector<std::string> &params = it->second[ i ]->getParameters();
            for (size_t j = 0; j < params.size(); j++)
            {
                std::cout << params[ j ] << " ";
            }
            std::cout << std::endl;
            it->second[ i ]->print(depth + 4);
        }
        std::cout << std::endl;
    }
}

std::vector<std::string> &ConfigurationBlock::getParameters(void)
{
    if (m_parameters.size() == 0)
    {
        return m_defaults.getBlockParameters(m_name);
    }
    return m_parameters;
}

std::vector<std::string> &ConfigurationBlock::setParameters(void)
{
    return m_parameters;
}

const std::string &ConfigurationBlock::getName(void) const { return m_name; }
ConfigurationBlock *ConfigurationBlock::getParent(void) const
{
    return m_parent;
}

bool ConfigurationBlock::isRegex(void) const { return m_is_regex; }
void ConfigurationBlock::isRegex(bool value) { m_is_regex = value; };
