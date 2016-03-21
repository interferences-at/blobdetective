#include <opencv2/opencv.hpp>
#include <iostream>
#include "application.h"

namespace blobdetective {

static std::string int_to_string(int value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

void Application::send_blob_coordinates(const std::vector<cv::KeyPoint> &keypoints)
{
    if (keypoints.size() == 0)
    {
        return;
    }
    else
    {
        unsigned int biggest_index = 0;
        float biggest_size = 0.0f;
        for (unsigned int i = 0; i < keypoints.size(); i++)
        {
            float size = keypoints[i].size;
            if (size > biggest_size)
            {
                biggest_size = size;
                biggest_index = i;
            }
        }
        float x = keypoints[biggest_index].pt.x;
        float y = keypoints[biggest_index].pt.y;
        float size = keypoints[biggest_index].size;
        // std::cout << "TODO Send /blob " << x << ", " << y << " " << size << std::endl;
        this->osc_interface->send_blob_position(x, y, size);
    }
}

Application::Application(Configuration& configuration)
{
    // First setup the configuration
    this->_configuration = configuration;

    // Setup OSC sender
    this->osc_interface = new OscInterface(
            this,
            this->get_string_option("identifier"),
            int_to_string(this->get_int_option("osc_send_port")).c_str(),
            this->get_string_option("osc_send_host"),
            int_to_string(this->get_int_option("osc_receive_port")).c_str()
            );
}


int Application::run()
{
    bool verbose = this->get_boolean_option("verbose");
    std::string video_device_id = this->get_string_option("video_device_id");

    if (verbose)
    {
        std::cout << "Opening video device " << video_device_id << std::endl;
    }

    cv::VideoCapture cap(video_device_id.c_str());
    if (! cap.isOpened())  // check if we succeeded
    {
        return 1;
    }

    cv::Mat edges;

    // Default values of parameters are tuned to extract dark circular blobs.
    cv::SimpleBlobDetector::Params params;

    // DEFAULT VALUES:
    //   thresholdStep = 10;
    //   minThreshold = 50;
    //   maxThreshold = 220;
    //   maxCentersDist = 10;
    //   defaultKeypointSize = 1;
    //   minRepeatability = 2;
    //   computeRadius = true;
    //   filterByColor = true;
    //   blobColor = 0;
    //   isGrayscaleCentroid = false;
    //   centroidROIMargin = 2;
    //
    //   filterByArea = true;
    //   minArea = 25;
    //   maxArea = 5000;
    //
    //   filterByInertia = true;
    //   minInertiaRatio = 0.1f;
    //
    //   filterByConvexity = true;
    //   minConvexity = 0.95f;
    //
    //   filterByCircularity = false;
    //   minCircularity = 0.8f;

    // Convert the source image to binary images by applying thresholding with
    // several thresholds from minThreshold (inclusive) to maxThreshold
    // (exclusive) with distance thresholdStep between neighboring thresholds
    // Thresholding : Convert the source images to several binary images by thresholding the source image with thresholds starting at minThreshold. These thresholds are incremented  by thresholdStep until maxThreshold. So the first threshold is minThreshold, the second is minThreshold + thresholdStep, the third is minThreshold + 2 x thresholdStep, and so on.
    // These are color thresholds:
    params.thresholdStep = this->get_int_option("thresholdStep");
    params.minThreshold = this->get_float_option("minThreshold");
    params.maxThreshold = this->get_float_option("maxThreshold");

    //params.maxCentersDist = this->get_int_option("maxCentersDist");

    // Group centers from several binary images by their coordinates.
    // Close centers form one group that corresponds to one blob, which is
    // controlled by the minDistBetweenBlobs parameter.
    params.minDistBetweenBlobs = this->get_float_option("minDistBetweenBlobs");

    // Extracted blobs have an area between minArea (inclusive) and
    // maxArea (exclusive).
    params.filterByArea = this->get_boolean_option("filterByArea");
    params.minArea = this->get_int_option("minArea");
    params.maxArea = this->get_int_option("maxArea");

    params.filterByCircularity = this->get_boolean_option("filterByCircularity");
    params.minCircularity = this->get_boolean_option("minCircularity");
    params.filterByConvexity = this->get_boolean_option("filterByConvexity");
    // params.defaultKeypointSize = this->get_int_option("defaultKeypointSize");
    params.minRepeatability = this->get_int_option("minRepeatability");
    // params.computeRadius = this->get_boolean_option("computeRadius");
    params.filterByColor = this->get_boolean_option("filterByColor");
    params.blobColor = this->get_int_option("blobColor");
    //params.isGrayscaleCentroid = this->get_boolean_option("isGrayscaleCentroid");
    //params.centroidROIMargin = this->get_int_option("centroidROIMargin");
    params.filterByInertia = this->get_boolean_option("filterByInertia");
    params.minInertiaRatio = this->get_float_option("minInertiaRatio");
    params.filterByConvexity = this->get_boolean_option("filterByConvexity");
    params.minConvexity = this->get_float_option("minConvexity");

    cv::SimpleBlobDetector detector(params);
    cv::namedWindow("Blobspy", 1);
    cv::Mat mirrored_img;
    std::cout << "Press any key to quit" << std::endl;

    while (true)
    {
        cv::Mat frame;
        cap >> frame; // get a new frame from camera
        if (frame.empty())
        {
            std::cerr << "Error: Cannot capture frame." << std::endl;
            return 1;
            // continue;
        }
        // Convert it to grayscale
        cv::cvtColor(frame, edges, CV_BGR2GRAY);
        cv::flip(edges, mirrored_img, 1);

        // Blur
        // cv::GaussianBlur(edges, edges, cv::Size(7, 7), 1.5, 1.5);

        // Invert colors
        cv::Mat inv_src = cv::Scalar::all(255) - mirrored_img;

        // No need to do a threshold manually, since the SimpleBlobDetector does it
        // cv::cvAdaptiveThreshold(inv_src, inv_src, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 13, 1);
        // int threshold_value = 0;
        // int const max_BINARY_value = 255;
        // //int threshold_type = 0; // 1: Binary
        // int threshold_type = 1; // 1: Binary Inverted
        // cv::threshold(inv_src, inv_src, threshold_value, max_BINARY_value,threshold_type );

        // Detect blobs.
        std::vector<cv::KeyPoint> keypoints;
        detector.detect(inv_src, keypoints);
 
        // Draw detected blobs as red circles.
        // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the
        // circle corresponds to the size of blob
        cv::Mat im_with_keypoints;
        //cv::drawKeypoints(frame, keypoints, im_with_keypoints,
        cv::drawKeypoints(inv_src, keypoints, im_with_keypoints,
                cv::Scalar(0, 0, 255),
                cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

        // Send OSC
        send_blob_coordinates(keypoints);

        // Show blobs
        cv::imshow("Blobspy", im_with_keypoints);

        if (cv::waitKey(30) >= 0)
        {
            break;
        }
        // TODO: also stop if the window has been destroyed
    }
    std::cout << "Done" << std::endl;
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

Application::~Application()
{
    delete this->osc_interface;
}

int Application::get_int_option(const char* name)
{
    return this->_configuration.get_option(name)->get_int();
}

float Application::get_float_option(const char* name)
{
    return this->_configuration.get_option(name)->get_float();
}

std::string Application::get_string_option(const char* name)
{
    return this->_configuration.get_option(name)->get_string();
}

bool Application::get_boolean_option(const char* name)
{
    return this->_configuration.get_option(name)->get_boolean();
}

Configuration* Application::get_configuration()
{
    return ( & this->_configuration);
}

} // end of namespace
