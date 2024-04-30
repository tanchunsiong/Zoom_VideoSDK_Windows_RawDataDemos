#pragma once

#include "resource.h"

#include "zoom_video_sdk_delegate_interface.h"
#include "zoom_video_sdk_chat_message_interface.h"

using namespace ZOOM_VIDEO_SDK_NAMESPACE;



class MainFrame :public IZoomVideoSDKDelegate
{
public:
	MainFrame();
	~MainFrame();


	void InitVideoSDK();
	void UninitVideoSDK();
	void JoinSession();
	void LeaveSession(bool bEnd);

	int numberOfSamples;
	


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

	virtual void onCalloutJoinSuccess(IZoomVideoSDKUser* pUser, const zchar_t* phoneNumber);

	virtual void onUserRecordingConsent(IZoomVideoSDKUser* pUser);

	virtual void onLiveStreamStatusChanged(IZoomVideoSDKLiveStreamHelper* pLiveStreamHelper, ZoomVideoSDKLiveStreamStatus status);
	virtual void onChatNewMessageNotify(IZoomVideoSDKChatHelper* pChatHelper, IZoomVideoSDKChatMessage* messageItem);
	virtual void onUserHostChanged(IZoomVideoSDKUserHelper* pUserHelper, IZoomVideoSDKUser* pUser);
	virtual void onUserActiveAudioChanged(IZoomVideoSDKAudioHelper* pAudioHelper, IVideoSDKVector<IZoomVideoSDKUser*>* list);


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

	virtual void onCameraListChanged();
	virtual void onLiveTranscriptionStatus(ZoomVideoSDKLiveTranscriptionStatus status);
	virtual void onLiveTranscriptionMsgReceived(const zchar_t* ltMsg, IZoomVideoSDKUser* pUser, ZoomVideoSDKLiveTranscriptionOperationType type);
	virtual void onOriginalLanguageMsgReceived(ILiveTranscriptionMessageInfo* messageInfo);
	virtual void onLiveTranscriptionMsgError(ILiveTranscriptionLanguage* spokenLanguage, ILiveTranscriptionLanguage* transcriptLanguage);
	virtual void onLiveTranscriptionMsgInfoReceived(ILiveTranscriptionMessageInfo* messageInfo);

	virtual void onChatMsgDeleteNotification(IZoomVideoSDKChatHelper* pChatHelper, const zchar_t* msgID, ZoomVideoSDKChatMessageDeleteType deleteBy);
	virtual void onChatPrivilegeChanged(IZoomVideoSDKChatHelper* pChatHelper, ZoomVideoSDKChatPrivilegeType privilege);

	virtual void onSendFileStatus(IZoomVideoSDKSendFile* file, const FileTransferStatus& status);
	virtual void onReceiveFileStatus(IZoomVideoSDKReceiveFile* file, const FileTransferStatus& status);

	virtual void onProxyDetectComplete();
	virtual void onProxySettingNotification(IZoomVideoSDKProxySettingHandler* handler);
	virtual void onSSLCertVerifiedFailNotification(IZoomVideoSDKSSLCertificateInfo* handler);

	virtual void onUserVideoNetworkStatusChanged(ZoomVideoSDKNetworkStatus status, IZoomVideoSDKUser* pUser);
	virtual void onCallCRCDeviceStatusChanged(ZoomVideoSDKCRCCallStatus status);


	virtual void onVideoCanvasSubscribeFail(ZoomVideoSDKSubscribeFailReason fail_reason, IZoomVideoSDKUser* pUser, void* handle);
	virtual void onShareCanvasSubscribeFail(ZoomVideoSDKSubscribeFailReason fail_reason, IZoomVideoSDKUser* pUser, void* handle);
	virtual void onAnnotationHelperCleanUp(IZoomVideoSDKAnnotationHelper* helper);
	virtual void onAnnotationPrivilegeChange(IZoomVideoSDKUser* pUser, bool enable);

	virtual void onAnnotationHelperActived(void* handle); private:
private:

	


};