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


//needed for getRawShare, getRawVideo
#include "ZoomVideoSDKRawDataPipeDelegate.h"
#include "helpers/zoom_video_sdk_video_helper_interface.h"

#include <json/json.h>
#include "WebService.h"


USING_ZOOM_VIDEO_SDK_NAMESPACE
using namespace Json;
using namespace std;

wstring sdk_jwt;
wstring sessionName;
wstring password;
wstring jwt_webservice_url;
IZoomVideoSDK* video_sdk_obj_;
constexpr auto CONFIG_FILE = "config.json";



bool isJWTWebService = false;

//these are flow to show developers different features
bool getRawVideo = true; //getRawVideo


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

	//JWT Token
	if (!isConfigFileOpened || config["sdk_jwt"].empty() || config["sdk_jwt"].asString() == "") {
		sdk_jwt = QuestionInput("SDK JWT: ");
	}
	else {
		sdk_jwt = StringToWString(config["sdk_jwt"].asString());
		printf("Found \"SDK JWT\" from %s: \n\"%s\"\n", CONFIG_FILE, WStringToString(sdk_jwt).c_str());
	}

	//WS URL
	if (!isConfigFileOpened || config["jwt_webservice_url"].empty() || config["jwt_webservice_url"].asString() == "") {
		jwt_webservice_url = QuestionInput("JWT Webservice URL: ");
	}
	else {
		jwt_webservice_url = StringToWString(config["jwt_webservice_url"].asString());
		printf("Found \"JWT Webservice URL\" from %s: \n\"%s\"\n", CONFIG_FILE, WStringToString(jwt_webservice_url).c_str());
	}
	//Session Name
	bool toQuestionForMeetingNumber = false;
	if (!isConfigFileOpened || config["sessionName"].empty() || config["sessionName"].asString() == "")
		toQuestionForMeetingNumber = true;
	else {
		try {
			sessionName = StringToWString(config["sessionName"].asString());
			printf("Found \"sessionName\" from %s: \"%s\"\n", CONFIG_FILE, WStringToString(sessionName).c_str());
		}
		catch (exception) {
			try {
				sessionName = StringToWString(config["sessionName"].asString());
				printf("Found \"sessionName\" from %s: \"%s\"\n", CONFIG_FILE, WStringToString(sessionName).c_str());
			}
			catch (exception) {
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
		catch (exception) {
			printf("Session Name should include numbers.\n");
		}
	}

	//Password
	if (!isConfigFileOpened || config["password"].empty() || config["password"].asString() == "") {
		printf("Password is empty.\n");
	}
	else {
		password = StringToWString(config["password"].asString());
		printf("Found \"password\" from %s: \"%s\"\n", CONFIG_FILE, WStringToString(password).c_str());
	}

	if (!isConfigFileOpened || config["useJWTTokenFromWebService"].empty() || config["useJWTTokenFromWebService"].asString() == "") {
		printf("useJWTTokenFromWebService is invalid, should be true or false.\n");
	}
	else {
		string wStrUseJWTTokenFromWebService = config["useJWTTokenFromWebService"].asString();
		if (wStrUseJWTTokenFromWebService == "true") {
			isJWTWebService = true;
			printf("Found \"isJWTWebService\" from %s: \"%s\"\n", CONFIG_FILE, "true");
		}

		else {
			isJWTWebService = false;
			printf("Found \"isJWTWebService\" from %s: \"%s\"\n", CONFIG_FILE, "false");
		}
	}




}

void StartMicRecording()
{
	//Get list of all microphones
	IVideoSDKVector<IZoomVideoSDKMicDevice*>* mics = video_sdk_obj_->getAudioHelper()->getMicList();

	//Get ID of selected microphone 
	const zchar_t* deviceID2 = mics->GetItem(0)->getDeviceId();
	const zchar_t* deviceName2 = mics->GetItem(0)->getDeviceName();

	video_sdk_obj_->getAudioHelper()->selectMic(deviceID2, deviceName2);

	//Start recording
	IZoomVideoSDKTestAudioDeviceHelper* audioDeviceHelper2 = video_sdk_obj_->GetAudioDeviceTestHelper();


	//ZoomVideoSDKErrors err = audioDeviceHelper2->startMicTestRecording(deviceID2);
	ZoomVideoSDKErrors err = audioDeviceHelper2->startMicTestRecording();
	printf("StartMicRecording status is %d\n", err);

}

void StopMicRecording()
{

	//Stop Recording
	IZoomVideoSDKTestAudioDeviceHelper* audioDeviceHelper2 = video_sdk_obj_->GetAudioDeviceTestHelper();


	ZoomVideoSDKErrors err = audioDeviceHelper2->stopMicTestRecording();
	printf("StopMicRecording status is %d\n", err);

}

void PlayMicRecording()
{
	////Start playback
	IZoomVideoSDKTestAudioDeviceHelper* audioDeviceHelper2 = video_sdk_obj_->GetAudioDeviceTestHelper();


	ZoomVideoSDKErrors err = audioDeviceHelper2->playMicTestRecording();
	printf("PlayMicRecording status is %d\n", err);

}
void StartVideoTest() {

	//Get list of all cameras
	IVideoSDKVector<IZoomVideoSDKCameraDevice*>* cameras = video_sdk_obj_->getVideoHelper()->getCameraList();

	//Get ID of selected camera
	const zchar_t* deviceID = cameras->GetItem(0)->getDeviceId();

	//Start Preview, and handle the callback in a ZoomVideoSDKRawDataPipeDelegate instance
	ZoomVideoSDKRawDataPipeDelegate* dataDelegate = new ZoomVideoSDKRawDataPipeDelegate();
	video_sdk_obj_->getVideoHelper()->startVideoPreview(dataDelegate, deviceID);

}
void StartSpeakerTest()
{

	//speaker test here
	IZoomVideoSDKTestAudioDeviceHelper* audioDeviceHelper = video_sdk_obj_->GetAudioDeviceTestHelper();
	IVideoSDKVector<IZoomVideoSDKSpeakerDevice*>* speakers = video_sdk_obj_->getAudioHelper()->getSpeakerList(); //not verified pseudo code

	IZoomVideoSDKSpeakerDevice* speaker = speakers->GetItem(0);
	const zchar_t* deviceID3 = speaker->getDeviceId();
	const zchar_t* deviceName3 = speaker->getDeviceName();

	ZoomVideoSDKErrors err4 = video_sdk_obj_->getAudioHelper()->selectSpeaker(deviceID3, deviceName3);
	bool selectedDevice = speaker->isSelectedDevice();
	//ZoomVideoSDKErrors err = audioDeviceHelper->startSpeakerTest(deviceID3);
	//printf("startSpeakerTest status is %d\n", err4);
}
void StopSpeakerTest()
{
	IZoomVideoSDKTestAudioDeviceHelper* audioDeviceHelper = video_sdk_obj_->GetAudioDeviceTestHelper();
	ZoomVideoSDKErrors err = audioDeviceHelper->stopSpeakerTest();
}

void StartPreview()
{

	//previewCameraAndMicrophone

	//StartVideoTest();
	StartSpeakerTest();

	StartMicRecording();

}

MainFrame::MainFrame() {

}

// this callback happens when sdk / use has successfully joined the session
void MainFrame::onSessionJoin()
{
	std::cout << "onSessionJoin()" << std::endl;





}
void MainFrame::onSessionLeave() {}
void MainFrame::onError(ZoomVideoSDKErrors errorCode, int detailErrorCode)
{
	std::cout << "onError(), errorCode " << errorCode << " detailedErrorCode: " << detailErrorCode << std::endl;

}
void MainFrame::onUserJoin(IZoomVideoSDKUserHelper* pUserHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{
	std::cout << "onUserJoin()" << std::endl;
	//we are using the onUserJoin callback to subscribe to a user's video feed
	//this is a efficient way of subscription, as there is a limit on the number of video feed which the sdk can subscribe to
	//for a rule of thumb, you should be able to subscribe up to 4 x 360p video stream per instance of SDK 

	//getRawVideo
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
void MainFrame::onUserLeave(IZoomVideoSDKUserHelper* pUserHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList) {}
void MainFrame::onUserVideoStatusChanged(IZoomVideoSDKVideoHelper* pVideoHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList) {
	printf("onUserVideoStatusChanged\n");


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
	//printf("onTestMicStatusChanged status Received is %s\n", status);




}
void MainFrame::onSelectedAudioDeviceChanged() {}
void MainFrame::onCameraListChanged()
{
}
void MainFrame::onLiveTranscriptionStatus(ZoomVideoSDKLiveTranscriptionStatus status) {

}
void MainFrame::onLiveTranscriptionMsgReceived(const zchar_t* ltMsg, IZoomVideoSDKUser* pUser, ZoomVideoSDKLiveTranscriptionOperationType type) {

}
void MainFrame::onOriginalLanguageMsgReceived(ILiveTranscriptionMessageInfo* messageInfo) {

}
void MainFrame::onLiveTranscriptionMsgInfoReceived(ILiveTranscriptionMessageInfo* messageInfo) {

}
void MainFrame::onLiveTranscriptionMsgError(ILiveTranscriptionLanguage* spokenLanguage, ILiveTranscriptionLanguage* transcriptLanguage) {

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

		sdk_jwt = StringToWString(GetSignatureFromWebService(WStringToString(jwt_webservice_url), WStringToString(sessionName), "1"));
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







