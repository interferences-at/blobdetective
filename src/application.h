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
