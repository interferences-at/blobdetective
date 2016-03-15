#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "configuration.h"
#include <opencv2/opencv.hpp>
#include "oscinterface.h"
#include <vector>

namespace blobspy {

class Application
{
    private:
        Configuration _configuration;
        OscInterface *osc_interface;
    public:
        Application(Configuration& configuration);
        int run();
    private:
        void send_blob_coordinates(const std::vector<cv::KeyPoint> &keypoints);
};

} // end of namespace

#endif // __APPLICATION_H__
