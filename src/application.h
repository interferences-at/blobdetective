#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "configuration.h"
#include "oscinterface.h"
#include "detector.h"
#include "camera.h"
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
		Detector* getDetector() { return detector; }
		Detector::Params& getDetectorParams() { return params; }
		Camera* getCamera() { return camera; }
		

    private:
        Configuration _configuration;
        OscInterface *osc_interface;
		//cv::VideoCapture* videoCapture;
		Detector* detector;
		Detector::Params params;
		Camera* camera;
		static const char* windowName;

        void send_blob_coordinates(const std::vector<cv::KeyPoint> &keypoints);
        int get_int_option(const char* name);
        float get_float_option(const char* name);
        std::string get_string_option(const char* name);
        bool get_boolean_option(const char* name);
        void detector_params_from_options();

};

} // end of namespace

#endif // __APPLICATION_H__

