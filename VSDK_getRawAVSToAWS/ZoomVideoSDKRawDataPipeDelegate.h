#pragma once
//getRawShare
//getRawVideo


#include <vector>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std::chrono;

#include "helpers/zoom_video_sdk_user_helper_interface.h"
#include <vector>

using namespace ZOOMVIDEOSDK;

class ZoomVideoSDKRawDataPipeDelegate :
	public  IZoomVideoSDKRawDataPipeDelegate
{
	virtual void onRawDataFrameReceived(YUVRawDataI420* data);
	virtual void onRawDataStatusChanged(RawDataStatus status);
	static ZoomVideoSDKRawDataPipeDelegate* find_instance(IZoomVideoSDKUser* user);


	int instance_id_;
	static int instance_count;
	static std::vector<ZoomVideoSDKRawDataPipeDelegate*> list_;
	IZoomVideoSDKUser* user_;

	int in_width = 0;
	int in_height = 0;
	int out_width = 640;
	int out_height = 480;

	

public:
	//this is for preview only, doesn't need user
	ZoomVideoSDKRawDataPipeDelegate();
	ZoomVideoSDKRawDataPipeDelegate(IZoomVideoSDKUser* user);
	~ZoomVideoSDKRawDataPipeDelegate();


	 void padData(IZoomVideoSDKUser* user);
	 void start_encoding_for(IZoomVideoSDKUser* user);
	 void stop_encoding_for(IZoomVideoSDKUser* user);
	 void log(const wchar_t* format, ...);
	 void err_msg(int code);
};




