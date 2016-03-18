#include "configuration.h"

namespace blobdetective {

Configuration::Configuration()
{
    this->video_device_id = 0;
}

bool Configuration::has_option(const char* name) const
{
    std::map<std::string, Option>::const_iterator iter = this->_options.find(std::string(name));
    if (iter == this->_options.end())
    {
        return false;
    }
    else
    {
        return true;
    }
}

Option* Configuration::get_option(const char *name)
{
    std::map<std::string, Option>::iterator iter = this->_options.find(std::string(name));
    if (iter == this->_options.end())
    {
        return NULL;
    }
    else
    {
        return & (iter->second);
    }
}

void Configuration::add_option(const char* name, Option option)
{
    this->_options.insert(std::map<std::string, Option>::value_type(std::string(name), option));
}

const std::vector<std::string> Configuration::list_options() const
{
    std::vector<std::string> ret;
    for (std::map<std::string, Option>::const_iterator item = this->_options.begin(); item != this->_options.end(); ++item)
    {
        ret.push_back(item->second.get_name());
    }
    return ret;
}

} // end of namespace
