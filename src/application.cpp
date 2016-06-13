#include <opencv2/opencv.hpp>
#include <iostream>
#include "application.h"
#include "convert.h"
#include <chrono>
#include <string>


namespace blobdetective {
	const char* Application::windowName = "Blob Detective";

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
			float convert = (camera->getResolution()) ? 1.0f : 2.0f;
			float x = keypoints[biggest_index].pt.x * convert;
			float y = keypoints[biggest_index].pt.y * convert;

			float size = keypoints[biggest_index].size * convert * convert;

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
			this->get_int_option("identifier"),
			int_to_string(this->get_int_option("osc_send_port")).c_str(),
			this->get_string_option("osc_send_host"),
			int_to_string(this->get_int_option("osc_receive_port")).c_str()
		);
	}


	int Application::run()
	{
		bool verbose = this->get_boolean_option("verbose");
		std::string video_device_id = this->get_string_option("video_device_id");
		int video_device_index = -1;
		try
		{
			video_device_index = from_string<int>(video_device_id);
		}
		catch (const std::runtime_error &e)
		{
			if (verbose)
			{
				std::cerr << "Error converting string to int: "
					<< e.what() << std::endl;
			}
		}

		if (verbose)
		{
			std::cout << "video_device_index: " << video_device_index << std::endl;
			std::cout << "Opening video device " << video_device_id << "..."
				<< std::endl;
		}

		camera = new Camera(this);
		this->detector_params_from_options();
		detector = new Detector(params, this);

		bool succeed;
		if (video_device_index == -1)
		{
			succeed = camera->start(video_device_id.c_str());
		}
		else
		{
			succeed = camera->start(video_device_index);
		}
	
		if (succeed)  // check if we succeeded
		{
			camera->createSettingWindow();
			camera->setResolution(0, camera);
			cv::setTrackbarPos("fps", Camera::windowName, 2);

			
			// Query camera properties:
			if (verbose)
			{/*
				std::cout << "Success opening video device." << std::endl;
				std::cout << "CV_CAP_PROP_FRAME_WIDTH: "
					<< videoCapture->get(CV_CAP_PROP_FRAME_WIDTH) << std::endl;
				std::cout << "CV_CAP_PROP_FRAME_HEIGHT: "
					<< videoCapture->get(CV_CAP_PROP_FRAME_HEIGHT) << std::endl;
				std::cout << "This is likely to cause an error message:" << std::endl;
				std::cout << "(but it's just a property that is not in this version of OpenCV)" << std::endl;
				std::cout << "CV_CAP_PROP_FPS: "
					<< videoCapture->get(CV_CAP_PROP_FPS) << std::endl;*/
			}
		}
		else
		{
			std::cerr << "Error: Could not open video device " << video_device_id
				<< std::endl;
			return 1;
		}

		
		cv::Mat frame;
		cv::Mat grayscale_img;
		cv::Mat mirrored_img;

		
		cv::namedWindow(windowName, CV_WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
		cv::moveWindow(windowName, 100, 50);

		std::cout << "Press Q to quit" << std::endl;
		std::cout << "Press S to toggle webcam display" << std::endl;
		std::cout << "Press D to toggle debug mode" << std::endl;
		std::cout << "Press R to toggle camera resolution" << std::endl;



		bool showWebcam = true;
		bool showDebug = false;
		bool toggleRes = true;

		int bugCounter = 0;
		int frameCounter = 0;
		bool isBugging = false;

		std::chrono::time_point<std::chrono::high_resolution_clock>  programStart, start, end;
		start, programStart = std::chrono::high_resolution_clock::now();
		std::cout.setf(std::ios::fixed, std::ios::floatfield);
		std::cout.precision(2);

		
		while (true)
		{
			// get a new frame from camera
			if (!camera->getFrame(frame))
			{
				std::cerr << "Error: Cannot capture frame." << std::endl;
				return 1;
				// continue;
			}

			std::vector<cv::KeyPoint> keypoints;

			// Convert it to grayscale
			cv::cvtColor(frame, grayscale_img, CV_BGR2GRAY);

			//Detect blobs
			detector->detect(grayscale_img, keypoints);

		

			// Invert X coordinate of detected blobs
			for (int i = 0; i < keypoints.size(); ++i)
			{
				keypoints[i].pt.x = (camera->getWidth() - 1) - keypoints[i].pt.x;
			}

			// Send blob data via OSC
			send_blob_coordinates(keypoints);

			// Calculate FPS
			end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> diff = end - start;
			double fps_real = 1 / diff.count();
			diff = end - programStart;
			double elapsedTime = diff.count();
			start = end;

			// Check for fps drop
			if (fps_real < 15 * (1 << camera->getFPS()) * 0.8) {
				++frameCounter;
				if (frameCounter > 10 && !isBugging) {
					++bugCounter;
					isBugging = true;
				}
			}
			else {
				frameCounter = 0;
				isBugging = false;
			}

			// Show data in terminal. Can be disabled
			if (showDebug)
			{
				std::cout << fps_real << " FPS;";
				std::cout << ";  Keypoints: " << keypoints.size();
				std::cout << ";  Bugs: " << bugCounter;
				std::cout << ";  Time: " << elapsedTime;
				std::cout << std::endl;
			}

			// Show camera display and blobs (red circles). Can be disabled for performance
			if (showWebcam)
			{
				cv::flip(grayscale_img, mirrored_img, 1);
				cv::Mat im_with_keypoints;
				// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the
				// circle corresponds to the size of blob
				cv::drawKeypoints(mirrored_img, keypoints,
					im_with_keypoints,
					cv::Scalar(0, 0, 255),
					cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
				std::string fps_string = std::to_string(fps_real) + " FPS";
				cv::putText(im_with_keypoints, fps_string, cv::Point(0, camera->getHeight() - 1), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255));
				cv::imshow(windowName, im_with_keypoints);
			}

			// if you don't call waitKey, HighGui cannot process windows events
			// like redraw, resizing, input event etc. So just call it, even
			// with a 1ms delay
			char key;
			if (key = cv::waitKey(1))
			{
				if (key == 'q') // press 'q' to quit
				{
					break;
				}
				else if (key == 's')
				{
					showWebcam = !showWebcam;
				}
				else if (key == 'd')
				{
					showDebug = !showDebug;
				}
				else if (key == 'r') {
					cv::setTrackbarPos("resolution", Camera::windowName, !camera->getResolution());
				}
			}
			// TODO: also stop if the window has been destroyed
		} // end of while loop

		delete camera;

		std::cout << "Done" << std::endl;
		return 0;
	}

	void Application::detector_params_from_options()
	{
		params.trackLightBlob = this->get_int_option("trackLightBlob");
		params.threshold = this->get_int_option("threshold");

		params.filterByArea = this->get_int_option("filterByArea");
		params.minArea = this->get_int_option("minArea");		
		params.maxArea = this->get_int_option("maxArea");

		params.filterByCircularity = this->get_int_option("filterByCircularity");
		params.minCircularity = this->get_int_option("minCircularity");
		params.maxCircularity = this->get_int_option("maxCircularity");

		params.filterByInertia = this->get_int_option("filterByInertia");
		params.minInertiaRatio = this->get_int_option("minInertiaRatio");
		params.maxInertiaRatio = this->get_int_option("maxInertiaRatio");

		params.filterByConvexity = this->get_int_option("filterByConvexity");
		params.minConvexity = this->get_int_option("minConvexity");
		params.maxConvexity = this->get_int_option("maxConvexity");
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
		return (&this->_configuration);
	}

} // end of namespace

