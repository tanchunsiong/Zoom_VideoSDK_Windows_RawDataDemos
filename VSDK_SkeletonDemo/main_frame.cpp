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

//previewCameraAndMicrophone
#include "helpers/zoom_video_sdk_video_helper_interface.h"
#include <chrono>
#include <thread>

//these are controls to demonstrate the flow
bool getRawAudio = false;
bool getRawVideo = false;
bool getRawShare = false;
bool sendRawVideo = false;  //work in progress, ignore this sample code for now
bool sendRawAudio = false;  //work in progress, ignore this sample code for now
bool sendRawShare = false;   //work in progress, ignore this sample code for now
bool enableLTT = false;
bool getLang = false;
bool enableCloudRecording = false;
bool enableCallout = false;
bool previewCameraAndMicrophone = true; //work in progress, ignore this sample code for now

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



void CMainFrame::InitControls()
{

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
	//previewCameraAndMicrophone

	//IVideoSDKVector<IZoomVideoSDKCameraDevice*> *cameras = ZoomVideoSDKMgr::GetInst().getVideoHelper()->getCameraList();

	////Get ID of selected camera
	//const zchar_t* deviceID = cameras->GetItem(0)->getDeviceId();

	////Start Preview, and handle the callback in a ZoomVideoSDKRawDataPipeDelegate instance

	//ZoomVideoSDKRawDataPipeDelegate* dataDelegate = new ZoomVideoSDKRawDataPipeDelegate();
	//ZoomVideoSDKMgr::GetInst().getVideoHelper()->startVideoPreview(dataDelegate, deviceID);

	//List all microphones
	IVideoSDKVector<IZoomVideoSDKMicDevice*>* mics = ZoomVideoSDKMgr::GetInst().getAudioHelper()->getMicList(); //not verified pseudo code

	//////Get ID of selected microphone 
	//const zchar_t* deviceID2 = mics->GetItem(2)->getDeviceId();
	//const zchar_t* deviceName2 = mics->GetItem(2)->getDeviceName();
	//////Start, stop and playback
	IZoomVideoSDKTestAudioDeviceHelper* audioDeviceHelper = ZoomVideoSDKMgr::GetInst().getAudioDeviceTestHelper();
	//audioDeviceHelper->setTimerInterval(500);
	//ZoomVideoSDKErrors err1 = audioDeviceHelper->startMicTestRecording(deviceID2);
	//std::this_thread::sleep_for(std::chrono::seconds(1));
	//ZoomVideoSDKErrors err2 = audioDeviceHelper->stopMicTestRecording();
	//ZoomVideoSDKErrors err3 = audioDeviceHelper->playMicTestRecording();
	//printf("playmictestrecording status is %d\n", err3);
	IVideoSDKVector<IZoomVideoSDKSpeakerDevice*>* speakers = ZoomVideoSDKMgr::GetInst().getAudioHelper()->getSpeakerList(); //not verified pseudo code

	IZoomVideoSDKSpeakerDevice* speaker = speakers->GetItem(0);
	const zchar_t* deviceID3 = speaker->getDeviceId();
	const zchar_t* deviceName3 = speaker->getDeviceName();

	ZoomVideoSDKErrors err4 = ZoomVideoSDKMgr::GetInst().getAudioHelper()->selectSpeaker(deviceID3, deviceName3);
	bool selectedDevice = speaker->isSelectedDevice();
	ZoomVideoSDKErrors err7 = audioDeviceHelper->startSpeakerTest(deviceID3);
	ZoomVideoSDKErrors err5 = audioDeviceHelper->startSpeakerTest(NULL);
	ZoomVideoSDKErrors err6 = audioDeviceHelper->startSpeakerTest();

	printf("startSpeakerTest status is %d\n", err7);


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


	if (getRawAudio) {

		//Optional: Virtual Audio Speaker is needed if you are intended to get raw audio from virtual microphone
		ZoomVideoSDKVirtualAudioSpeaker* vSpeaker = new ZoomVideoSDKVirtualAudioSpeaker();
		session_context.virtualAudioSpeaker = vSpeaker;

		//the sdk needs to connect to audio
		session_context.audioOption.connect = true;
	}
	if (sendRawVideo) {

		//the sdk uses a Video Source to send raw video
		//this needs to be done before joining session
		ZoomVideoSDKVideoSource* virtual_video_source = new ZoomVideoSDKVideoSource();
		session_context.externalVideoSource = virtual_video_source;
	}
	if (sendRawShare) {
		//nothing needed to be done before joining session
	}

	if (sendRawAudio) {

		//the sdk needs to connect to audio, we can also unmute beforehand
		session_context.audioOption.connect = true; //needed for sending raw audio data
		session_context.audioOption.mute = false; //needed for sending raw audio data

		//ZoomVideoSDKVirtualAudioMic is the object used to send audio
		ZoomVideoSDKVirtualAudioMic* vMic = new ZoomVideoSDKVirtualAudioMic();
		session_context.virtualAudioMic = vMic;
	}

	if (enableLTT) {

		//the sdk will need to connect to audio to do live transcription and translation for self.
		session_context.audioOption.connect = true;
		session_context.audioOption.mute = false;
	}

	if (previewCameraAndMicrophone) {
		StartPreview();
	}
	else {
		IZoomVideoSDKSession* pSession = ZoomVideoSDKMgr::GetInst().JoinSession(session_context);
		if (pSession)
		{
			pSession->getMyself()->GetVideoPipe();
		}
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


	if (getRawAudio) {
		IZoomVideoSDKAudioHelper* m_pAudiohelper = ZoomVideoSDKMgr::GetInst().getAudioHelper();
		if (m_pAudiohelper) {
			//needed for getting raw audio

			//you will need to subscribe to audio, before the callback happens next
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
			m_pAudiohelper->unMuteAudio(0);
		}
	}
	if (enableLTT) {


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
	if (getLang) {
		//iterate and print out language ID and language Name of supported Spoken languages 
		IZoomVideoSDKUser* user = ZoomVideoSDKMgr::GetInst().GetMySelf();
		IZoomVideoSDKLiveTranscriptionHelper* m_ltthelper = user->getLiveTranscriptionHelper();

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

	if (enableCloudRecording) {

		IZoomVideoSDKRecordingHelper* m_pRecordhelper = ZoomVideoSDKMgr::GetInst().getRecordingHelper();
		ZoomVideoSDKErrors err = m_pRecordhelper->canStartRecording();
		if (err == ZoomVideoSDKErrors_Success) {
			ZoomVideoSDKErrors err2 = m_pRecordhelper->startCloudRecording();
			if (err2 != ZoomVideoSDKErrors_Success) {

			}
		}

	}

	if (enableCallout) {

		IZoomVideoSDKPhoneHelper* m_phonehelper = ZoomVideoSDKMgr::GetInst().getPhoneHelper();
		if (m_phonehelper->isSupportPhoneFeature()) {
			m_phonehelper->inviteByPhone(L"+65", L"91234567", L"Alice");
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
		//TODO: do a start video here

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
	if (getRawVideo) {
		if (userList)
		{
			int count = userList->GetCount();
			for (int index = 0; index < count; index++)
			{
				IZoomVideoSDKUser* user = userList->GetItem(index);
				if (user)
				{

					//the callback for raw video frame in YUV420 format is handled in the ZoomVideoSDKRawDataPipeDelegate delegate
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
	//getting share or share screen is handled here
	//the method of getting raw share is similar to getting raw video, as it uses the same delegate ZoomVideoSDKRawDataPipeDelegate
	if (getRawShare) {
		if (status == ZoomVideoSDKShareStatus::ZoomVideoSDKShareStatus_Start)
		{
			ZoomVideoSDKRawDataPipeDelegate* pVideoPipe = new ZoomVideoSDKRawDataPipeDelegate(pUser, true);


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
	//this is the part where callback for raw audio data happens. this is the audio stream where everyone's audio has already been mixed into one channel.
	//you can choose to save the data_ buffer as PCM file, or convert it to wav / mp3 or other format. 
	//do be mindful of the compute power and memory usage which this callback can utilize
	if (getRawAudio) {

	}

}

void CMainFrame::onOneWayAudioRawDataReceived(AudioRawData* data_, IZoomVideoSDKUser* pUser)
{
	//this is the part where callback for raw audio data happens. this is the audio stream for each individual users
	//you can choose to save the data_ buffer as PCM file, or convert it to wav / mp3 or other format. 
	//do be mindful of the compute power and memory usage which this callback can utilize
	if (getRawAudio) {

	}
}

void CMainFrame::onSharedAudioRawDataReceived(AudioRawData* data_)
{
	//this is the part where callback for raw audio data happens. this is the audio stream from screensharing.
	//you can choose to save the data_ buffer as PCM file, or convert it to wav / mp3 or other format. 
	//do be mindful of the compute power and memory usage which this callback can utilize
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
	if (enableCloudRecording) {

	}
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
