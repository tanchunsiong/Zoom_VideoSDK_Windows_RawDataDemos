#ifndef _ZOOM_INSTANT_SDK_MGR_H_
#define _ZOOM_INSTANT_SDK_MGR_H_

#include "zoom_video_sdk_session_info_interface.h"
#include "helpers/zoom_video_sdk_audio_helper_interface.h"
#include "helpers/zoom_video_sdk_video_helper_interface.h"
#include "helpers/zoom_video_sdk_share_helper_interface.h"
#include "helpers/zoom_video_sdk_chat_helper_interface.h"
#include "helpers/zoom_video_sdk_cmd_channel_interface.h"

enum CameraControlAction
{
	TURN_LEFT, TURN_RIGHT, TURN_UP, TURN_DOWN, ZOOM_IN, ZOOM_OUT
};

enum CmdChannelType
{
	CMD_REACTION = 1, CMD_FEEDBACK_LAUNCH, CMD_FEEDBACK_SUBMIT, CMD_LOWER_THIRD
};

enum LowerThirdColor
{
	COLOR_NONE, COLOR_BULE, COLOR_VIOLET, COLOR_GREEN, COLOR_ORANGE, COLOR_RED, COLOR_DARKBULE, COLOR_YELLOW
};

struct LowerThirdInfo
{
	std::wstring lower_third_name;
	std::wstring lower_third_description;
	std::wstring lower_third_color;

	LowerThirdInfo()
	{
		lower_third_name = _T("");
		lower_third_description = _T("");
		lower_third_color = _T("");
	}
};

struct LowerThirdColorInfo
{
	LowerThirdColor color;
	std::wstring color_normal_image;
	std::wstring color_triangle_image;
	std::wstring bkcolor;
	Gdiplus::Color gdi_font_color;
	Gdiplus::Color gdi_font_bk_color;
	DWORD textcolor;

	LowerThirdColorInfo()
	{
		color = LowerThirdColor::COLOR_NONE;
		bkcolor = _T("#444B53");
		textcolor = 0XF7F9FA;
		color_normal_image = _T("res = 'BUTTON_COLOR_COMMON' restype = 'ZPIMGRES' source = '0,0,2,18'");
		color_triangle_image = _T("res = 'BUTTON_COLOR_COMMON' restype = 'ZPIMGRES' source = '0,0,11,18'");
		gdi_font_color = Gdiplus::Color(255, 255, 255);
		gdi_font_bk_color = { 68, 75, 83 };
	}
};

class IParseChannelCmdWnd {
public:
	virtual void OnParseChannelCmd(IZoomVideoSDKUser* pSender,const std::vector<std::wstring >& cmdVector) = 0;
	virtual HWND GetHWND() = 0;
};

class ZoomVideoSDKMgr
{
public:
	static ZoomVideoSDKMgr& GetInst();
	~ZoomVideoSDKMgr();

	bool Init(IZoomVideoSDKDelegate* listener, ZoomVideoSDKInitParams init_params);

	void UnInit();
	std::wstring GetErrorStringByErrorCode(ZoomVideoSDKErrors err);

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

	ZoomVideoSDKErrors StartShare2ndCamera();
	std::wstring GetSharingCameraID();
	void SetSharingCameraID(std::wstring cameraID);
	void SetRemoteControlCameraUser(IZoomVideoSDKUser* user);
	IZoomVideoSDKUser* GetRemoteControlCameraUser();
	ZoomVideoSDKErrors RequestControlRemoteCamera();
	ZoomVideoSDKErrors GiveUpControlRemoteCamera(std::wstring& user_name);
	ZoomVideoSDKErrors TurnLeft(unsigned int range);
	ZoomVideoSDKErrors TurnRight(unsigned int range);
	ZoomVideoSDKErrors TurnUp(unsigned int range);
	ZoomVideoSDKErrors TurnDown(unsigned int range);
	ZoomVideoSDKErrors ZoomIn(unsigned int range);
	ZoomVideoSDKErrors ZoomOut(unsigned int range);
	bool RemoveUser(IZoomVideoSDKUser* user);
	void OnUserLeave(IVideoSDKVector<IZoomVideoSDKUser*>* userList);
	
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
	uint32_t GetNumberOfCameras();
	IZoomVideoSDKShareHelper* GetShareHelper();

	ZoomVideoSDKErrors GetSessionAudioStatisticInfo(ZoomVideoSDKSessionAudioStatisticInfo& send_info, ZoomVideoSDKSessionAudioStatisticInfo& recv_info);
	ZoomVideoSDKErrors GetSessionVideoStatisticInfo(ZoomVideoSDKSessionASVStatisticInfo& send_info, ZoomVideoSDKSessionASVStatisticInfo& recv_info);
	ZoomVideoSDKErrors GetSessionShareStatisticInfo(ZoomVideoSDKSessionASVStatisticInfo& send_info, ZoomVideoSDKSessionASVStatisticInfo& recv_info);

private:
	ZoomVideoSDKMgr();

	bool is_inited_;
	
	IZoomVideoSDK* video_sdk_obj_;

	std::wstring sharing_camera_id;
	IZoomVideoSDKUser* remote_camera_control_user;
};

#endif //_ZOOM_INSTANT_SDK_MGR_H_