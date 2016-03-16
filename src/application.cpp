#include <opencv2/opencv.hpp>
#include <iostream>
#include "application.h"

namespace blobspy {

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
    this->_configuration = configuration;
    this->osc_interface = new OscInterface(
            std::string("default"), // FIXME
            std::string("31340"), // FIXME
            // std::string("127.0.0.1")); // FIXME
            std::string("192.168.1.6")); // FIXME
}

int Application::run()
{
    cv::VideoCapture cap(this->_configuration.video_device_id);
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
    params.minThreshold = 0.0; 
    params.maxThreshold = 50.0;

    // Group centers from several binary images by their coordinates.
    // Close centers form one group that corresponds to one blob, which is
    // controlled by the minDistBetweenBlobs parameter.
    params.minDistBetweenBlobs = 10.0;

    // Extracted blobs have an area between minArea (inclusive) and
    // maxArea (exclusive).
    params.filterByArea = true;
    params.minArea = 5000;
    params.maxArea = 50000; 

    // Filter by Circularity
    params.filterByCircularity = false;
    // params.minCircularity = 0.2;

    params.filterByConvexity = false;

    cv::SimpleBlobDetector detector(params);
    cv::namedWindow("Blobspy", 1);
    std::cout << "Press any key to quit" << std::endl;

    while (true)
    {
        cv::Mat frame;
        cap >> frame; // get a new frame from camera
        // Convert it to grayscale
        cv::cvtColor(frame, edges, CV_BGR2GRAY);

        // Blur
        // cv::GaussianBlur(edges, edges, cv::Size(7, 7), 1.5, 1.5);

        // Invert colors
        cv::Mat inv_src = cv::Scalar::all(255) - edges;

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

} // end of namespace
