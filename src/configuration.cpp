#include "configuration.h"

namespace blobdetective {

Configuration::Configuration()
{
}

bool Configuration::has_option(const char* name) const
{
    bool ret = false;
    // this->_mutex.lock();
    std::map<std::string, Option>::const_iterator iter =
            this->_options.find(std::string(name));
    if (iter == this->_options.end())
    {
        ret = false;
    }
    else
    {
        ret = true;
    }
    // this->_mutex.unlock();
    return ret;
}

const Option* Configuration::get_option_const(const char *name) const
{
    const Option* ret = NULL;
    // this->_mutex.lock();
    std::map<std::string, Option>::const_iterator iter =
            this->_options.find(std::string(name));
    if (iter == this->_options.end())
    {
        ret = NULL;
    }
    else
    {
        ret = & (iter->second);
    }
    // this->_mutex.unlock();
    return ret;
}

Option* Configuration::get_option(const char *name)
{
    // this->_mutex.lock();
    Option* ret = NULL;
    std::map<std::string, Option>::iterator iter = this->_options.find(
            std::string(name));
    if (iter == this->_options.end())
    {
        ret = NULL;
    }
    else
    {
        ret = & (iter->second);
    }
    // this->_mutex.unlock();
    return ret;
}

void Configuration::add_option(const char* name, Option option)
{
    // this->_mutex.lock();
    this->_options.insert(std::map<std::string,
            Option>::value_type(std::string(name), option));
    // this->_mutex.unlock();
}

const std::vector<std::string> Configuration::list_options() const
{
    std::vector<std::string> ret;
    // this->_mutex.lock();
    for (std::map<std::string, Option>::const_iterator item =
            this->_options.begin(); item != this->_options.end(); ++item)
    {
        ret.push_back(item->second.get_name());
    }
    // this->_mutex.unlock();
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
