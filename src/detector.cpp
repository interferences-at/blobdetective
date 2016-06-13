#include "detector.h"
#include "application.h"
#include <iterator>
#include <limits>

using namespace cv;

namespace blobdetective {
	const char* Detector::windowName = "Detector Settings";

	Detector::Params::Params() {
		threshold = 250;

		trackLightBlob = 1;

		filterByArea = 1;
		minArea = 25;
		maxArea = 5000;

		filterByCircularity = 1;
		minCircularity = 0;
		maxCircularity = 100;

		filterByInertia = 1;
		minInertiaRatio = 0;
		maxInertiaRatio = 100;

		filterByConvexity = 1;
		minConvexity = 0;
		maxConvexity = 100;
	}

	Detector::Detector(Params& params_, Application* parent_) :
		params(params_), parent(parent_) {

		cv::namedWindow(windowName, CV_WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
		cv::moveWindow(windowName, 750, 100);

		cv::createTrackbar("trackLightBlob", windowName, &params.trackLightBlob, 1);
		cv::createTrackbar("threshold", windowName, &params.threshold, 255);
		
		cv::createTrackbar("filterByArea", windowName, &params.filterByArea, 1);
		cv::createTrackbar("minArea", windowName, &params.minArea, 20000);
		cv::createTrackbar("maxArea", windowName, &params.maxArea, 20000);

		cv::createTrackbar("filterByCircularity", windowName, &params.filterByCircularity, 1);
		cv::createTrackbar("minCircularity", windowName, &params.minCircularity, 100);
		cv::createTrackbar("maxCircularity", windowName, &params.maxCircularity, 100);

		cv::createTrackbar("filterByConvexity", windowName, &params.filterByConvexity, 1);
		cv::createTrackbar("minConvexity", windowName, &params.minConvexity, 100);
		cv::createTrackbar("maxConvexity", windowName, &params.maxConvexity, 100);

		cv::createTrackbar("filterByInertia", windowName, &params.filterByInertia, 1);
		cv::createTrackbar("minInertiaRatio", windowName, &params.minInertiaRatio, 100);
		cv::createTrackbar("maxInertiaRatio", windowName, &params.maxInertiaRatio, 100);
		

	}

	Detector::~Detector()
	{
	}

	void Detector::findBlobs(const Mat & image, const Mat & binaryImage, vector<Center>& centers)
	{
		(void)image;
		centers.clear();

		std::vector < std::vector<Point> > contours;
		Mat tmpBinaryImage = binaryImage.clone();
		findContours(tmpBinaryImage, contours, RETR_LIST, CHAIN_APPROX_NONE);

		for (size_t contourIdx = 0; contourIdx < contours.size(); contourIdx++)
		{
			Center center;
			center.confidence = 1;
			Moments moms = moments(Mat(contours[contourIdx]));
			if (params.filterByArea)
			{
				double area = moms.m00;
				if (area < params.minArea || area >= params.maxArea)
					continue;
			}

			if (params.filterByCircularity)
			{
				double area = moms.m00;
				double perimeter = arcLength(Mat(contours[contourIdx]), true);
				double ratio = 4 * CV_PI * area / (perimeter * perimeter);
				if (ratio < params.minCircularity *0.01 || ratio >= params.maxCircularity *0.01)
					continue;
			}

			if (params.filterByInertia)
			{
				double denominator = std::sqrt(std::pow(2 * moms.mu11, 2) + std::pow(moms.mu20 - moms.mu02, 2));
				const double eps = 1e-2;
				double ratio;
				if (denominator > eps)
				{
					double cosmin = (moms.mu20 - moms.mu02) / denominator;
					double sinmin = 2 * moms.mu11 / denominator;
					double cosmax = -cosmin;
					double sinmax = -sinmin;

					double imin = 0.5 * (moms.mu20 + moms.mu02) - 0.5 * (moms.mu20 - moms.mu02) * cosmin - moms.mu11 * sinmin;
					double imax = 0.5 * (moms.mu20 + moms.mu02) - 0.5 * (moms.mu20 - moms.mu02) * cosmax - moms.mu11 * sinmax;
					ratio = imin / imax;
				}
				else
				{
					ratio = 1;
				}

				if (ratio < params.minInertiaRatio * 0.01|| ratio >= params.maxInertiaRatio * 0.01)
					continue;

				center.confidence = ratio * ratio;
			}

			if (params.filterByConvexity)
			{
				std::vector < Point > hull;
				convexHull(Mat(contours[contourIdx]), hull);
				double area = contourArea(Mat(contours[contourIdx]));
				double hullArea = contourArea(Mat(hull));
				double ratio = area / hullArea;
				if (ratio < params.minConvexity *0.01 || ratio >= params.maxConvexity*0.01)
					continue;
			}

			if (moms.m00 == 0.0)
				continue;
			center.location = Point2d(moms.m10 / moms.m00, moms.m01 / moms.m00);

			
			//compute blob radius
			{
				std::vector<double> dists;
				for (size_t pointIdx = 0; pointIdx < contours[contourIdx].size(); pointIdx++)
				{
					Point2d pt = contours[contourIdx][pointIdx];
					dists.push_back(norm(center.location - pt));
				}
				std::sort(dists.begin(), dists.end());
				center.radius = (dists[(dists.size() - 1) / 2] + dists[dists.size() / 2]) / 2.;
			}

			centers.push_back(center);
		}
	}

		
	void Detector::detect(const Mat & image, vector<KeyPoint>& keypoints, const Mat & mask)
	{
		keypoints.clear();
		Mat grayscaleImage;
		if (image.channels() == 3)
			cvtColor(image, grayscaleImage, COLOR_BGR2GRAY);
		else
			grayscaleImage = image;

		if (grayscaleImage.type() != CV_8UC1) {
			std::cerr << "Blob detector only supports 8-bit images!";
		}

		std::vector < std::vector<Center> > centers;
		
		Mat binarizedImage;
		threshold(grayscaleImage, binarizedImage, params.threshold, 255, (params.trackLightBlob) ? THRESH_BINARY: THRESH_BINARY_INV );

		std::vector < Center > curCenters;
		findBlobs(grayscaleImage, binarizedImage, curCenters);
		for (int i = 0; i < curCenters.size(); ++i) {
			KeyPoint kpt(curCenters[i].location, curCenters[i].radius * 2.0f);
			keypoints.push_back(kpt);
		}	
	}
} //end namespace