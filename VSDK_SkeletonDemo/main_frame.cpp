#include "stdafx.h"
//#include "zoom_video_sdk_include_interface.h"
#include "main_frame.h"
#include "videosdk_demo_mgr.h"

//getRawShare, getRawVideo
#include "ZoomVideoSDKRawDataPipeDelegate.h"

//getRawAudio
#include "ZoomVideoSDKVirtualAudioSpeaker.h"

//sendRawVideo
#include "ZoomVideoSDKVideoSource.h"

//sendRawAudio
#include "ZoomVideoSDKVirtualAudioMic.h"

//sendRawShare
#include "ZoomVideoSDKShareSource.h"

//these are controls to demonstrate the flow
bool getRawAudio = false;
bool getRawVideo = false;
bool getRawShare = false;
bool sendRawVideo = false;
bool sendRawAudio = false;
bool sendRawShare = false;
bool enableLTT = true;
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



void CMainFrame::InitControls()
{

}



void CMainFrame::OnLeaveSessionUIUpdate()
{

	StopShare();

	//ShowWindow(false);
}

void CMainFrame::OnJoinMettingJoinFailed(int error_code)
{
	
}

void CMainFrame::OnInvalidParamterNotification()
{
	
}

void CMainFrame::OnMeetingDisconnecting()
{
	

	StartPreview();
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


		std::wstring session_name = L"webchun6871";
		std::wstring sUserName = L"vsdk_skeletondemo";
		std::wstring session_password_ = L"12345678";
		std::wstring token = L"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJhcHBfa2V5IjoiY0FzWUFCcThST1p0cWtnS1lLVUxSRjVTa2o5aWpVdzBXMGlZIiwidmVyc2lvbiI6MSwicm9sZV90eXBlIjoxLCJ1c2VyX2lkZW50aXR5IjoiVXNlciBJRCBGcm9tIFB5dGhvbiBTY3JpcHQiLCJzZXNzaW9uX2tleSI6IiIsImlhdCI6MTY4MzUxNzE0OSwiZXhwIjoxNjgzNjg5OTQ5LCJ0cGMiOiJ3ZWJjaHVuNjg3MSJ9.TmPxzaNed10Css-Gyii5RhC0IgMfFfy9p4b5Pbl-fGM";


		//is turn off video
		bool is_video_on = false;
		//is mute audio
		bool is_mute_audio = true;

		ZoomVideoSDKSessionContext session_context;
		session_context.sessionName = session_name.c_str();
		session_context.sessionPassword = session_password_.c_str();
		session_context.userName = sUserName.c_str();
		session_context.token = token.c_str();
		session_context.videoOption.localVideoOn = is_video_on;
		session_context.audioOption.connect = false;
		session_context.audioOption.mute = is_mute_audio;

		if (getRawAudio) {
		
			ZoomVideoSDKVirtualAudioSpeaker* vSpeaker = new ZoomVideoSDKVirtualAudioSpeaker();
			session_context.virtualAudioSpeaker = vSpeaker;

			session_context.audioOption.connect = true;
		}
		if (sendRawVideo) {
			//needed for send raw video
			//the sdk uses a Video Source to send raw video
			//this needs to be done before joining session
			ZoomVideoSDKVideoSource* virtual_video_source = new ZoomVideoSDKVideoSource();
			session_context.externalVideoSource = virtual_video_source;
		}
		if (sendRawShare) {
			//nothing much to do before joining session
		}


		if (sendRawAudio) {
			session_context.audioOption.connect = true; //needed for sending raw audio data
			session_context.audioOption.mute = false; //needed for sending raw audio data

			//ZoomVideoSDKVirtualAudioMic is the object used to send audio
			ZoomVideoSDKVirtualAudioMic* vMic = new ZoomVideoSDKVirtualAudioMic();
			session_context.virtualAudioMic = vMic;

		}

		if (enableLTT) {
			session_context.audioOption.connect = true; //needed for sending raw audio data
			session_context.audioOption.mute = false; //needed for sending raw audio data

		}

	IZoomVideoSDKSession* pSession = ZoomVideoSDKMgr::GetInst().JoinSession(session_context);
	if (pSession)
	{
		pSession->getMyself()->GetVideoPipe();
		
		if (video_show_mgr_)
		{
		
		}

		StartPreview();

	}
}

