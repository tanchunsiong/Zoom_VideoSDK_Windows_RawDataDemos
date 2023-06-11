#include "stdafx.h"
//#include "zoom_video_sdk_include_interface.h"
#include "main_frame.h"
#include "videosdk_demo_mgr.h"

//getRawAudio
#include "ZoomVideoSDKVirtualAudioSpeaker.h"

//savePCMtoBuffer
#include <fstream>
#include <iostream>

//these are controls to demonstrate the flow
bool getRawAudio = false;

//turning this on will prevent this sample code from joining session!
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


	if (getRawAudio) {

		//Optional: Virtual Audio Speaker is needed if you are intended to get raw audio from virtual microphone
		ZoomVideoSDKVirtualAudioSpeaker* vSpeaker = new ZoomVideoSDKVirtualAudioSpeaker();
		session_context.virtualAudioSpeaker = vSpeaker;

		//the sdk needs to connect to audio
		session_context.audioOption.connect = true;
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


	if (getRawAudio) {
		IZoomVideoSDKAudioHelper* m_pAudiohelper = ZoomVideoSDKMgr::GetInst().getAudioHelper();
		if (m_pAudiohelper) {
			//needed for getting raw audio

			//you will need to subscribe to audio, before the callback happens next
			ZoomVideoSDKErrors err = m_pAudiohelper->subscribe();
			printf("subscribe status is %d\n", err);
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
//this is a helper method, and not part of the implementation
void savePcmBufferToFile(const std::string& filename, char* pcmBuffer, std::size_t bufferSize) {
	std::ofstream outfile(filename, std::ios::out | std::ios::binary | std::ios::app);
	outfile.write(reinterpret_cast<char*>(pcmBuffer), bufferSize);
	outfile.close();
	if (!outfile) {
		std::cerr << "Error writing PCM data to file!" << std::endl;
	}
	else {
		std::cout << "PCM data saved to file: " << filename << std::endl;
	}
}
void CMainFrame::onMixedAudioRawDataReceived(AudioRawData* data_)
{
	//this is the part where callback for raw audio data happens. this is the audio stream where everyone's audio has already been mixed into one channel.
	//you can choose to save the data_ buffer as PCM file, or convert it to wav / mp3 or other format. 
	//do be mindful of the compute power and memory usage which this callback can utilize
	if (getRawAudio) {
		std::string filename = "onMixedAudioRawDataReceived.pcm";

		printf("onMixedAudioRawDataReceived\n");
		if (data_) {
			savePcmBufferToFile(filename, data_->GetBuffer(), data_->GetBufferLen());
			printf("Data buffer: %s\n", data_->GetBuffer());
			printf("Length is : %d\n", data_->GetBufferLen());
			printf("Sample is : %d\n", data_->GetSampleRate());
			printf("Channel is : %d\n", data_->GetChannelNum());
		}

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
