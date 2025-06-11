//getRawShare
//getRawVideo
#include "ZoomVideoSDKRawDataPipeDelegateMain.h"
#include "helpers/zoom_video_sdk_user_helper_interface.h"
#include <iostream>
#include <cstdint>
#include <fstream>
#include <cstring>
#include <cstdio>

using namespace ZOOMVIDEOSDK;
std::vector<ZoomVideoSDKRawDataPipeDelegateMain*> ZoomVideoSDKRawDataPipeDelegateMain::list_;
int ZoomVideoSDKRawDataPipeDelegateMain::instance_count = 0;

int lastMainHeight = 0;
int lastMainWidth = 0;


//this is for video preview only, doesn't need user
ZoomVideoSDKRawDataPipeDelegateMain::ZoomVideoSDKRawDataPipeDelegateMain()
{

}



ZoomVideoSDKRawDataPipeDelegateMain::~ZoomVideoSDKRawDataPipeDelegateMain()
{
	// finish ffmpeg encoding
	//log(L"********** [%d] Finishing encoding, user: %s, %dx%d.\n", instance_id_, user_->getUserName(), in_width, in_height);


	//user_->GetVideoPipe()->unSubscribe(this);
	//log(L"********** [%d] UnSubscribe, user: %s.\n", instance_id_, user_->getUserName());
	//instance_count--;
	//user_ = nullptr;
}







void ZoomVideoSDKRawDataPipeDelegateMain::onRawDataFrameReceived(YUVRawDataI420* data)
{
	//getRawVideo
	//this is the part where callback for raw video data or raw share data happens.
	//you can choose to save the data_ buffer as raw YUV file (warning, this is huge), or convert it to mp4 / avi or other format. 
	//do be mindful of the compute power and memory usage which this callback can utilize

	//I'm using isShareScreen_ to determine if it is user's video or sharescreen video

	//const zchar_t* userName = user_->getUserName();
	//const zchar_t* userID = user_->getUserID();
	const int width = data->GetStreamWidth();
	const int height = data->GetStreamHeight();
	const int bufLen = data->GetBufferLen();
	const int rotation = data->GetRotation();
	const int sourceID = data->GetSourceID();

	if (lastMainHeight != height) {
		lastMainHeight = height;
		lastMainWidth = width;

		printf("main cam: width:%d\n", width);
		printf("main cam: height:%d\n", height);
		printf("main cam: len:%d\n", bufLen);
	}




	//// Open the file for writing
	//std::ofstream outputFile("output.yuv", std::ios::out | std::ios::binary | std::ios::app);
	//if (!outputFile.is_open())
	//{
	//	//error opening file
	//	return;
	//}


	//char* _data = new char[data->GetStreamHeight() * data->GetStreamWidth() * 3 / 2];

	//memset(_data, 0, data->GetStreamHeight() * data->GetStreamWidth() * 3 / 2);

	//// Copy Y buffer
	//memcpy(_data, data->GetYBuffer(), data->GetStreamHeight() * data->GetStreamWidth());

	//// Copy U buffer
	//size_t loc = data->GetStreamHeight() * data->GetStreamWidth();
	//memcpy(&_data[loc], data->GetUBuffer(), data->GetStreamHeight() * data->GetStreamWidth() / 4);


	//// Copy V buffer
	//loc = (data->GetStreamHeight() * data->GetStreamWidth()) + (data->GetStreamHeight() * data->GetStreamWidth() / 4);
	//memcpy(&_data[loc], data->GetVBuffer(), data->GetStreamHeight() * data->GetStreamWidth() / 4);



	////outputFile.write((char*)data->GetBuffer(), data->GetBufferLen());
	//// Write the Y plane
	//outputFile.write(_data, data->GetStreamHeight() * data->GetStreamWidth() * 3 / 2);


	//// Close the file
	//outputFile.close();
	//outputFile.flush();
	////cout << "YUV420 buffer saved to file." << endl;

}

void ZoomVideoSDKRawDataPipeDelegateMain::onRawDataStatusChanged(RawDataStatus status)
{
	//log(L"********** [%d] onRawDataStatusChanged, user: %s, %d.\n", instance_id_, user_->getUserName(), status);
	if (status == RawData_On)
	{
	}
	else
	{
	}
}

void ZoomVideoSDKRawDataPipeDelegateMain::onShareCursorDataReceived(ZoomVideoSDKShareCursorData info) {}

void ZoomVideoSDKRawDataPipeDelegateMain::err_msg(int code)
{
	char errbuf[100];

	printf("%s\n", errbuf);
}

void ZoomVideoSDKRawDataPipeDelegateMain::log(const wchar_t* format, ...)
{

}