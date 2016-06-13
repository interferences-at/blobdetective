#ifndef __DETECTOR_H__
#define __DETECTOR_H__

#include <opencv2/opencv.hpp>


namespace blobdetective {
	class Application;

	class Detector
	{

	private:

	public:
		struct Params {
			Params();
			int threshold;
			int trackLightBlob;

			int filterByArea;
			int minArea, maxArea;

			int filterByCircularity;
			int minCircularity, maxCircularity;

			int filterByInertia;
			int minInertiaRatio, maxInertiaRatio;

			int filterByConvexity;
			int minConvexity, maxConvexity;
		};

		struct  Center
		{
			cv::Point2d location;
			double radius;
			double confidence;
		};

		Detector(Params& params_, Application* parent_);
		~Detector();

		void findBlobs(const cv::Mat &image, const cv::Mat &binaryImage, std::vector<Center> &centers);
		void detect(const cv::Mat& _image, std::vector<cv::KeyPoint>& _keypoints, const cv::Mat& _mask = cv::Mat());

		Application* parent;
		Params& params;
		static const char* windowName;

	};
}

#endif

