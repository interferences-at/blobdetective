#include "camera.h"
#include "application.h"

using namespace cv;

namespace blobdetective {

	const char* Camera::windowName = "Camera Settings";

	Camera::Camera(Application* parent_)
	{
		parent = parent_;
		brightness = 0;
		contrast = 0;
		exposure = 0;
		gain = 0;
	}


	Camera::~Camera()
	{
		delete videoCapture;
	}

	bool Camera::start(int index) {
		videoCapture = new cv::VideoCapture(index);
		return videoCapture->isOpened();
	}

	bool Camera::start(const char * ID)
	{
		videoCapture = new cv::VideoCapture(ID);
		return videoCapture->isOpened();
	}

	int Camera::getFPS()
	{
		return fps;
	}


	void Camera::setFPS(int fps_, void * ptr)
	{

		Camera * cam = static_cast<Camera*>(ptr);
		cam->fps = fps_;
		cam->videoCapture->set(CV_CAP_PROP_FPS, (15 * (1 << fps_)));
	}

	void Camera::setResolution(int resolution_, void * ptr)
	{
		Camera * cam = static_cast<Camera*>(ptr);

		cam->resolution = resolution_;
		cam->videoCapture->set(CV_CAP_PROP_FRAME_WIDTH, (cam->resolution) ? 640 : 320);
		cam->videoCapture->set(CV_CAP_PROP_FRAME_HEIGHT, (cam->resolution) ? 480 : 240);
		int& minArea = cam->parent->getDetectorParams().minArea;
		int& maxArea = cam->parent->getDetectorParams().maxArea;
		//minArea = (cam->resolution) ? minArea << 2 : minArea >> 2;
		//maxArea = (cam->resolution) ? maxArea << 2 : maxArea >> 2;
		//setTrackbarPos("fps", windowName, cam->fps);
		setTrackbarPos("minArea", Detector::windowName, (cam->resolution) ? minArea << 2: minArea >> 2);
		setTrackbarPos("maxArea", Detector::windowName, (cam->resolution) ? maxArea << 2 : maxArea >> 2);
		cam->setFPS(cam->fps, cam);

	}
	void Camera::setBrightness(int brightness_, void * ptr)
	{
		Camera * cam = static_cast<Camera*>(ptr);
		cam->brightness = brightness_;
		cam->videoCapture->set(CV_CAP_PROP_BRIGHTNESS, cam->brightness / 100.0);
	}

	void Camera::setContrast(int contrast_, void * ptr)
	{
		Camera * cam = static_cast<Camera*>(ptr);
		cam->contrast = contrast_;
		cam->videoCapture->set(CV_CAP_PROP_CONTRAST, cam->contrast / 100.0);
	}

	void Camera::setExposure(int exposure_, void * ptr)
	{
		Camera * cam = static_cast<Camera*>(ptr);
		cam->exposure = exposure_;
		cam->videoCapture->set(CV_CAP_PROP_EXPOSURE, cam->exposure / 100.0);
	}

	void Camera::setGain(int gain_, void * ptr)
	{
		Camera * cam = static_cast<Camera*>(ptr);
		cam->gain = gain_;
		cam->videoCapture->set(CV_CAP_PROP_GAIN, cam->gain / 100.0);
	}

	void Camera::createSettingWindow()
	{
		cv::namedWindow(windowName, CV_WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
		cv::moveWindow(windowName, 1200, 50);

		cv::createTrackbar("resolution", windowName, &resolution, 1, setResolution, this);
		cv::createTrackbar("fps", windowName, &fps, 4, setFPS, this);
		cv::createTrackbar("brightness", windowName, &brightness, 100, setBrightness, this);
		cv::createTrackbar("contrast", windowName, &contrast, 100, setContrast, this);
		cv::createTrackbar("exposure", windowName, &exposure, 100, setExposure, this);
		cv::createTrackbar("gain", windowName, &gain, 100, setGain, this);
	}

	bool Camera::getFrame(cv::Mat& frame)
	{
		return videoCapture->read(frame);
	}

	int Camera::getWidth()
	{
		return videoCapture->get(CV_CAP_PROP_FRAME_WIDTH);
	}

	int Camera::getHeight()
	{
		return videoCapture->get(CV_CAP_PROP_FRAME_HEIGHT);
	}

	int Camera::getResolution()
	{
		return resolution;
	}
	
}