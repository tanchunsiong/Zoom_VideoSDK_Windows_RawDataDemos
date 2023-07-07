#include "stdafx.h"
//#include "zoom_video_sdk_include_interface.h"
#include "main_frame.h"
#include "videosdk_demo_mgr.h"



//these are controls to demonstrate the flow

bool enableLTT = false;
bool getLang = false;

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
	std::wstring session_name = L"webchun6871";
	std::wstring sUserName = L"vsdk_skeletondemo";
	std::wstring session_password_ = L"12345678";
	std::wstring token = L"xxxxx.yyyyyy.zzzzz";


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






	if (enableLTT) {

		//the sdk will need to connect to audio to do live transcription and translation for self.
		session_context.audioOption.connect = true;
		session_context.audioOption.mute = false;
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


	

	if (enableLTT) {
		IZoomVideoSDKAudioHelper* m_pAudiohelper = ZoomVideoSDKMgr::GetInst().getAudioHelper();
		if (m_pAudiohelper) {
			// Connect User's audio.
			printf("Starting Audio\n");
			m_pAudiohelper->startAudio();
			m_pAudiohelper->unMuteAudio(0);
		}
	}
	if (enableLTT) {


		IZoomVideoSDKLiveTranscriptionHelper* m_ltthelper = ZoomVideoSDKMgr::GetInst().getLiveTranscriptionHelper();
		if (m_ltthelper != NULL)
		{
			

			if (m_ltthelper) {
				m_ltthelper->setSpokenLanguage(0);
				m_ltthelper->setTranslationLanguage(0);
				bool canstartLTT = m_ltthelper->canStartLiveTranscription();
				if (canstartLTT) {
					ZoomVideoSDKErrors err = m_ltthelper->startLiveTranscription();
					printf(">startLiveTranscription() status is : %s\n", err);
				}
			}//end if m_ltthelper
		}

	}
	if (getLang) {
		//iterate and print out language ID and language Name of supported Spoken languages 

		IZoomVideoSDKLiveTranscriptionHelper* m_ltthelper = ZoomVideoSDKMgr::GetInst().getLiveTranscriptionHelper();

		IVideoSDKVector<ILiveTranscriptionLanguage*>* availableSpokenLanguages = m_ltthelper->getAvailableSpokenLanguages();

		if (availableSpokenLanguages) {
			for (size_t i = 0; i < availableSpokenLanguages->GetCount(); ++i) {
				ILiveTranscriptionLanguage* language = availableSpokenLanguages->GetItem(i);
				if (language) {
					printf("Spoken Language ID: %d\n", language->getLTTLanguageID());
					printf("Spoken Language Name: %ls\n", language->getLTTLanguageName());
					// Print other properties as needed
					printf("---------------------\n");
				}
			}
		}


		//iterate and print out language ID and language Name of supported translation languages 


		IVideoSDKVector<ILiveTranscriptionLanguage*>* availableTranslateLanguages = m_ltthelper->getAvailableTranslationLanguages();


		if (availableTranslateLanguages) {
			for (size_t i = 0; i < availableTranslateLanguages->GetCount(); ++i) {
				ILiveTranscriptionLanguage* language = availableTranslateLanguages->GetItem(i);
				if (language) {
					printf("Translate Language ID: %d\n", language->getLTTLanguageID());
					printf("Translate Language Name: %ls\n", language->getLTTLanguageName());
					// Print other properties as needed        printf("---------------------\n");
				}
			}
		}
	}



	


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
	//we are using the onUserJoin callback to subscribe to a user's video feed
	//this is a efficient way of subscription, as there is a limit on the number of video feed which the sdk can subscribe to
	//for a rule of thumb, you should be able to subscribe up to 4 x 360p video stream per instance of SDK 



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
	/*if (pUser)
	{
		const zchar_t* szUserName = pUser->getUserName();
		ZoomVideoSDKAudioStatus audioStatus = pUser->getAudioStatus();
		if (audioStatus.isMuted)
		{

		}

		ZoomVideoSDKVideoStatus videoStatus = pUser->getVideoStatus();
	}

	if (bottom_bar_wnd_)
		bottom_bar_wnd_->OnSessionJoin();*/
}

