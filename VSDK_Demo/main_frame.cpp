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
#include "helpers/zoom_video_sdk_audio_helper_interface.h"
#include "helpers/zoom_video_sdk_video_helper_interface.h"
#include "helpers/zoom_video_sdk_share_helper_interface.h"
#include "helpers/zoom_video_sdk_chat_helper_interface.h"
#include "helpers/zoom_video_sdk_cmd_channel_interface.h"
#include "helpers/zoom_video_sdk_user_helper_interface.h"
#include "zoom_video_sdk_platform.h"



//needed for sendRawAudio
#include "ZoomVideoSDKVirtualAudioMic.h" 
#include "helpers/zoom_video_sdk_audio_send_rawdata_interface.h"

//neede for getRawAudio
#include "ZoomVideoSDKVirtualAudioSpeaker.h"

//needed for sendRawShare
#include "ZoomVideoSDKShareSource.h"
#include "helpers/zoom_video_sdk_share_helper_interface.h"

//needed for sendRawVideo
#include "ZoomVideoSDKVideoSource.h" 
#include "helpers/zoom_video_sdk_video_helper_interface.h"

//needed for getRawShare, getRawVideo
#include "ZoomVideoSDKRawDataPipeDelegate.h"
#include "helpers/zoom_video_sdk_video_helper_interface.h"

//needed for command channel
#include "helpers/zoom_video_sdk_cmd_channel_interface.h"

//needed for chat
#include "helpers/zoom_video_sdk_chat_helper_interface.h"
#include "zoom_video_sdk_chat_message_interface.h"

//needed for LTT
#include "zoom_video_sdk_session_info_interface.h"
#include <string>

//needed for previewCameraAndMicrophone
#include "helpers/zoom_video_sdk_video_helper_interface.h"
#include "helpers/zoom_video_sdk_audio_helper_interface.h"

#include <json/json.h>
#include "WebService.h"

//testing for framerates
#include "ZoomVideoSDKRawDataPipeDelegateMultiStream.h"
#include "ZoomVideoSDKRawDataPipeDelegateShare.h"

USING_ZOOM_VIDEO_SDK_NAMESPACE
using namespace Json;
using namespace std;

wstring sdk_jwt;
wstring sessionName;;
wstring password;
IZoomVideoSDK* video_sdk_obj_;
constexpr auto CONFIG_FILE = "config.json";

 wstring DEFAULT_VIDEO_SOURCE = L"Big_Buck_Bunny_720_10s_1MB.mp4"; //sendRawVideo
ZoomVideoSDKVideoSource* virtual_camera_video_source;  //sendRawVideo


bool isJWTWebService = true;

//these are flow to show developers different features
bool serviceQuality = true;					     //serviceQuality
bool languageTranscriptionAndTranslation = false;	 //languageTranscriptionAndTranslation
bool getLTTSupportedLanguage = false;				 //getLTTSupportedLanguage
bool enableCloudRecording = false;					 //enableCloudRecording
bool enableCallout = false;							 //enableCallout
bool previewCameraAndMicrophone = false;			 //previewCameraAndMicrophone :work in progress, ignore this sample code for now

bool sendRawVideo = false; //sendRawVideo 
bool sendRawShare = false;//sendRawShare "work in progress, ignore this sample code for now, this shares code with sendRawVideo
bool sendRawAudio = false; //sendRawAudio

bool getRawVideo = true; //getRawVideo
bool getRawShare = false; //getRawShare
bool getRawAudio = false; //getRawAudio

bool sendVideo = true;
bool multiStreamVideo = true; 
bool subscribeMultiCameraStream = false;

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

