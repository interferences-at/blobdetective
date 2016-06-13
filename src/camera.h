#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <opencv2/opencv.hpp>

namespace blobdetective {

	class Application;

	class Camera
	{
	public:
	
		Camera(Application* parent_);
		~Camera();		
		Application* parent;
		static const char* windowName;
		
		bool start(int index);
		bool start(const char * ID);
		int getFPS();
		static void setFPS(int fps, void * ptr);
		static void setResolution(int resolution, void * ptr);
		static void setBrightness(int brightness, void * ptr);
		static void setContrast(int contrast, void * ptr);
		static void setExposure(int exposure, void * ptr);
		static void setGain(int gain, void * ptr);
		void createSettingWindow();
		bool getFrame(cv::Mat& frame);
		int getWidth();
		int getHeight();
		int getResolution();


	private:
		cv::VideoCapture* videoCapture;
		int fps;
		int resolution;
		int brightness;
		int contrast;
		int exposure;
		int gain;
	}; 

}

#endif
