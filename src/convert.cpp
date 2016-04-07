#include <string>
#include "convert.h"

namespace blobdetective {

/**
 * Converts a string to a bool.
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

