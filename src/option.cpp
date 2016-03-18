#include "option.h"

namespace blobdetective {

Option::Option(OptionType type, const char* name)
{
    this->_type = type;
    this->_name = std::string(name);
    this->_int = 0;
    this->_float = 0.0f;
    this->_boolean = false;
    this->_string = std::string("");
}

const std::string& Option::get_name() const
{
    return this->_name;
}

int Option::get_int() const
{
    return this->_int;
}

float Option::get_float() const
{
    return this->_float;
}

const std::string& Option::get_string() const
{
    return this->_string;
}

bool Option::get_boolean() const
{
    return this->_boolean;
}

bool Option::is_type(OptionType type) const
{
    return this->_type == type;
}

void Option::set_boolean(bool value)
{
    this->_boolean = value;
}
void Option::set_float(float value)
{
    this->_float = value;
}

void Option::set_int(int value)
{
    this->_int = value;
}

void Option::set_string(const char* value)
{
    this->_string = std::string(value);
}

bool Option::is_string() const
{
    return this->_type == STRING;
}
bool Option::is_float() const
{
    return this->_type == FLOAT;
}
bool Option::is_int() const
{
    return this->_type == INT;
}
bool Option::is_boolean() const
{
    return this->_type == BOOLEAN;
}

} // end of namespace
