#pragma once

#include "zoom_video_sdk_delegate_interface.h"
#include "zoom_video_sdk_chat_message_interface.h"

#define MAIN_FRAME_TITLE _T("demo_main_window")
#define APP_NAME _T("video sdk demo")
#define POPUP_EXIST_APP 1
using namespace ZOOM_VIDEO_SDK_NAMESPACE;

enum ReactionType;
enum CmdChannelType;
struct LowerThirdColorInfo;
struct LowerThirdInfo;

class CJoinSessionWndUI;
class CTitleBarWndUI;
class CBottomBarWndUI;
class CLeaveSessionTipsWndUI;
class CSessionInfoWndUI;
class CSessionSettingsWndUI;
class VideoShowMgr;
class ShareSelectAppsWndUI;
class ShareToolBarWndUI;
class ChatContentWndUI;
class TurnPageWndUI;
class MessageTipWndUI;
class CReactionBarWnd;
class CLowerThirdsInfoWnd;
class CBulletScreenWnd;
class IParseChannelCmdWnd;
class CFeedBackLaunchWnd;
class CFeedBackSubmitWnd;
class CFeedBackTipsWnd;
class CLowerThirdsSettingWndUI;
class CMorePopWnd;
class StatisticsWndUI;

class CMainFrame :public IZoomVideoSDKDelegate
{
public:

	static CMainFrame& GetInstance();


	void InitVideoSDK();
	void UninitVideoSDK();

	
	

	
	void StopShare();
	void SendChatToAll(const zchar_t* msgContent);



	bool IsCommandChannelConnect();

	void JoinSession(ZoomVideoSDKSessionContext& session_context);
	void LeaveSession(bool bEnd);





	//IZoomVideoSDKDelegate
public:
	virtual void onSessionJoin();
	virtual void onSessionLeave();
	virtual void onError(ZoomVideoSDKErrors errorCode, int detailErrorCode);

	virtual void onUserJoin(IZoomVideoSDKUserHelper* pUserHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList);
	virtual void onUserLeave(IZoomVideoSDKUserHelper* pUserHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList);

	virtual void onUserVideoStatusChanged(IZoomVideoSDKVideoHelper* pVideoHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList);
	virtual void onUserAudioStatusChanged(IZoomVideoSDKAudioHelper* pAudioHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList);
	virtual void onUserShareStatusChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, ZoomVideoSDKShareStatus status, ZoomVideoSDKShareType type);
	virtual void onLiveStreamStatusChanged(IZoomVideoSDKLiveStreamHelper* pLiveStreamHelper, ZoomVideoSDKLiveStreamStatus status);
	virtual void onChatNewMessageNotify(IZoomVideoSDKChatHelper* pChatHelper, IZoomVideoSDKChatMessage* messageItem);
	virtual void onUserHostChanged(IZoomVideoSDKUserHelper* pUserHelper, IZoomVideoSDKUser* pUser);
	virtual void onUserActiveAudioChanged(IZoomVideoSDKAudioHelper* pAudioHelper, IVideoSDKVector<IZoomVideoSDKUser*>* list);

	virtual void onUserRecordingConsent(IZoomVideoSDKUser* pUser) {}

	virtual void onSessionNeedPassword(IZoomVideoSDKPasswordHandler* handler);
	virtual void onSessionPasswordWrong(IZoomVideoSDKPasswordHandler* handler);

	virtual void onMixedAudioRawDataReceived(AudioRawData* data_);
	virtual void onOneWayAudioRawDataReceived(AudioRawData* data_, IZoomVideoSDKUser* pUser);
	virtual void onSharedAudioRawDataReceived(AudioRawData* data_);
	virtual void onUserManagerChanged(IZoomVideoSDKUser* pUser);
	virtual void onUserNameChanged(IZoomVideoSDKUser* pUser);
	virtual void onCameraControlRequestResult(IZoomVideoSDKUser* pUser, bool isApproved);
	virtual void onCameraControlRequestReceived(IZoomVideoSDKUser* pUser, ZoomVideoSDKCameraControlRequestType requestType, IZoomVideoSDKCameraControlRequestHandler* pCameraControlRequestHandler);
	virtual void onCommandReceived(IZoomVideoSDKUser* sender, const zchar_t* strCmd);
	virtual void onCommandChannelConnectResult(bool isSuccess);
	virtual void onInviteByPhoneStatus(PhoneStatus status, PhoneFailedReason reason);
	virtual void onCloudRecordingStatus(RecordingStatus status, IZoomVideoSDKRecordingConsentHandler* pHandler);
	virtual void onHostAskUnmute();
	virtual void onMultiCameraStreamStatusChanged(ZoomVideoSDKMultiCameraStreamStatus status, IZoomVideoSDKUser* pUser, IZoomVideoSDKRawDataPipe* pVideoPipe);

	virtual void onMicSpeakerVolumeChanged(unsigned int micVolume, unsigned int speakerVolume);
	virtual void onAudioDeviceStatusChanged(ZoomVideoSDKAudioDeviceType type, ZoomVideoSDKAudioDeviceStatus status);
	virtual void onTestMicStatusChanged(ZoomVideoSDK_TESTMIC_STATUS status);
	virtual void onSelectedAudioDeviceChanged();

	virtual void onLiveTranscriptionStatus(ZoomVideoSDKLiveTranscriptionStatus status);
	virtual void onLiveTranscriptionMsgReceived(const zchar_t* ltMsg, IZoomVideoSDKUser* pUser, ZoomVideoSDKLiveTranscriptionOperationType type);
	virtual void onLiveTranscriptionMsgError(ILiveTranscriptionLanguage* spokenLanguage, ILiveTranscriptionLanguage* transcriptLanguage);
	virtual void onLiveTranscriptionMsgInfoReceived(ILiveTranscriptionMessageInfo* messageInfo) {};
	virtual void onChatMsgDeleteNotification(IZoomVideoSDKChatHelper* pChatHelper, const zchar_t* msgID, ZoomVideoSDKChatMessageDeleteType deleteBy) {}

	virtual void onProxyDetectComplete() {}
	virtual void onProxySettingNotification(IZoomVideoSDKProxySettingHandler* handler) {}
	virtual void onSSLCertVerifiedFailNotification(IZoomVideoSDKSSLCertificateInfo* handler) {}

	virtual void onUserVideoNetworkStatusChanged(ZoomVideoSDKNetworkStatus status, IZoomVideoSDKUser* pUser) {}
private:
	CMainFrame();
	~CMainFrame();
	
	void InitControls();
	
	

	void StartPreview();



	void OnLeaveSessionUIUpdate();
	void OnJoinMettingJoinFailed(int error_code);
	void OnInvalidParamterNotification();
	void OnMeetingDisconnecting();
	void SetCommandChannelConnect(bool is_connect);



private:
	



	bool is_show_gallery_ = false;
	bool is_command_channel_connected_ = false;
	bool is_full_screen_ = false;
public:

	VideoShowMgr* video_show_mgr_;
};