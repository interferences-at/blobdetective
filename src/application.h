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
        Configuration* get_configuration();
    private:
        Configuration _configuration;
        OscInterface *osc_interface;
        int frame_width;
        int frame_height;
        bool showWebcam;
        bool showFPS;

        void send_blob_coordinates(const std::vector<cv::KeyPoint> &keypoints);
        int get_int_option(const char* name);
        float get_float_option(const char* name);
        std::string get_string_option(const char* name);
        bool get_boolean_option(const char* name);
        cv::SimpleBlobDetector::Params detector_params_from_options();
        float convert_x_to_final_range(float value);
        float convert_y_to_final_range(float value);
};

} // end of namespace

#endif // __APPLICATION_H__

