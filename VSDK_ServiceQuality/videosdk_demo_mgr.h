#ifndef _ZOOM_INSTANT_SDK_MGR_H_
#define _ZOOM_INSTANT_SDK_MGR_H_

#include "zoom_video_sdk_delegate_interface.h"
#include "zoom_video_sdk_session_info_interface.h"
#include "helpers/zoom_video_sdk_audio_helper_interface.h"
#include "helpers/zoom_video_sdk_video_helper_interface.h"
#include "helpers/zoom_video_sdk_share_helper_interface.h"
#include "helpers/zoom_video_sdk_chat_helper_interface.h"
#include "helpers/zoom_video_sdk_cmd_channel_interface.h"
#include "zoom_video_sdk_interface.h"
#include "zoom_video_sdk_api.h"
#include "helpers/zoom_video_sdk_user_helper_interface.h"
#include "zoom_video_sdk_platform.h"
#include <vector>

using namespace ZOOM_VIDEO_SDK_NAMESPACE;


class ZoomVideoSDKMgr
{
public:
	static ZoomVideoSDKMgr& GetInst();
	~ZoomVideoSDKMgr();

	

	bool Init(IZoomVideoSDKDelegate* listener, ZoomVideoSDKInitParams init_params);

	void UnInit();
	std::wstring GetErrorStringByErrorCode(ZoomVideoSDKErrors err);


	IZoomVideoSDKSession* getSessionInfo();
	IZoomVideoSDKSession* JoinSession(ZoomVideoSDKSessionContext& session_context);
	ZoomVideoSDKErrors LeaveSession(bool end);
	ZoomVideoSDKErrors MuteAudio();
	ZoomVideoSDKErrors UnmuteAudio();
	ZoomVideoSDKErrors MuteVideo();
	ZoomVideoSDKErrors UnmuteVideo();
	bool IsMyselfVideoOn();
	bool IsMyselfAudioMuted();

	bool SelectCamera(const zchar_t* camera_device_id);
	ZoomVideoSDKErrors SelectSpeaker(const zchar_t* device_id, const zchar_t* device_name);
	ZoomVideoSDKErrors SelectMic(const zchar_t* device_id, const zchar_t* device_name);
	ZoomVideoSDKErrors StartShareScreen(const zchar_t* monitorID, ZoomVideoSDKShareOption option);
	ZoomVideoSDKErrors StartShareView(HWND hwnd, ZoomVideoSDKShareOption option);
	ZoomVideoSDKErrors StopShare();

	ZoomVideoSDKErrors SendCommand(IZoomVideoSDKUser* receiver, const zchar_t* strCmd);
	ZoomVideoSDKErrors SendChatToAll(const zchar_t* msgContent);

	const zchar_t* GetSessionName() const;
	int GetUserCountInSession();
	bool IsInSession();
	bool IsHost();
	IZoomVideoSDKUser* GetSharingUser();
	IZoomVideoSDKUser* GetMySelf();
	IZoomVideoSDKUser* GetSessionHost();

	IVideoSDKVector<IZoomVideoSDKCameraDevice*>* GetCameraList();
	IVideoSDKVector<IZoomVideoSDKSpeakerDevice*>* GetSpeakerList();
	IVideoSDKVector<IZoomVideoSDKMicDevice*>* GetMicList();
	vector<IZoomVideoSDKUser*> GetAllUsers();

	ZoomVideoSDKErrors GetSessionAudioStatisticInfo(ZoomVideoSDKSessionAudioStatisticInfo& send_info, ZoomVideoSDKSessionAudioStatisticInfo& recv_info);
	ZoomVideoSDKErrors GetSessionVideoStatisticInfo(ZoomVideoSDKSessionASVStatisticInfo& send_info, ZoomVideoSDKSessionASVStatisticInfo& recv_info);
	ZoomVideoSDKErrors GetSessionShareStatisticInfo(ZoomVideoSDKSessionASVStatisticInfo& send_info, ZoomVideoSDKSessionASVStatisticInfo& recv_info);


	IZoomVideoSDKAudioHelper* getAudioHelper();

	IZoomVideoSDKShareHelper* getShareHelper();

	IZoomVideoSDKRecordingHelper* getRecordingHelper();

	IZoomVideoSDKPhoneHelper* getPhoneHelper();

	IZoomVideoSDKVideoHelper* getVideoHelper();

	IZoomVideoSDKTestAudioDeviceHelper* getAudioDeviceTestHelper();
private:
	ZoomVideoSDKMgr();

	bool is_inited_;

	IZoomVideoSDK* video_sdk_obj_;
};

#endif //_ZOOM_INSTANT_SDK_MGR_H_