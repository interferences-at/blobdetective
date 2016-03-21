#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <string>
#include <iostream>
#include <sstream>
#include <ostream>
#include <map>
#include <vector>
// #include <mutex>
#include "option.h"

namespace blobdetective {

struct Configuration
{
    public:
        Configuration();
        int video_device_id;
        bool has_option(const char* name) const;
        Option* get_option(const char *name);
        const Option* get_option_const(const char *name) const;
        const std::vector<std::string> list_options() const;
        void add_option(const char* name, Option option);

        void add_option_boolean(const char* name, bool value);
        void add_option_int(const char* name, int value);
        void add_option_float(const char* name, float value);
        void add_option_string(const char* name, const char* value);
    private:
        std::map<std::string, Option> _options;
        //std::mutex _mutex;
};

std::ostream &operator<<(std::ostream &os, const Configuration &configuration);

} // end of namespace

#endif // __CONFIGURATION_H__
