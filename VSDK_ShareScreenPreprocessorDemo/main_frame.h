#pragma once

#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <tesseract/baseapi.h>
#include <opencv2/core.hpp>
#include "resource.h"

#include "zoom_video_sdk_delegate_interface.h"
#include "zoom_video_sdk_chat_message_interface.h"
#include "helpers/zoom_video_sdk_share_helper_interface.h"

using namespace ZOOM_VIDEO_SDK_NAMESPACE;



class MainFrame :public IZoomVideoSDKDelegate, public IZoomVideoSDKShareSource, public IZoomVideoSDKRawDataPipeDelegate, public IZoomVideoSDKSharePreprocessor
{
public:
	MainFrame();
	~MainFrame();

	//custom method for mainframe
	void StartToShare();
	std::vector<std::wstring> GetMonitorDeviceNames();
	void RemovePIIFromRawData(YUVRawDataI420* data_);

	//IZoomVideoSDKSharePreprocessor
	virtual void onCapturedRawDataReceived(YUVRawDataI420* pRawData, IZoomVideoSDKSharePreprocessSender* pSender);
	virtual void onShareStopped();

	void InitVideoSDK();
	void UninitVideoSDK();
	void JoinSession();
	void LeaveSession(bool bEnd);


	


	//IZoomVideoSDKDelegate
public:
	virtual void onSessionJoin();
	virtual void onSessionLeave();
	virtual void onSessionLeave(ZoomVideoSDKSessionLeaveReason eReason);
	virtual void onError(ZoomVideoSDKErrors errorCode, int detailErrorCode);

	virtual void onUserJoin(IZoomVideoSDKUserHelper* pUserHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList);
	virtual void onUserLeave(IZoomVideoSDKUserHelper* pUserHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList);

