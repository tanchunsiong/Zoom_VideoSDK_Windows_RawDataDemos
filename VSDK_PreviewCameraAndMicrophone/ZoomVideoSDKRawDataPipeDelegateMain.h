#pragma once
//getRawShare
//getRawVideo



#include "helpers/zoom_video_sdk_user_helper_interface.h"
#include <vector>

using namespace ZOOMVIDEOSDK;

class ZoomVideoSDKRawDataPipeDelegateMain :
	public  IZoomVideoSDKRawDataPipeDelegate
{
	virtual void onRawDataFrameReceived(YUVRawDataI420* data);
	virtual void onRawDataStatusChanged(RawDataStatus status);
	virtual void onShareCursorDataReceived(ZoomVideoSDKShareCursorData info);

	bool isShareScreen_;
	int instance_id_;
	static int instance_count;
	static std::vector<ZoomVideoSDKRawDataPipeDelegateMain*> list_;
	IZoomVideoSDKUser* user_;

	int in_width = 0;
	int in_height = 0;
	int out_width = 640;
	int out_height = 480;


public:
	//this is for preview only, doesn't need user
	ZoomVideoSDKRawDataPipeDelegateMain();
	ZoomVideoSDKRawDataPipeDelegateMain(IZoomVideoSDKUser* user);
	ZoomVideoSDKRawDataPipeDelegateMain(IZoomVideoSDKUser* user, bool isShareScreen);
	~ZoomVideoSDKRawDataPipeDelegateMain();


	static void log(const wchar_t* format, ...);
	static void err_msg(int code);
};




