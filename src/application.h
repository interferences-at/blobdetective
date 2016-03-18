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
        int get_int_option(const char* name);
        float get_float_option(const char* name);
        std::string get_string_option(const char* name);
        bool get_boolean_option(const char* name);
};

} // end of namespace

#endif // __APPLICATION_H__