	virtual void onUserVideoStatusChanged(IZoomVideoSDKVideoHelper* pVideoHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList);
	virtual void onUserAudioStatusChanged(IZoomVideoSDKAudioHelper* pAudioHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList);
	 virtual void onUserShareStatusChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, IZoomVideoSDKShareAction* pShareAction);


	virtual void onShareContentChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, IZoomVideoSDKShareAction* pShareAction);
	virtual void onFailedToStartShare(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser);




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

	virtual void onRemoteControlStatus(IZoomVideoSDKUser* pUser, IZoomVideoSDKShareAction* pShareAction, ZoomVideoSDKRemoteControlStatus status);
	virtual void onRemoteControlRequestReceived(IZoomVideoSDKUser* pUser, IZoomVideoSDKShareAction* pShareAction,
		IZoomVideoSDKRemoteControlRequestHandler* pRemoteControlRequestHandler);
	virtual void onRemoteControlServiceInstallResult(bool bSuccess);

	virtual void onCommandReceived(IZoomVideoSDKUser* sender, const zchar_t* strCmd);
	virtual void onCommandChannelConnectResult(bool isSuccess);
	virtual void onInviteByPhoneStatus(PhoneStatus status, PhoneFailedReason reason);
	virtual void onCalloutJoinSuccess(IZoomVideoSDKUser* pUser, const zchar_t* phoneNumber);

	virtual void onCloudRecordingStatus(RecordingStatus status, IZoomVideoSDKRecordingConsentHandler* pHandler);
	virtual void onHostAskUnmute();
	virtual void onMultiCameraStreamStatusChanged(ZoomVideoSDKMultiCameraStreamStatus status, IZoomVideoSDKUser* pUser, IZoomVideoSDKRawDataPipe* pVideoPipe);

	virtual void onMicSpeakerVolumeChanged(unsigned int micVolume, unsigned int speakerVolume);
	virtual void onAudioDeviceStatusChanged(ZoomVideoSDKAudioDeviceType type, ZoomVideoSDKAudioDeviceStatus status);
	virtual void onTestMicStatusChanged(ZoomVideoSDK_TESTMIC_STATUS status);

	virtual void onSelectedAudioDeviceChanged();
	virtual void onCameraListChanged();
	virtual void onLiveTranscriptionStatus(ZoomVideoSDKLiveTranscriptionStatus status);
	virtual void onOriginalLanguageMsgReceived(ILiveTranscriptionMessageInfo* messageInfo);

	virtual void onLiveTranscriptionMsgInfoReceived(ILiveTranscriptionMessageInfo* messageInfo);
	virtual void onLiveTranscriptionMsgError(ILiveTranscriptionLanguage* spokenLanguage, ILiveTranscriptionLanguage* transcriptLanguage);


	//virtual void onLiveTranscriptionMsgReceived(const zchar_t* ltMsg, IZoomVideoSDKUser* pUser, ZoomVideoSDKLiveTranscriptionOperationType type);


	virtual void onChatMsgDeleteNotification(IZoomVideoSDKChatHelper* pChatHelper, const zchar_t* msgID, ZoomVideoSDKChatMessageDeleteType deleteBy);
	virtual void onChatPrivilegeChanged(IZoomVideoSDKChatHelper* pChatHelper, ZoomVideoSDKChatPrivilegeType privilege);

	virtual void onSendFileStatus(IZoomVideoSDKSendFile* file, const FileTransferStatus& status);
	virtual void onReceiveFileStatus(IZoomVideoSDKReceiveFile* file, const FileTransferStatus& status);

	virtual void onProxyDetectComplete();
	virtual void onProxySettingNotification(IZoomVideoSDKProxySettingHandler* handler);
	virtual void onSSLCertVerifiedFailNotification(IZoomVideoSDKSSLCertificateInfo* handler);

	virtual void onUserVideoNetworkStatusChanged(ZoomVideoSDKNetworkStatus status, IZoomVideoSDKUser* pUser);
	virtual void onShareNetworkStatusChanged(ZoomVideoSDKNetworkStatus shareNetworkStatus, bool isSendingShare);

	virtual void onCallCRCDeviceStatusChanged(ZoomVideoSDKCRCCallStatus status);


	virtual void onVideoCanvasSubscribeFail(ZoomVideoSDKSubscribeFailReason fail_reason, IZoomVideoSDKUser* pUser, void* handle);
	virtual void onShareCanvasSubscribeFail(IZoomVideoSDKUser* pUser, void* handle, IZoomVideoSDKShareAction* pShareAction);
	virtual void onAnnotationHelperCleanUp(IZoomVideoSDKAnnotationHelper* helper);
	virtual void onAnnotationPrivilegeChange(IZoomVideoSDKUser* pUser, IZoomVideoSDKShareAction* pShareAction);

	virtual void onAnnotationHelperActived(void* handle);
	virtual void onVideoAlphaChannelStatusChanged(bool isAlphaModeOn);

	virtual void onSpotlightVideoChanged(IZoomVideoSDKVideoHelper* pVideoHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList);
	virtual void onBindIncomingLiveStreamResponse(bool bSuccess, const zchar_t* strStreamKeyID);
	virtual void onUnbindIncomingLiveStreamResponse(bool bSuccess, const zchar_t* strStreamKeyID);

	virtual void onIncomingLiveStreamStatusResponse(bool bSuccess, IVideoSDKVector<IncomingLiveStreamStatus>* pStreamsStatusList);
	virtual void onStartIncomingLiveStreamResponse(bool bSuccess, const zchar_t* strStreamKeyID);
	virtual void onStopIncomingLiveStreamResponse(bool bSuccess, const zchar_t* strStreamKeyID);
	virtual void onShareContentSizeChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, IZoomVideoSDKShareAction* pShareAction);

	virtual void onSubSessionStatusChanged(ZoomVideoSDKSubSessionStatus status, IVideoSDKVector<ISubSessionKit*>* pSubSessionKitList);
	virtual void onSubSessionManagerHandle(IZoomVideoSDKSubSessionManager* pManager);
	virtual void onSubSessionParticipantHandle(IZoomVideoSDKSubSessionParticipant* pParticipant);
	
	
	virtual void onSubSessionUsersUpdate(ISubSessionKit* pSubSessionKit);
	virtual void onBroadcastMessageFromMainSession(const zchar_t* sMessage, const zchar_t* sUserName);



	virtual void onSubSessionUserHelpRequest(ISubSessionUserHelpRequestHandler* pHandler);
	virtual void onSubSessionUserHelpRequestResult(ZoomVideoSDKUserHelpRequestResult eResult);


	//IZoomVideoSDKRawDataPipeDelegate

	virtual void onRawDataFrameReceived(YUVRawDataI420* data_);
	virtual void onRawDataStatusChanged(RawDataStatus status);
	virtual void onShareCursorDataReceived(ZoomVideoSDKShareCursorData info);


	
	//IZoomVideoSDKShareSource 
	virtual void onShareSendStarted(IZoomVideoSDKShareSender* pSender);
	virtual void onShareSendStopped();





private:

	
	IZoomVideoSDKSharePreprocessSender* m_pShareSender;
	tesseract::TessBaseAPI m_tess;
	bool m_tessInitialized = false;
	int m_frameCounter = 0;

//optimizing OCR with previous screen compare
	std::vector<cv::Rect> m_prevPIIBoxes;
	std::vector<std::string> m_prevWords;

};