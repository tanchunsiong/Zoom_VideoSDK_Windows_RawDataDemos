#include <windows.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <vector>
#include <locale>
#include <codecvt>

#include "main_frame.h";
#include "zoom_video_sdk_api.h"
#include "zoom_video_sdk_def.h"
#include "zoom_video_sdk_interface.h"
#include "zoom_video_sdk_delegate_interface.h"
#include "zoom_video_sdk_session_info_interface.h"
#include "zoom_video_sdk_platform.h"



#include <json/json.h>
#include "WebService.h"

//needed for LTT
#include "zoom_video_sdk_session_info_interface.h"
#include <string>
USING_ZOOM_VIDEO_SDK_NAMESPACE
using namespace Json;
using namespace std;

wstring sdk_jwt;
wstring sessionName;;
wstring password;
IZoomVideoSDK* video_sdk_obj_;
constexpr auto CONFIG_FILE = "config.json";



bool isJWTWebService = true;

//these are flow to show developers different features

bool languageTranscriptionAndTranslation = false;	 //languageTranscriptionAndTranslation
bool getLTTSupportedLanguage = false;

wstring StringToWString(string input)
{
	wstring output;
	output.assign(input.begin(), input.end());
	return output;
}

string WStringToString(wstring input)
{
	string output;
	output.assign(input.begin(), input.end());
	return output;
}

wstring QuestionInput(string qustion)
{
	wstring input;
	std::cout << qustion;
	getline(wcin, input);
	return input;
}
/// <summary>
/// Loads configuration from config.json file.
/// This config file contains the JWT token, meeting number, passcode and mp4 file for streaming
/// </summary>
void LoadConfig() {
	ifstream f(CONFIG_FILE);
	Value config;
	printf("Trying to find meeting parameters from config.json.\n");
	bool isConfigFileOpened = f.is_open();
	if (isConfigFileOpened) {
		try {
			f >> config;
		}
		catch (exception) {
			printf("config.json is in wrong format.\n");
		}
	}
	else {
		printf("Didn't find config.json file.\n");
	}
	if (!isConfigFileOpened || config["sdk_jwt"].empty() || config["sdk_jwt"].asString() == "") {
		sdk_jwt = QuestionInput("SDK JWT: ");
	}
	else {
		sdk_jwt = StringToWString(config["sdk_jwt"].asString());
		printf("Found \"SDK JWT\" from %s: \n\"%s\"\n", CONFIG_FILE, WStringToString(sdk_jwt).c_str());
	}
	bool toQuestionForMeetingNumber = false;
	if (!isConfigFileOpened || config["sessionName"].empty() || config["sessionName"].asString() == "")
		toQuestionForMeetingNumber = true;
	else {
		try {
			sessionName = StringToWString(config["sessionName"].asString());

			printf("Found \"sessionName\" from %s: \"%s\"\n", CONFIG_FILE, WStringToString(sessionName).c_str());
		}
		catch (exception)
		{
			try {
				sessionName = StringToWString(config["sessionName"].asString());

				printf("Found \"sessionName\" from %s: \"%s\"\n", CONFIG_FILE, WStringToString(sessionName).c_str());
			}
			catch (exception)
			{
				printf("Failed to read \"sessionName\" from config.json, it should include only numbers.\n");
				toQuestionForMeetingNumber = true;
			}
		}
	}
	while (toQuestionForMeetingNumber) {
		std::wcout << "SessionName: ";
		string input;
		getline(cin, input);
		try {
			sessionName = stoull(input, nullptr, 10);
			toQuestionForMeetingNumber = false;
		}
		catch (exception)
		{
			printf("Session Name should include numbers.\n");
		}
	}
	if (!isConfigFileOpened || config["password"].empty() || config["password"].asString() == "") {
		printf("Password is empty.\n");
	}
	else {
		password = StringToWString(config["password"].asString());
		printf("Found \"password\" from %s: \"%s\"\n", CONFIG_FILE, WStringToString(password).c_str());
	}

}


void StartPreview()
{


}

MainFrame::MainFrame() {

}

