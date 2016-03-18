#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <string>
#include <map>
#include <vector>
#include "option.h"

namespace blobdetective {

struct Configuration
{
    public:
        Configuration();
        int video_device_id;
        bool has_option(const char* name) const;
        Option* get_option(const char *name);
        const std::vector<std::string> list_options() const;
        void add_option(const char* name, Option option);
    private:
        std::map<std::string, Option> _options;
};

} // end of namespace

#endif // __CONFIGURATION_H__
