#include "stdafx.h"
//#include "zoom_video_sdk_include_interface.h"
#include "main_frame.h"
#include "videosdk_demo_mgr.h"



//sendRawShare
#include "ZoomVideoSDKShareSource.h"

//these are controls to demonstrate the flow
bool sendRawShare = false;   //work in progress, ignore this sample code for now


void CMainFrame::onVideoCanvasSubscribeFail(ZoomVideoSDKSubscribeFailReason fail_reason, IZoomVideoSDKUser* pUser, void* handle)
{
}

void CMainFrame::onShareCanvasSubscribeFail(ZoomVideoSDKSubscribeFailReason fail_reason, IZoomVideoSDKUser* pUser, void* handle)
{
}

void CMainFrame::onAnnotationHelperCleanUp(IZoomVideoSDKAnnotationHelper* helper)
{
}

void CMainFrame::onAnnotationPrivilegeChange(IZoomVideoSDKUser* pUser, bool enable)
{
}

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{


	UninitVideoSDK();
}


CMainFrame& CMainFrame::GetInstance()
{
	static CMainFrame s_MainFramInstance;
	return s_MainFramInstance;
}

void CMainFrame::InitVideoSDK()
{
	ZoomVideoSDKInitParams init_params;
	init_params.domain = _T("https://zoom.us");
	init_params.enableLog = true;
	init_params.logFilePrefix = _T("zoom_win_video_demo");
	init_params.videoRawDataMemoryMode = ZoomVideoSDKRawDataMemoryModeHeap;
	init_params.shareRawDataMemoryMode = ZoomVideoSDKRawDataMemoryModeHeap;
	init_params.audioRawDataMemoryMode = ZoomVideoSDKRawDataMemoryModeHeap;
	init_params.enableIndirectRawdata = false;

	ZoomVideoSDKMgr::GetInst().Init(this, init_params);
}

void CMainFrame::UninitVideoSDK()
{
	ZoomVideoSDKMgr::GetInst().UnInit();
}






void CMainFrame::OnLeaveSessionUIUpdate()
{

	StopShare();

}

void CMainFrame::OnJoinMeetingJoinFailed(int error_code)
{

}

void CMainFrame::OnInvalidParamterNotification()
{

}

void CMainFrame::OnMeetingDisconnecting()
{

}

void CMainFrame::StartPreview()
{


}

void CMainFrame::StopShare()
{

	ZoomVideoSDKErrors err = ZoomVideoSDKMgr::GetInst().StopShare();
}

void CMainFrame::SendChatToAll(const zchar_t* msgContent)
{
	ZoomVideoSDKMgr::GetInst().SendChatToAll(msgContent);
}



void CMainFrame::SetCommandChannelConnect(bool is_connect)
{
	is_command_channel_connected_ = is_connect;
}

bool CMainFrame::IsCommandChannelConnect()
{
	return is_command_channel_connected_;
}

void CMainFrame::JoinSession()
{


	//credentials to auth and password & topic name to join specific session
	std::wstring session_name = L"chunsession6871";
	std::wstring sUserName = L"vsdk_skeletondemo";
	std::wstring session_password_ = L"12345678";
	std::wstring token = L"xxxx.yyyy.zzzz";


	//is turn off video
	bool is_video_on = false;
	//is mute audio
	bool is_mute_audio = true;

	//session context 
	ZoomVideoSDKSessionContext session_context;
	session_context.sessionName = session_name.c_str();
	session_context.sessionPassword = session_password_.c_str();
	session_context.userName = sUserName.c_str();
	session_context.token = token.c_str();
	session_context.videoOption.localVideoOn = is_video_on;
	session_context.audioOption.connect = false;
	session_context.audioOption.mute = is_mute_audio;


	if (sendRawShare) {
		//nothing needed to be done before joining session
	}

	IZoomVideoSDKSession* pSession = ZoomVideoSDKMgr::GetInst().JoinSession(session_context);
	if (pSession)
	{
		pSession->getMyself()->GetVideoPipe();
	}
}

void CMainFrame::LeaveSession(bool end)
{
	ZoomVideoSDKErrors err = ZoomVideoSDKMgr::GetInst().LeaveSession(end);
}


//this callback happens when sdk/use has successfully joined the session
void CMainFrame::onSessionJoin()
{

	std::cout << "onSessionJoin()" << std::endl;


	if (sendRawShare) {

		//needed for share source
		//this needs to be done after joing session
		ZoomVideoSDKShareSource* virtual_share_source = new ZoomVideoSDKShareSource();
		ZoomVideoSDKErrors err2 = ZoomVideoSDKMgr::GetInst().getShareHelper()->startSharingExternalSource(virtual_share_source);
		if (err2 == ZoomVideoSDKErrors_Success) {
		}
		else {
			printf("Error setting external source %s\n", err2);
		}
	};


}

void CMainFrame::onSessionLeave()
{

}

void CMainFrame::onError(ZoomVideoSDKErrors errorCode, int detailErrorCode)
{
	std::cout << "onError()" << std::endl;
	if (errorCode == ZoomVideoSDKErrors_Session_Disconnecting || errorCode == ZoomVideoSDKErrors_Session_Reconnecting)
	{
		OnMeetingDisconnecting();
	}
	else if (errorCode == ZoomVideoSDKErrors_Invalid_Parameter)
	{
		OnInvalidParamterNotification();
	}
	else
	{
		OnJoinMeetingJoinFailed(errorCode);
	}
}