//getRawAudio
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
	IVideoSDKVector<IZoomVideoSDKSpeakerDevice*>* speakers =video_sdk_obj_->getAudioHelper()->getSpeakerList(); //not verified pseudo code

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

	//servicequality
	if (serviceQuality) {
		IZoomVideoSDKSession* pSession = video_sdk_obj_->getSessionInfo();
		if (pSession)
		{

			ZoomVideoSDKSessionASVStatisticInfo sendInfo;
			ZoomVideoSDKSessionASVStatisticInfo recvInfo;
			pSession->getSessionVideoStatisticInfo(sendInfo, recvInfo);

			ZoomVideoSDKSessionAudioStatisticInfo audioSendInfo;
			ZoomVideoSDKSessionAudioStatisticInfo audiorecvInfo;
			pSession->getSessionAudioStatisticInfo(audioSendInfo, audiorecvInfo);

			ZoomVideoSDKSessionASVStatisticInfo shareSendInfo;
			ZoomVideoSDKSessionASVStatisticInfo shareRecvInfo;
			pSession->getSessionShareStatisticInfo(shareSendInfo, shareRecvInfo);

		}
	}

	//dreamtcs todo
	//sendRawVideo
	if (sendRawVideo) {
	
		video_sdk_obj_->getVideoHelper()->startVideo();
	}

	//sendRawShare
	if (sendRawShare) {

		//needed for share source
		//this needs to be done after joing session
		ZoomVideoSDKShareSource* virtual_share_source = new ZoomVideoSDKShareSource();
		ZoomVideoSDKErrors err2 = video_sdk_obj_->getShareHelper()->startSharingExternalSource(virtual_share_source);
		if (err2 == ZoomVideoSDKErrors_Success) {
		}
		else {
			printf("Error setting external source %s\n", err2);
		}
	}

	//sendRawAudio
	if (sendRawAudio) {
		//needed for audio
		IZoomVideoSDKAudioHelper* m_pAudiohelper = video_sdk_obj_->getAudioHelper();
		if (m_pAudiohelper) {
		
			// Connect User's audio.
			printf("Starting Audio\n");
			m_pAudiohelper->startAudio();
		}
	}

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

	//getRawAudio
	if (getRawAudio) {
		IZoomVideoSDKAudioHelper* m_pAudiohelper = video_sdk_obj_->getAudioHelper();
		if (m_pAudiohelper) {
			//needed for getting raw audio

			//you will need to subscribe to audio, before the callback happens next
			ZoomVideoSDKErrors err = m_pAudiohelper->subscribe();
			printf("subscribe status is %d\n", err);
		}
	}

	//enableCloudRecording
	if (enableCloudRecording) {

		IZoomVideoSDKRecordingHelper* m_pRecordhelper = video_sdk_obj_->getRecordingHelper();
		ZoomVideoSDKErrors err = m_pRecordhelper->canStartRecording();
		if (err == ZoomVideoSDKErrors_Success) {
			ZoomVideoSDKErrors err2 = m_pRecordhelper->startCloudRecording();
			if (err2 != ZoomVideoSDKErrors_Success) {

			}
		}

	}
	//enableCallout
	if (enableCallout) {

		IZoomVideoSDKPhoneHelper* m_phonehelper = video_sdk_obj_->getPhoneHelper();
		if (m_phonehelper->isSupportPhoneFeature()) {
			m_phonehelper->inviteByPhone(L"+65", L"91234567", L"Alice");
		}
	}


	if (multiStreamVideo) {
	IZoomVideoSDKVideoHelper* videohelper=	video_sdk_obj_->getVideoHelper();


	ZoomVideoSDKVideoPreferenceSetting setting;
	setting.mode = ZoomVideoSDKVideoPreferenceMode_Custom;
	setting.minimum_frame_rate = 20;
	setting.maximum_frame_rate = 30;
	videohelper->setVideoQualityPreference(setting);


	bool isSuccess=videohelper->enableMultiStreamVideo(videohelper->getCameraList()->GetItem(1)->getDeviceId(), videohelper->getCameraList()->GetItem(1)->getDeviceName());
	std::cout << "enableMultiStreamVideo: " << isSuccess  << std::endl;
	}
	if (sendVideo) {

		IZoomVideoSDKVideoHelper* videohelper = video_sdk_obj_->getVideoHelper();
		videohelper->selectCamera(videohelper->getCameraList()->GetItem(0)->getDeviceId());
		videohelper->startVideo();


	
	}

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
	//getting share or share screen is handled here
	//the method of getting raw share is similar to getting raw video, as it uses the same delegate ZoomVideoSDKRawDataPipeDelegate
	//getRawShare
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

	if (subscribeshare2ndCameraStream) {

		ZoomVideoSDKRawDataPipeDelegateShare* sharePipe = new ZoomVideoSDKRawDataPipeDelegateShare();
		pUser->GetSharePipe()->subscribe(ZoomVideoSDKResolution_720P, sharePipe);
		//ZoomVideoSDKRawDataPipeDelegateShare * sharePipe2 = new ZoomVideoSDKRawDataPipeDelegateShare();
		//pShareHelper->subscribeMyShareCamera(sharePipe2);

	}

}
void MainFrame::onUserRecordingConsent(IZoomVideoSDKUser* pUser) {}
void  MainFrame::onLiveStreamStatusChanged(IZoomVideoSDKLiveStreamHelper* pLiveStreamHelper, ZoomVideoSDKLiveStreamStatus status) {}
void MainFrame::onChatNewMessageNotify(IZoomVideoSDKChatHelper* pChatHelper, IZoomVideoSDKChatMessage* messageItem)
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
void MainFrame::onUserHostChanged(IZoomVideoSDKUserHelper* pUserHelper, IZoomVideoSDKUser* pUser) {}
void MainFrame::onUserActiveAudioChanged(IZoomVideoSDKAudioHelper* pAudioHelper, IVideoSDKVector<IZoomVideoSDKUser*>* list) {}
void MainFrame::onSessionNeedPassword(IZoomVideoSDKPasswordHandler* handler) {}
void MainFrame::onSessionPasswordWrong(IZoomVideoSDKPasswordHandler* handler) {}
void MainFrame::onMixedAudioRawDataReceived(AudioRawData* data_) {
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
void MainFrame::onOneWayAudioRawDataReceived(AudioRawData* data_, IZoomVideoSDKUser* pUser) {
	//this is the part where callback for raw audio data happens. this is the audio stream for each individual users
	//you can choose to save the data_ buffer as PCM file, or convert it to wav / mp3 or other format. 
	//do be mindful of the compute power and memory usage which this callback can utilize
	if (getRawAudio) {

	}
}
void MainFrame::onSharedAudioRawDataReceived(AudioRawData* data_) {
	//this is the part where callback for raw audio data happens. this is the audio stream from screensharing.
	//you can choose to save the data_ buffer as PCM file, or convert it to wav / mp3 or other format. 
	//do be mindful of the compute power and memory usage which this callback can utilize
	if (getRawAudio) {

	}
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
	//enableCloudRecording
	if (enableCloudRecording) {

	}
}
void MainFrame::onHostAskUnmute() {}
void MainFrame::onMultiCameraStreamStatusChanged(ZoomVideoSDKMultiCameraStreamStatus status, IZoomVideoSDKUser* pUser, IZoomVideoSDKRawDataPipe* pVideoPipe) {}
void MainFrame::onMicSpeakerVolumeChanged(unsigned int micVolume, unsigned int speakerVolume) {}
void MainFrame::onAudioDeviceStatusChanged(ZoomVideoSDKAudioDeviceType type, ZoomVideoSDKAudioDeviceStatus status) {}
void MainFrame::onTestMicStatusChanged(ZoomVideoSDK_TESTMIC_STATUS status) {
	//printf("onTestMicStatusChanged status Received is %s\n", status);


	if (status == 0) {

		//StopMicRecording();
		//PlayMicRecording();
	}

	if (status == 1) {

		//StopMicRecording();
		//PlayMicRecording();
	}
	if (status == 2) {
	
		StopMicRecording();
		PlayMicRecording();
	}


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
	if (previewCameraAndMicrophone) {

		const wchar_t* relativePath = L"example.mp3";
		// Use the GetCurrentDirectory function to get the current working directory of your executable.
		wchar_t buffer[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, buffer);

		// Combine the current directory with the relative path to get the full path.
		wchar_t fullPath[MAX_PATH];
		swprintf(fullPath, MAX_PATH, L"%s\\%s", buffer, relativePath);

		init_params.extendParam.speakerTestFilePath = fullPath;
	}


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


	//sendRawVideo
	if (sendRawVideo) {

		//the sdk uses a Video Source to send raw video
		//this needs to be done before joining session
		session_context.videoOption.localVideoOn = true;
		

		


		ZoomVideoSDKVideoSource* virtual_video_source = new ZoomVideoSDKVideoSource(WStringToString(DEFAULT_VIDEO_SOURCE));
		session_context.externalVideoSource = virtual_video_source;
	}
	//sendRawShare
	if (sendRawShare) {
		//nothing needed to be done before joining session
	}
	//sendRawAudio
	if (sendRawAudio) {

		//the sdk needs to connect to audio, we can also unmute beforehand
		session_context.audioOption.connect = true; //needed for sending raw audio data
		session_context.audioOption.mute = false; //needed for sending raw audio data

		//ZoomVideoSDKVirtualAudioMic is the object used to send audio
		ZoomVideoSDKVirtualAudioMic* vMic = new ZoomVideoSDKVirtualAudioMic();
		session_context.virtualAudioMic = vMic;
	}
	//languageTranscriptionAndTranslation
	if (languageTranscriptionAndTranslation) {
		//the sdk will need to connect to audio to do live transcription and translation for self.
		session_context.audioOption.connect = true;
		session_context.audioOption.mute = false;
	}
	//getRawAudio
	if (getRawAudio) {

		//Optional: Virtual Audio Speaker is needed if you are intended to get raw audio from virtual microphone
		ZoomVideoSDKVirtualAudioSpeaker* vSpeaker = new ZoomVideoSDKVirtualAudioSpeaker();
		session_context.virtualAudioSpeaker = vSpeaker;

		//the sdk needs to connect to audio
		session_context.audioOption.connect = true;
	}

	if (previewCameraAndMicrophone) {
		//if preview is on, don't join session
		StartPreview();
	}
	else {
	IZoomVideoSDKSession* pSession = video_sdk_obj_->joinSession(session_context);
	if (pSession)
	{
		pSession->getMyself()->GetVideoPipe();
	}
	}
}

void MainFrame::LeaveSession(bool end)
{
	ZoomVideoSDKErrors err = video_sdk_obj_->leaveSession(end);
}







