#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "configuration.h"
#include "oscinterface.h"

namespace blobspy {

class Application
{
    private:
        Configuration _configuration;
        // OscInterface osc_interface;
    public:
        Application(Configuration& configuration);
        int run();
};

} // end of namespace

#endif // __APPLICATION_H__