void CMainFrame::LeaveSession(bool end)
{
	ZoomVideoSDKErrors err = ZoomVideoSDKMgr::GetInst().LeaveSession(end);
}



void CMainFrame::onSessionJoin()
{
	
	std::cout << "onSessionJoin()" << std::endl;

	if (getRawAudio) {
		IZoomVideoSDKAudioHelper* m_pAudiohelper = ZoomVideoSDKMgr::GetInst().getAudioHelper();
		if (m_pAudiohelper) {
			//needed for getting raw audio
			ZoomVideoSDKErrors err = m_pAudiohelper->subscribe();
			printf("subscribe status is %d\n", err);
		}
	}


	if (sendRawAudio) {
		//needed for audio
		IZoomVideoSDKAudioHelper* m_pAudiohelper = ZoomVideoSDKMgr::GetInst().getAudioHelper();
		if (m_pAudiohelper) {
			// Connect User's audio.
			printf("Starting Audio\n");
			m_pAudiohelper->startAudio();



		}
	}

	if (enableLTT) {
		IZoomVideoSDKAudioHelper* m_pAudiohelper = ZoomVideoSDKMgr::GetInst().getAudioHelper();
		if (m_pAudiohelper) {
			// Connect User's audio.
			printf("Starting Audio\n");
			m_pAudiohelper->startAudio();

		}
	}
	if (enableLTT) {

		//vector<IZoomVideoSDKUser*> remoteUsers = ZoomVideoSDKMgr::GetInst().GetAllUsers();

		IZoomVideoSDKUser* user = ZoomVideoSDKMgr::GetInst().GetMySelf();
		if (user != NULL)
		{

			IZoomVideoSDKLiveTranscriptionHelper* m_ltthelper = user->getLiveTranscriptionHelper();
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

	if (sendRawVideo) {
		//dreamtcs todo: do a start video here
		
	}

	if (sendRawAudio) {
		//dreamtcs todo: do a start / unmute audio here
		IZoomVideoSDKAudioHelper* m_pAudiohelper = ZoomVideoSDKMgr::GetInst().getAudioHelper();
		if (m_pAudiohelper) {
			// Connect User's audio.
			printf("Starting Audio\n");
			m_pAudiohelper->unMuteAudio(0);



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
		OnJoinMettingJoinFailed(errorCode);
	}
}

void CMainFrame::onUserJoin(IZoomVideoSDKUserHelper* pUserHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{
	std::cout << "onUserJoin()" << std::endl;
	if (getRawVideo) {
		if (userList)
		{
			int count = userList->GetCount();
			for (int index = 0; index < count; index++)
			{
				IZoomVideoSDKUser* user = userList->GetItem(index);
				if (user)
				{
					ZoomVideoSDKRawDataPipeDelegate* encoder = new ZoomVideoSDKRawDataPipeDelegate(user);
				}

			}
		}
	}


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
	if (getRawShare) {
		if (status== ZoomVideoSDKShareStatus::ZoomVideoSDKShareStatus_Start)
		{
			ZoomVideoSDKRawDataPipeDelegate *pVideoPipe = new ZoomVideoSDKRawDataPipeDelegate(pUser,true);
			
			
		}
		else if (status == ZoomVideoSDKShareStatus::ZoomVideoSDKShareStatus_Stop)
		{
			ZoomVideoSDKRawDataPipeDelegate::stop_encoding_for(pUser, true);
		}
	
	}
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
	//dreamtcs to set getRawAudio to true and test if this fires
	if (getRawAudio) {
	
	}

}

void CMainFrame::onOneWayAudioRawDataReceived(AudioRawData* data_, IZoomVideoSDKUser* pUser)
{
	//dreamtcs to set getRawAudio to true and test if this fires
	if (getRawAudio) {

	}
}

void CMainFrame::onSharedAudioRawDataReceived(AudioRawData* data_)
{
	//dreamtcs to set getRawAudio to true and test if this fires
	if (getRawAudio) {

	}
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
 };