void CMainFrame::onUserJoin(IZoomVideoSDKUserHelper* pUserHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{
	std::cout << "onUserJoin()" << std::endl;



}

void CMainFrame::onUserLeave(IZoomVideoSDKUserHelper* pUserHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{

}


void CMainFrame::onUserVideoStatusChanged(IZoomVideoSDKVideoHelper* pVideoHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{

}

void CMainFrame::onUserAudioStatusChanged(IZoomVideoSDKAudioHelper* pAudioHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{

}

void CMainFrame::onUserShareStatusChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, ZoomVideoSDKShareStatus status, ZoomVideoSDKShareType type)
{

}

void CMainFrame::onLiveStreamStatusChanged(IZoomVideoSDKLiveStreamHelper* pLiveStreamHelper, ZoomVideoSDKLiveStreamStatus status)
{

}

void CMainFrame::onChatNewMessageNotify(IZoomVideoSDKChatHelper* pChatHelper, IZoomVideoSDKChatMessage* messageItem)
{
	std::cout << "onChatNewMessageNotify()" << std::endl;
	if (!messageItem)
		return;
	IZoomVideoSDKUser* send_user = messageItem->getSendUser();
	if (send_user)
	{
		const zchar_t* user_name = send_user->getUserName();
		const zchar_t* content = messageItem->getContent();


	}
}

void CMainFrame::onUserHostChanged(IZoomVideoSDKUserHelper* pUserHelper, IZoomVideoSDKUser* pUser)
{

}

void CMainFrame::onUserActiveAudioChanged(IZoomVideoSDKAudioHelper* pAudioHelper, IVideoSDKVector<IZoomVideoSDKUser*>* list)
{

}

void CMainFrame::onSessionNeedPassword(IZoomVideoSDKPasswordHandler* handler)
{

}

void CMainFrame::onSessionPasswordWrong(IZoomVideoSDKPasswordHandler* handler)
{

}

void CMainFrame::onMixedAudioRawDataReceived(AudioRawData* data_)
{
	

}

void CMainFrame::onOneWayAudioRawDataReceived(AudioRawData* data_, IZoomVideoSDKUser* pUser)
{
	
}

void CMainFrame::onSharedAudioRawDataReceived(AudioRawData* data_)
{
	
}

void CMainFrame::onUserManagerChanged(IZoomVideoSDKUser* pUser)
{

}

void CMainFrame::onUserNameChanged(IZoomVideoSDKUser* pUser)
{

}

void CMainFrame::onCameraControlRequestResult(IZoomVideoSDKUser* pUser, bool isApproved)
{

}

void CMainFrame::onCameraControlRequestReceived(IZoomVideoSDKUser* pUser, ZoomVideoSDKCameraControlRequestType requestType, IZoomVideoSDKCameraControlRequestHandler* pCameraControlRequestHandler)
{
}

void CMainFrame::onCommandReceived(IZoomVideoSDKUser* pSender, const zchar_t* strCmd)
{
	/*wstring wstrCmd(strCmd);
	if (wstrCmd.empty())
		return;

	std::vector<std::wstring> cmd_elems;
	split(wstrCmd, _T('|'), cmd_elems);

	if (cmd_elems.size() == 0)
		return;

	CmdChannelType nCmdType = (CmdChannelType)_ttoi(cmd_elems[0].c_str());

	IParseChannelCmdWnd* channelCmdWnd = NULL;
	if (cmd_channel_wnd_map_.count(nCmdType) > 0)
		channelCmdWnd = cmd_channel_wnd_map_[nCmdType];

	if (channelCmdWnd)
		channelCmdWnd->OnParseChannelCmd(pSender, cmd_elems);*/
}

void CMainFrame::onCommandChannelConnectResult(bool isSuccess)
{
	this->SetCommandChannelConnect(isSuccess);


}

void CMainFrame::onCloudRecordingStatus(RecordingStatus status, IZoomVideoSDKRecordingConsentHandler* pHandler)
{

}

void CMainFrame::onHostAskUnmute()
{
	//	CMainFrame::GetInstance().ShowMessageTipWnd(true, _T("The host would like you to unmute"));
}

void CMainFrame::onInviteByPhoneStatus(PhoneStatus status, PhoneFailedReason reason)
{

}

void CMainFrame::onMultiCameraStreamStatusChanged(ZoomVideoSDKMultiCameraStreamStatus status, IZoomVideoSDKUser* pUser, IZoomVideoSDKRawDataPipe* pVideoPipe)
{
}

void CMainFrame::onMicSpeakerVolumeChanged(unsigned int micVolume, unsigned int speakerVolume)
{

}

void CMainFrame::onAudioDeviceStatusChanged(ZoomVideoSDKAudioDeviceType type, ZoomVideoSDKAudioDeviceStatus status)
{

}

void CMainFrame::onTestMicStatusChanged(ZoomVideoSDK_TESTMIC_STATUS status)
{

}

void CMainFrame::onSelectedAudioDeviceChanged()
{

}

void CMainFrame::onLiveTranscriptionStatus(ZoomVideoSDKLiveTranscriptionStatus status)
{

}

void CMainFrame::onLiveTranscriptionMsgReceived(const zchar_t* ltMsg, IZoomVideoSDKUser* pUser, ZoomVideoSDKLiveTranscriptionOperationType type)
{
	
}

void CMainFrame::onOriginalLanguageMsgReceived(ILiveTranscriptionMessageInfo* messageInfo)
{

}

void CMainFrame::onLiveTranscriptionMsgError(ILiveTranscriptionLanguage* spokenLanguage, ILiveTranscriptionLanguage* transcriptLanguage)
{
	
}
void CMainFrame::onLiveTranscriptionMsgInfoReceived(ILiveTranscriptionMessageInfo* messageInfo) {

}
void CMainFrame::onChatPrivilegeChanged(IZoomVideoSDKChatHelper* pChatHelper, ZoomVideoSDKChatPrivilegeType privilege)
{
}
;
