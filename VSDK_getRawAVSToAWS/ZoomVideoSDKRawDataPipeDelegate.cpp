//getRawShare
//getRawVideo
#include "ZoomVideoSDKRawDataPipeDelegate.h"
#include "helpers/zoom_video_sdk_user_helper_interface.h"
#include <iostream>
#include <cstdint>
#include <fstream>
#include <cstring>
#include <cstdio>

using namespace ZOOMVIDEOSDK;
std::vector<ZoomVideoSDKRawDataPipeDelegate*> ZoomVideoSDKRawDataPipeDelegate::list_;
int ZoomVideoSDKRawDataPipeDelegate::instance_count = 0;

//this is for video preview only, doesn't need user
ZoomVideoSDKRawDataPipeDelegate::ZoomVideoSDKRawDataPipeDelegate()
{

}

ZoomVideoSDKRawDataPipeDelegate::ZoomVideoSDKRawDataPipeDelegate(IZoomVideoSDKUser* user)
{
	//getRawVideo
	instance_id_ = instance_count++;
	user_ = user;
	user_->GetVideoPipe()->subscribe(ZoomVideoSDKResolution_360P, this);
	list_.push_back(this);
}


ZoomVideoSDKRawDataPipeDelegate::~ZoomVideoSDKRawDataPipeDelegate()
{
	// finish ffmpeg encoding
	log(L"********** [%d] Finishing encoding, user: %s, %dx%d.\n", instance_id_, user_->getUserName(), in_width, in_height);


	user_->GetVideoPipe()->unSubscribe(this);
	log(L"********** [%d] UnSubscribe, user: %s.\n", instance_id_, user_->getUserName());
	instance_count--;
	user_ = nullptr;
}


ZoomVideoSDKRawDataPipeDelegate* ZoomVideoSDKRawDataPipeDelegate::find_instance(IZoomVideoSDKUser* user)
{
	for (auto iter = list_.begin(); iter != list_.end(); iter++)
	{
		ZoomVideoSDKRawDataPipeDelegate* item = *iter;
		if (item->user_ == user)
		{
			return item;
		}
	}
	return nullptr;
}

void ZoomVideoSDKRawDataPipeDelegate::start_encoding_for(IZoomVideoSDKUser* user)
{
	//getRawVideo
	instance_id_ = instance_count++;
	user_ = user;
	user_->GetVideoPipe()->subscribe(ZoomVideoSDKResolution_360P, this);
	list_.push_back(this);
}

void ZoomVideoSDKRawDataPipeDelegate::stop_encoding_for(IZoomVideoSDKUser* user)
{
	ZoomVideoSDKRawDataPipeDelegate* encoder = ZoomVideoSDKRawDataPipeDelegate::find_instance(user);
	if (encoder)
	{
		encoder->~ZoomVideoSDKRawDataPipeDelegate();
	}
}

void  ZoomVideoSDKRawDataPipeDelegate::padData(IZoomVideoSDKUser* user) {
	const int width = 640;
	const int height = 360;
	const int ySize = width * height;
	const int uvSize = ySize / 4; // Because of the 4:2:0 format

	// Open the file for writing
	std::ofstream outputFile("chrome.yuv", std::ios::out | std::ios::binary | std::ios::app);
	if (!outputFile.is_open()) {
		// Error opening file
		return;
	}

	// Allocate memory for a black frame
	char* blackFrame = new char[ySize + 2 * uvSize]; // YUV420p format

	// Set Y plane to 0 for black
	memset(blackFrame, 0, ySize);

	// Set U and V planes to 128 for color neutral (assuming 0-255 range)
	memset(blackFrame + ySize, 128, uvSize); // U plane
	memset(blackFrame + ySize + uvSize, 128, uvSize); // V plane

	// Write the black frame to the file
	outputFile.write(blackFrame, ySize + 2 * uvSize);

	// Clean up
	delete[] blackFrame;
	outputFile.close();
	std::cout << "Padding" << std::endl;
}

void ZoomVideoSDKRawDataPipeDelegate::onRawDataFrameReceived(YUVRawDataI420* data)
{
	//getRawVideo
	//this is the part where callback for raw video data or raw share data happens.
	//you can choose to save the data_ buffer as raw YUV file (warning, this is huge), or convert it to mp4 / avi or other format. 
	//do be mindful of the compute power and memory usage which this callback can utilize

	
	const zchar_t* userName = user_->getUserName();
	const zchar_t* userID = user_->getUserID();
	const int width = data->GetStreamWidth();
	const int height = data->GetStreamHeight();
	const int bufLen = data->GetBufferLen();
	const int rotation = data->GetRotation();
	const int sourceID = data->GetSourceID();

	//method 1

	//// Open the file for writing
	//std::ofstream outputFile("chrome.yuv", std::ios::out | std::ios::binary | std::ios::app);
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
	//std::cout << "Saving Raw Data" << std::endl;

	//method 2

	// Open the file for writing
	std::ofstream outputFile("chrome.yuv", std::ios::out | std::ios::binary | std::ios::app);
	if (!outputFile.is_open())
	{
		std::cout << "Error opening file." << std::endl;
		return;
	}
	// Calculate the sizes for Y, U, and V components
	size_t ySize = data->GetStreamWidth() * data->GetStreamHeight();
	size_t uvSize = ySize / 4;

	// Write Y, U, and V components to the output file
	outputFile.write(data->GetYBuffer(), ySize);
	outputFile.write(data->GetUBuffer(), uvSize);
	outputFile.write(data->GetVBuffer(), uvSize);


	// Close the file
	outputFile.close();
	outputFile.flush();

}

void ZoomVideoSDKRawDataPipeDelegate::onRawDataStatusChanged(RawDataStatus status)
{
	log(L"********** [%d] onRawDataStatusChanged, user: %s, %d.\n", instance_id_, user_->getUserName(), status);
	if (status == RawData_On)
	{
	}
	else
	{
	}
}

void ZoomVideoSDKRawDataPipeDelegate::err_msg(int code)
{
	char errbuf[100];

	printf("%s\n", errbuf);
}

void ZoomVideoSDKRawDataPipeDelegate::log(const wchar_t* format, ...)
{

}