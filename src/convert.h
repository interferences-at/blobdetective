#ifndef __CONVERT_H__
#define __CONVERT_H__

#include <sstream>
#include <stdexcept>

namespace blobdetective {

/**
 * Converts a string to an int.
 */
template<typename T>
T from_string(const std::string& str)
{
    std::istringstream ss(str);
    T ret;
    if ((ss >> ret).fail())
    {
        throw std::runtime_error(str);
    }
    return ret;
}


/**
 * Converts a string to an bool.
 */
template<>
bool from_string<bool>(const std::string& str)
{
    bool ret = false;
    std::string true_string = "true";
    std::string yes_string = "yes";
    std::string one_string = "1";

    if (str.find(true_string) != std::string::npos)
    {
        ret = true;
    }
    else if (str.find(yes_string) != std::string::npos)
    {
        ret = true;
    }
    else if (str.find(one_string) != std::string::npos)
    {
        ret = true;
    }
    return ret;
}


} // end of namespace

#endif // include guard

