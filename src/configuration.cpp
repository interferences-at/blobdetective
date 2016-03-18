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

const Option* Configuration::get_option_const(const char *name) const // FIXME: can we use const here?
{
    std::map<std::string, Option>::const_iterator iter = this->_options.find(std::string(name));
    if (iter == this->_options.end())
    {
        return NULL;
    }
    else
    {
        return & (iter->second);
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

void Configuration::add_option_boolean(const char* name, bool value)
{
    Option option = Option(BOOLEAN, name);
    option.set_boolean(value);
    this->add_option(name, option);
}

void Configuration::add_option_int(const char* name, int value)
{
    Option option = Option(INT, name);
    option.set_int(value);
    this->add_option(name, option);
}

void Configuration::add_option_float(const char* name, float value)
{
    Option option = Option(FLOAT, name);
    option.set_float(value);
    this->add_option(name, option);
}

void Configuration::add_option_string(const char* name, const char* value)
{
    Option option = Option(STRING, name);
    option.set_string(value);
    this->add_option(name, option);
}

std::ostream &operator<<(std::ostream &os, const Configuration &configuration)
{
    os << "";
    const std::vector<std::string> names = configuration.list_options();

    std::vector<std::string>::const_iterator iter;
    for (iter = names.begin(); iter < names.end(); ++iter)
    {
        std::string name = (*iter);
        const Option *option = configuration.get_option_const(name.c_str());
        os << "-o " << (*option) << std::endl;
    }
    return os;
}

} // end of namespace
