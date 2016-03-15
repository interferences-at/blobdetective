#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "configuration.h"

namespace blobspy {

class Application
{
    private:
        Configuration _configuration;
    public:
        Application(Configuration& configuration);
        int run();
};

} // end of namespace

#endif // __APPLICATION_H__