void CMainFrame::onUserActiveAudioChanged(IZoomVideoSDKAudioHelper* pAudioHelper, IVideoSDKVector<IZoomVideoSDKUser*>* list)
{

}

void CMainFrame::onSessionNeedPassword(IZoomVideoSDKPasswordHandler* handler)
{
	/*if (!handler)
		return;

	if (join_session_wnd_)
		join_session_wnd_->OnJoinPasswordWrong();

	handler->leaveSessionIgnorePassword();*/
}

void CMainFrame::onSessionPasswordWrong(IZoomVideoSDKPasswordHandler* handler)
{
	//if (!handler)
	//	return;

	//if (join_session_wnd_)
	//	join_session_wnd_->OnJoinPasswordWrong();

	//handler->leaveSessionIgnorePassword();
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
	if (enableLTT) {

		char buffer[256];
		sprintf(buffer, "Transcription status Received is %d\n", status);
		OutputDebugStringA(buffer);
	}
}

void CMainFrame::onLiveTranscriptionMsgReceived(const zchar_t* ltMsg, IZoomVideoSDKUser* pUser, ZoomVideoSDKLiveTranscriptionOperationType type)
{
	if (enableLTT) {
		char buffer[256];
		sprintf(buffer, "Transcription Message Received is %s\n", ltMsg);
		OutputDebugStringA(buffer);
		sprintf(buffer, "Transcription Message2 Received is %ls\n", ltMsg);
		OutputDebugStringA(buffer);

		sprintf(buffer, "Transcription User Received is %ls\n", pUser->getUserName());
		OutputDebugStringA(buffer);

		sprintf(buffer, "Transcription Type Received is %d\n", type);
		OutputDebugStringA(buffer);

		//printf("Transcription Type Receieved is %s\n", type);

	}
}

void CMainFrame::onOriginalLanguageMsgReceived(ILiveTranscriptionMessageInfo* messageInfo)
{
	if (enableLTT) {
		char buffer[256];
		sprintf(buffer, "Original Language Received is %ls\n", messageInfo->getMessageContent());
		OutputDebugStringA(buffer);
	}
}

void CMainFrame::onLiveTranscriptionMsgError(ILiveTranscriptionLanguage* spokenLanguage, ILiveTranscriptionLanguage* transcriptLanguage)
{
	if (enableLTT) {
		char buffer[256];
		sprintf(buffer, "Transcription spokenLanguage Received is %ls\n", spokenLanguage->getLTTLanguageName());
		OutputDebugStringA(buffer);

		sprintf(buffer, "Transcription transcriptLanguage Received is %ls\n", transcriptLanguage->getLTTLanguageName());
		OutputDebugStringA(buffer);
	}
	//printf("Transcription transcriptLanguage Receieved is %s\n", transcriptLanguage);
}
void CMainFrame::onLiveTranscriptionMsgInfoReceived(ILiveTranscriptionMessageInfo* messageInfo) {
	if (enableLTT) {

		char buffer[256];
		sprintf(buffer, "Transcription messageType Received is %d\n", messageInfo->getMessageType());
		OutputDebugStringA(buffer);
		sprintf(buffer, "Transcription messageContent Received is %s\n", messageInfo->getMessageContent());
		OutputDebugStringA(buffer);
		sprintf(buffer, "Transcription messageContent2 Received is %ls\n", messageInfo->getMessageContent());
		OutputDebugStringA(buffer);
	}
}
void CMainFrame::onChatPrivilegeChanged(IZoomVideoSDKChatHelper* pChatHelper, ZoomVideoSDKChatPrivilegeType privilege)
{
}
;
