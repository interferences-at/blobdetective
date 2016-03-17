#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "configuration.h"
#include "oscinterface.h"
#include <opencv2/opencv.hpp>
#include <vector>

namespace blobdetective {

class Application
{
    public:
        Application(Configuration& configuration);
        ~Application();
        int run();
    private:
        Configuration _configuration;
        OscInterface *osc_interface;
        void send_blob_coordinates(const std::vector<cv::KeyPoint> &keypoints);
};

} // end of namespace

#endif // __APPLICATION_H__
