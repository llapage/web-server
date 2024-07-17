#include "../../includes/configuration/BlockList.hpp"
#include "../../includes/configuration/ConfigurationBlock.hpp"

BlockList::BlockList() {}

BlockList::~BlockList() {}

void BlockList::setParent(ConfigurationBlock *block) { m_parent = block; }

void BlockList::setLogger(ILogger *logger) { m_logger = logger; }

IConfiguration *BlockList::operator[](size_t index) const
{
    if (index >= this->size())
        return m_parent;
    return std::vector<IConfiguration *>::operator[](index);
}

IConfiguration *BlockList::at(size_t index)
{
    try
    {
        return std::vector<IConfiguration *>::at(index);
    }
    catch (std::exception &e)
    {
        return m_parent;
    }
}
