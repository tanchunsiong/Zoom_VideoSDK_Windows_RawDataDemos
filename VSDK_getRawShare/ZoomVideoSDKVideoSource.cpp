//sendRawVideo

#include "ZoomVideoSDKVideoSource.h"

#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

using namespace ZOOMVIDEOSDK;
using namespace cv;
using namespace std;

int video_play_flag = -1;
int width = WIDTH;
int height = HEIGHT;

void PlayVideoFileToVirtualCamera(IZoomVideoSDKVideoSender* video_sender, string video_source)
{
    char* frameBuffer;
    int frameLen = height / 2 * 3 * width;
    frameBuffer = (char*)malloc(frameLen);

    // execute in a thread.
    while (video_play_flag > 0 && video_sender) {
        Mat frame;
        VideoCapture cap;
        cap.open(video_source);
        if (!cap.isOpened()) {
            cerr << "ERROR! Unable to open camera\n";
            video_play_flag = 0;
            return;
        }

        //--- GRAB AND WRITE LOOP
        std::cout << "Start grabbing" << endl;
        while (cap.read(frame))
        {
            // wait for a new frame from camera and store it into 'frame'

            // check if we succeeded
            if (frame.empty()) {
                cerr << "ERROR! blank frame grabbed\n";
                break;
            }

            //covert Mat to YUV buffer
            Mat yuv;
            cv::cvtColor(frame, yuv, COLOR_BGRA2YUV_I420);

            char* frameBuffer = (char*)yuv.data;
            int width = yuv.cols;
            int height = yuv.rows;
            int frameLen = yuv.total() * yuv.elemSize();

            
             ((IZoomVideoSDKVideoSender*)video_sender)->sendVideoFrame(frameBuffer, width, height, frameLen, 0);
        
        }
        cap.release();

    }
    video_play_flag = -1;
}

void ZoomVideoSDKVideoSource::onInitialize(IZoomVideoSDKVideoSender* sender, IVideoSDKVector<VideoSourceCapability >* support_cap_list, VideoSourceCapability& suggest_cap) {

	//sendRawVideo
	this->video_sender_ = sender;

}


void ZoomVideoSDKVideoSource::onPropertyChange(IVideoSDKVector<VideoSourceCapability >* support_cap_list, VideoSourceCapability suggest_cap) {}

void ZoomVideoSDKVideoSource::onStartSend() {
	//sendRawVideo
	printf("ZoomVideoSDKVideoSource::onStartSend() fired \n");
    if (video_sender_ && video_play_flag != 1) {
        while (video_play_flag > -1) {}
        video_play_flag = 1;
        thread(PlayVideoFileToVirtualCamera, video_sender_, video_source_).detach();
    }
    else {
        std::cout << "video_sender_ is null" << endl;
    }
}
void ZoomVideoSDKVideoSource::onStopSend() {}

void ZoomVideoSDKVideoSource::onUninitialized() {

	std::cout << "onUninitialized" << endl;
	video_sender_ = nullptr;
}

ZoomVideoSDKVideoSource::ZoomVideoSDKVideoSource(string video_source)
{
    video_source_ = video_source;
}