// this callback happens when sdk / use has successfully joined the session
void MainFrame::onSessionJoin()
{
	std::cout << "onSessionJoin()" << std::endl;



	//languageTranscriptionAndTranslation
	if (languageTranscriptionAndTranslation) {

		//turn on audio
		IZoomVideoSDKAudioHelper* m_pAudiohelper = video_sdk_obj_->getAudioHelper();
		if (m_pAudiohelper) {
			// Connect User's audio.
			printf("Starting Audio\n");
			m_pAudiohelper->startAudio();
			m_pAudiohelper->unMuteAudio(0);
		}

		//start transcription
		IZoomVideoSDKLiveTranscriptionHelper* m_ltthelper = video_sdk_obj_->getLiveTranscriptionHelper();
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
	//getLTTSupportedLanguage
	if (getLTTSupportedLanguage) {
		//iterate and print out language ID and language Name of supported Spoken languages 

		IZoomVideoSDKLiveTranscriptionHelper* m_ltthelper = video_sdk_obj_->getLiveTranscriptionHelper();

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
void MainFrame::onSessionLeave() {}
void MainFrame::onError(ZoomVideoSDKErrors errorCode, int detailErrorCode)
{
	std::cout << "onError(), errorCode " << errorCode << " detailedErrorCode: " << detailErrorCode << std::endl;

}
void MainFrame::onUserJoin(IZoomVideoSDKUserHelper* pUserHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{

}
void MainFrame::onUserLeave(IZoomVideoSDKUserHelper* pUserHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList) {}
void MainFrame::onUserVideoStatusChanged(IZoomVideoSDKVideoHelper* pVideoHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList) {


}
void MainFrame::onUserAudioStatusChanged(IZoomVideoSDKAudioHelper* pAudioHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList) {}
void MainFrame::onUserShareStatusChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, ZoomVideoSDKShareStatus status, ZoomVideoSDKShareType type) {
	std::cout << "onUserShareStatusChanged()" << std::endl;

}
void MainFrame::onCalloutJoinSuccess(IZoomVideoSDKUser* pUser, const zchar_t* phoneNumber) {}
void MainFrame::onUserRecordingConsent(IZoomVideoSDKUser* pUser) {}
void MainFrame::onLiveStreamStatusChanged(IZoomVideoSDKLiveStreamHelper* pLiveStreamHelper, ZoomVideoSDKLiveStreamStatus status) {}
void MainFrame::onChatNewMessageNotify(IZoomVideoSDKChatHelper* pChatHelper, IZoomVideoSDKChatMessage* messageItem)
{

}
void MainFrame::onUserHostChanged(IZoomVideoSDKUserHelper* pUserHelper, IZoomVideoSDKUser* pUser) {}
void MainFrame::onUserActiveAudioChanged(IZoomVideoSDKAudioHelper* pAudioHelper, IVideoSDKVector<IZoomVideoSDKUser*>* list) {}
void MainFrame::onSessionNeedPassword(IZoomVideoSDKPasswordHandler* handler) {}
void MainFrame::onSessionPasswordWrong(IZoomVideoSDKPasswordHandler* handler) {}
void MainFrame::onMixedAudioRawDataReceived(AudioRawData* data_) {

}
void MainFrame::onOneWayAudioRawDataReceived(AudioRawData* data_, IZoomVideoSDKUser* pUser) {

}
void MainFrame::onSharedAudioRawDataReceived(AudioRawData* data_) {


}
void MainFrame::onUserManagerChanged(IZoomVideoSDKUser* pUser) {}
void MainFrame::onUserNameChanged(IZoomVideoSDKUser* pUser) {}
void MainFrame::onCameraControlRequestResult(IZoomVideoSDKUser* pUser, bool isApproved) {}
void MainFrame::onCameraControlRequestReceived(IZoomVideoSDKUser* pUser, ZoomVideoSDKCameraControlRequestType requestType, IZoomVideoSDKCameraControlRequestHandler* pCameraControlRequestHandler) {}
void MainFrame::onCommandReceived(IZoomVideoSDKUser* pSender, const zchar_t* strCmd)
{

}

void MainFrame::onCommandChannelConnectResult(bool isSuccess) {}
void MainFrame::onInviteByPhoneStatus(PhoneStatus status, PhoneFailedReason reason) {}
void MainFrame::onCloudRecordingStatus(RecordingStatus status, IZoomVideoSDKRecordingConsentHandler* pHandler) {

}
void MainFrame::onHostAskUnmute() {}
void MainFrame::onMultiCameraStreamStatusChanged(ZoomVideoSDKMultiCameraStreamStatus status, IZoomVideoSDKUser* pUser, IZoomVideoSDKRawDataPipe* pVideoPipe) {


}
void MainFrame::onMicSpeakerVolumeChanged(unsigned int micVolume, unsigned int speakerVolume) {}
void MainFrame::onAudioDeviceStatusChanged(ZoomVideoSDKAudioDeviceType type, ZoomVideoSDKAudioDeviceStatus status) {}
void MainFrame::onTestMicStatusChanged(ZoomVideoSDK_TESTMIC_STATUS status) {


}
void MainFrame::onSelectedAudioDeviceChanged() {}
void MainFrame::onCameraListChanged()
{
}
void MainFrame::onLiveTranscriptionStatus(ZoomVideoSDKLiveTranscriptionStatus status) {
	if (languageTranscriptionAndTranslation) {
		char buffer[256];
		printf(buffer, "Transcription status Received is %d\n", status);
		OutputDebugStringA(buffer);
	}
}
void MainFrame::onLiveTranscriptionMsgReceived(const zchar_t* ltMsg, IZoomVideoSDKUser* pUser, ZoomVideoSDKLiveTranscriptionOperationType type) {
	if (languageTranscriptionAndTranslation) {
		char buffer[256];
		printf(buffer, "Transcription Message Received is %s\n", ltMsg);
		OutputDebugStringA(buffer);
		printf(buffer, "Transcription Message2 Received is %ls\n", ltMsg);
		OutputDebugStringA(buffer);
		printf(buffer, "Transcription User Received is %ls\n", pUser->getUserName());
		OutputDebugStringA(buffer);
		printf(buffer, "Transcription Type Received is %d\n", type);
		OutputDebugStringA(buffer);

		//printf("Transcription Type Receieved is %s\n", type);

	}
}
void MainFrame::onOriginalLanguageMsgReceived(ILiveTranscriptionMessageInfo* messageInfo) {
	if (languageTranscriptionAndTranslation) {
		char buffer[256];
		printf(buffer, "Original Language Received is %ls\n", messageInfo->getMessageContent());
		OutputDebugStringA(buffer);
	}
}
void MainFrame::onLiveTranscriptionMsgInfoReceived(ILiveTranscriptionMessageInfo* messageInfo) {
	if (languageTranscriptionAndTranslation) {
		char buffer[256];
		printf(buffer, "Transcription messageType Received is %d\n", messageInfo->getMessageType());
		OutputDebugStringA(buffer);
		printf(buffer, "Transcription messageContent Received is %s\n", messageInfo->getMessageContent());
		OutputDebugStringA(buffer);
		printf(buffer, "Transcription messageContent2 Received is %ls\n", messageInfo->getMessageContent());
		OutputDebugStringA(buffer);
	}
}
void MainFrame::onLiveTranscriptionMsgError(ILiveTranscriptionLanguage* spokenLanguage, ILiveTranscriptionLanguage* transcriptLanguage) {
	if (languageTranscriptionAndTranslation) {
		char buffer[256];
		printf(buffer, "Transcription spokenLanguage Received is %ls\n", spokenLanguage->getLTTLanguageName());
		OutputDebugStringA(buffer);

		printf(buffer, "Transcription transcriptLanguage Received is %ls\n", transcriptLanguage->getLTTLanguageName());
		OutputDebugStringA(buffer);
	}
	//printf("Transcription transcriptLanguage Receieved is %s\n", transcriptLanguage);
}
void MainFrame::onChatMsgDeleteNotification(IZoomVideoSDKChatHelper* pChatHelper, const zchar_t* msgID, ZoomVideoSDKChatMessageDeleteType deleteBy) {}
void MainFrame::onChatPrivilegeChanged(IZoomVideoSDKChatHelper* pChatHelper, ZoomVideoSDKChatPrivilegeType privilege) {}
void MainFrame::onSendFileStatus(IZoomVideoSDKSendFile* file, const FileTransferStatus& status) {}
void MainFrame::onReceiveFileStatus(IZoomVideoSDKReceiveFile* file, const FileTransferStatus& status) {}
void MainFrame::onProxyDetectComplete() {}
void MainFrame::onProxySettingNotification(IZoomVideoSDKProxySettingHandler* handler) {}
void MainFrame::onSSLCertVerifiedFailNotification(IZoomVideoSDKSSLCertificateInfo* info) {}
void MainFrame::onUserVideoNetworkStatusChanged(ZoomVideoSDKNetworkStatus status, IZoomVideoSDKUser* pUser) {}
void MainFrame::onCallCRCDeviceStatusChanged(ZoomVideoSDKCRCCallStatus status) {}
void MainFrame::onVideoCanvasSubscribeFail(ZoomVideoSDKSubscribeFailReason fail_reason, IZoomVideoSDKUser* pUser, void* handle) {}
void MainFrame::onShareCanvasSubscribeFail(ZoomVideoSDKSubscribeFailReason fail_reason, IZoomVideoSDKUser* pUser, void* handle) {}
void MainFrame::onAnnotationHelperCleanUp(IZoomVideoSDKAnnotationHelper* helper) {}
void MainFrame::onAnnotationPrivilegeChange(IZoomVideoSDKUser* pUser, bool enable) {}
void MainFrame::onAnnotationHelperActived(void* handle) {}
void MainFrame::onVideoAlphaChannelStatusChanged(bool isAlphaModeOn) {}
void MainFrame::InitVideoSDK()
{
	LoadConfig();

	if (isJWTWebService) {

		sdk_jwt = StringToWString(GetSignatureFromWebService("https://asdc.cc/video", WStringToString(sessionName), "1"));
		std::cout << "token from webservice is :" << WStringToString(sdk_jwt).c_str() << std::endl;
	}
	std::cout << "InitVideoSDK()" << std::endl;
	video_sdk_obj_ = ZOOMVIDEOSDK::CreateZoomVideoSDKObj();
	video_sdk_obj_->addListener(this);
	ZoomVideoSDKInitParams init_params;
	init_params.domain = L"https://zoom.us";
	init_params.enableLog = true;
	init_params.logFilePrefix = L"zoom_win_video_demo";
	init_params.videoRawDataMemoryMode = ZoomVideoSDKRawDataMemoryModeHeap;
	init_params.shareRawDataMemoryMode = ZoomVideoSDKRawDataMemoryModeHeap;
	init_params.audioRawDataMemoryMode = ZoomVideoSDKRawDataMemoryModeHeap;
	init_params.enableIndirectRawdata = false;



	ZoomVideoSDKErrors err = video_sdk_obj_->initialize(init_params);
	std::cout << "initialization result is :" << err << std::endl;
}

void MainFrame::UninitVideoSDK()
{
	video_sdk_obj_->cleanup();
	DestroyZoomVideoSDKObj();
	video_sdk_obj_ = nullptr;
}

void MainFrame::JoinSession()
{
	std::cout << "JoinSession()" << std::endl;
	//credentials to auth and password & topic name to join specific session
	std::wstring session_name = sessionName;
	std::wstring sUserName = L"vsdk_skeletondemo_receiver";
	std::wstring session_password_ = password;
	std::wstring token = sdk_jwt;


	bool is_video_on = false;  //turn off video
	bool is_mute_audio = true; //mute audio

	//session context 
	ZoomVideoSDKSessionContext session_context;
	session_context.sessionName = session_name.c_str();
	session_context.sessionPassword = session_password_.c_str();
	session_context.userName = sUserName.c_str();
	session_context.token = token.c_str();
	session_context.videoOption.localVideoOn = is_video_on;
	session_context.audioOption.connect = false;
	session_context.audioOption.mute = is_mute_audio;


	//languageTranscriptionAndTranslation
	if (languageTranscriptionAndTranslation) {
		//the sdk will need to connect to audio to do live transcription and translation for self.
		session_context.audioOption.connect = true;
		session_context.audioOption.mute = false;
	}


	IZoomVideoSDKSession* pSession = video_sdk_obj_->joinSession(session_context);
	if (pSession)
	{
		pSession->getMyself()->GetVideoPipe();
	}

}

void MainFrame::LeaveSession(bool end)
{
	ZoomVideoSDKErrors err = video_sdk_obj_->leaveSession(end);
}







