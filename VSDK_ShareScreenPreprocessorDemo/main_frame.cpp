#include <windows.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include <fstream>
#include <iosfwd>
#include <iostream>
#include <signal.h>
#include <stdlib.h>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp> // Only if you're using imshow or GUI tools
#include <tesseract/baseapi.h>          // Tesseract OCR API
#include <leptonica/allheaders.h>       // Required by Tesseract for image structures
#include <regex>


#include "main_frame.h";
#include "helpers/zoom_video_sdk_user_helper_interface.h"
#include "zoom_video_sdk_api.h"
#include "zoom_video_sdk_def.h"
#include "zoom_video_sdk_delegate_interface.h"
#include "zoom_video_sdk_interface.h"

#include "zoom_video_sdk_platform.h"

#include "zoom_video_sdk_delegate_interface.h"
#include "zoom_video_sdk_session_info_interface.h"
#include "helpers/zoom_video_sdk_audio_helper_interface.h"
#include "helpers/zoom_video_sdk_video_helper_interface.h"
#include "helpers/zoom_video_sdk_share_helper_interface.h"
#include "helpers/zoom_video_sdk_chat_helper_interface.h"
#include "helpers/zoom_video_sdk_cmd_channel_interface.h"
#include "zoom_video_sdk_interface.h"
#include "zoom_video_sdk_api.h"
#include "helpers/zoom_video_sdk_user_helper_interface.h"
#include "zoom_video_sdk_platform.h"
#include <vector>

//needed for audio
//#include "ZoomVideoSDKVirtualAudioMic.h"
//#include "ZoomVideoSDKVirtualAudioSpeaker.h"
#include "helpers/zoom_video_sdk_audio_send_rawdata_interface.h"

//needed for share screen
//#include "ZoomVideoSDKShareSource.h"
#include "helpers/zoom_video_sdk_share_helper_interface.h"

//needed for get raw video
//#include "ZoomVideoSDKRawDataPipeDelegate.h"
//#include "ZoomVideoSDKVideoSource.h"
#include "helpers/zoom_video_sdk_video_helper_interface.h"

//needed for command channel
#include "helpers/zoom_video_sdk_cmd_channel_interface.h"

//needed for chat
#include "helpers/zoom_video_sdk_chat_helper_interface.h"
#include "zoom_video_sdk_chat_message_interface.h"

//needed for LTT
#include "zoom_video_sdk_session_info_interface.h"
#include <string>

#include "json\json.h"
#include "WebService.h"
#include <atlstr.h>

//needed for screenshare
#include "helpers/zoom_video_sdk_share_helper_interface.h"

USING_ZOOM_VIDEO_SDK_NAMESPACE
using namespace Json;
using namespace std;

wstring sdk_jwt;
wstring sessionName;
wstring password;
wstring jwt_webservice_url;
IZoomVideoSDK* video_sdk_obj_;
IZoomVideoSDKShareSender* m_pShareSender;
constexpr auto CONFIG_FILE = "config.json";

bool isJWTWebService = false;



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
MainFrame::MainFrame() {

}

MainFrame::~MainFrame()
{
}

void MainFrame::StartToShare()
{
	std::cout << "StartToShare()" << std::endl;
	//  App calls startShareWithPreprocessing
	//	SDK captures the data of a screen or a windows, meanwhile, nothing is shared out
	//	SDK triggers the callback onCapturedRawDataReceived with a YUV data
	//	App masks the PII in the YUV data
	//	App call IZoomVideoSDKSharePreprocessSender.sendPreprocessedData with masked YUV data
	//	SDK shares the masked YUV data out
	//	The other ones in session see the masked YUV data.

	IZoomVideoSDKShareHelper* sdkShareHelper = video_sdk_obj_->getShareHelper();

	GetMonitorDeviceNames();



	ZoomVideoSDKSharePreprocessParam param;
	param.type = ZoomVideoSDKSharePreprocessType_screen;
	param.monitorID = L"\\\\.\\DISPLAY1"; // Example

	// Print the monitor ID
	std::wcout << L"Sharing monitor with ID: " << param.monitorID << std::endl;

	// Start sharing
	ZoomVideoSDKErrors err = sdkShareHelper->startShareWithPreprocessing(param, this);

	// Optional: check the result
	if (err != ZoomVideoSDKErrors_Success) {
		std::wcerr << L"Failed to start share. Error code: " << err << std::endl;
	}





	//ZoomVideoSDKErrors err = startSharingExternalSource(this); //It will trigger the callback onShareSendStarted

}

//save the sender object locally for later usage
void MainFrame::onShareSendStarted(IZoomVideoSDKShareSender* pSender)
{
	std::cout << "onShareSendStarted()" << std::endl;

}


void MainFrame::onShareSendStopped()
{
	m_pShareSender = nullptr;
}


std::vector<std::wstring> MainFrame::GetMonitorDeviceNames()
{
	std::vector<std::wstring> monitorNames;
	DISPLAY_DEVICE dd;
	dd.cb = sizeof(dd);
	int deviceIndex = 0;

	while (EnumDisplayDevices(nullptr, deviceIndex, &dd, 0))
	{
		std::wcout << L"Device Name: " << dd.DeviceName << std::endl;
		std::wcout << L"Device String: " << dd.DeviceString << std::endl;
		std::wcout << L"State Flags: " << dd.StateFlags << std::endl;
		std::wcout << std::endl;

		monitorNames.push_back(dd.DeviceName);

		deviceIndex++;
		ZeroMemory(&dd, sizeof(dd));  // Clear the structure for next use
		dd.cb = sizeof(dd);           // Reset size after clearing
	}

	return monitorNames;
}

//working sample
//void MainFrame::RemovePIIFromRawData(YUVRawDataI420* data_)
//{
//	std::cout << "RemovePIIFromRawData()" << std::endl;
//
//		//Share the data to others in the session. Use YBuffer as the first parameter for this SDK API
//	if (m_pShareSender)
//		m_pShareSender->sendPreprocessedData(data_);
//	    //m_pShareSender->sendPreprocessedData(nullptr);
//
//	data_->Release(); //IMPORTANT：call Release to make sure the SDK will delete the data after the data is sent out.
//}


//dreamtcs work in progress for OCR
void MainFrame::RemovePIIFromRawData(YUVRawDataI420* data_)
{
	std::cout << "[INFO] RemovePIIFromRawData() invoked." << std::endl;

	int width = data_->GetStreamWidth();
	int height = data_->GetStreamHeight();
	std::cout << "[INFO] Stream dimensions: " << width << "x" << height << std::endl;

	// Convert YUV420 to OpenCV Mat (grayscale)
	std::cout << "[INFO] Creating YUV420 Mat..." << std::endl;
	cv::Mat yuv420(height + height / 2, width, CV_8UC1, (void*)data_->GetYBuffer());

	cv::Mat bgr;
	std::cout << "[INFO] Converting YUV420 to BGR..." << std::endl;
	cv::cvtColor(yuv420, bgr, cv::COLOR_YUV2BGR_I420);

	// Resize for OCR (e.g., 1/2 size)
	cv::Mat resized;
	std::cout << "[INFO] Resizing image for OCR..." << std::endl;
	cv::resize(bgr, resized, cv::Size(), 0.5, 0.5);

	// OCR
	std::cout << "[INFO] Initializing Tesseract..." << std::endl;
	tesseract::TessBaseAPI tess;

	char* tessPrefix = nullptr;
	size_t len = 0;
	if (_dupenv_s(&tessPrefix, &len, "TESSDATA_PREFIX") == 0 && tessPrefix != nullptr) {
		std::cout << "[INFO] TESSDATA_PREFIX=" << tessPrefix << std::endl;
		free(tessPrefix);
	}
	else {
		std::cerr << "[WARN] TESSDATA_PREFIX not set." << std::endl;
	}

	if (tess.Init("C:/vcpkg/installed/x64-windows/share/tessdata", "eng", tesseract::OEM_LSTM_ONLY)) {
		std::cerr << "[ERROR] Tesseract failed to initialize." << std::endl;
		data_->Release();
		return;
	}
	std::cout << "[INFO] Tesseract initialized successfully." << std::endl;

	std::cout << "[INFO] Setting image to Tesseract..." << std::endl;
	tess.SetImage(resized.data, resized.cols, resized.rows, 3, resized.step);
	//tess.SetImage(bgr.data, bgr.cols, bgr.rows, 3, bgr.step);


	std::cout << "[INFO] Running OCR..." << std::endl;
	tess.Recognize(0);
	tesseract::ResultIterator* ri = tess.GetIterator();
	tesseract::PageIteratorLevel level = tesseract::RIL_WORD;

	//std::regex piiRegex(R"(\d{3}-\d{2}-\d{4})");
	std::regex piiRegex(R"(\bZane\b)", std::regex_constants::icase);

	int maskCount = 0;

	if (ri != nullptr) {
		do {
			const char* word = ri->GetUTF8Text(level);
			float conf = ri->Confidence(level);
			int x1, y1, x2, y2;
			ri->BoundingBox(level, &x1, &y1, &x2, &y2);

			if (word) {
				//std::cout << "[OCR] Word: '" << word << "' at (" << x1 << "," << y1 << ") - (" << x2 << "," << y2 << ") confidence: " << conf << std::endl;

				if (std::regex_match(word, piiRegex)) {
					std::cout << "[MATCH] Masking PII word: " << word << std::endl;
					maskCount++;

					int orig_x1 = static_cast<int>(x1 * 2);
					int orig_y1 = static_cast<int>(y1 * 2);
					int orig_x2 = static_cast<int>(x2 * 2);
					int orig_y2 = static_cast<int>(y2 * 2);

					char* yBuffer = data_->GetYBuffer();
					for (int y = orig_y1; y < orig_y2 && y < height; ++y) {
						for (int x = orig_x1; x < orig_x2 && x < width; ++x) {
							int index = y * width + x;
							yBuffer[index] = 128;
						}
					}
				}
				delete[] word;
			}
		} while (ri->Next(level));
	}
	else {
		std::cout << "[INFO] No text detected." << std::endl;
	}

	std::cout << "[INFO] Total PII items masked: " << maskCount << std::endl;

	if (m_pShareSender) {
		std::cout << "[INFO] Sending preprocessed data..." << std::endl;
		m_pShareSender->sendPreprocessedData(data_);
	}
	else {
		std::cerr << "[WARN] m_pShareSender is null." << std::endl;
	}

	data_->Release();
	std::cout << "[INFO] Raw data released." << std::endl;
}

void MainFrame::onCapturedRawDataReceived(YUVRawDataI420* pRawData, IZoomVideoSDKSharePreprocessSender* pSender)
{
	std::cout << "onCapturedRawDataReceived()" << std::endl;

	m_pShareSender = pSender;

	if (!pRawData) return;
	pRawData->AddRef(); //IMPORTANT：call AddRef to make sure the SDK will not delete the data immediately.
	RemovePIIFromRawData(pRawData);

}

//dreamtcs might be able to remove this
void MainFrame::onShareStopped()
{
	m_pShareSender = nullptr;

}




// this callback happens when sdk / use has successfully joined the session
void MainFrame::onSessionJoin()
{
	std::cout << "onSessionJoin()" << std::endl;


	if (video_sdk_obj_)
	{
		StartToShare();
		////dreamtcs
		//// 
		//IZoomVideoSDKVideoHelper* videohelper = video_sdk_obj_->getVideoHelper();
		//IZoomVideoSDKShareHelper* sharehelper = video_sdk_obj_->getShareHelper();
		////get all camera devices
		//IVideoSDKVector<IZoomVideoSDKCameraDevice*>* pCameraList = videohelper->getCameraList();
		//IZoomVideoSDKCameraDevice* usbCaptureDevice{};

		////loop all camera devices
		//for (int i = 0; i < pCameraList->GetCount(); i++)
		//{
		//	IZoomVideoSDKCameraDevice* pCamera = pCameraList->GetItem(i);

		//	CString strItem;

		//	//check if there is persisted settings (multi-camera send) from last run
		//	//if there is, remove the setting
		//	if (pCamera->isSelectedAsMultiCamera()) {
		//		strItem.Format(_T("%s***%s"), pCamera->getDeviceName(), "true");
		//		wprintf(_T("%s\n"), (LPCTSTR)strItem);
		//		//disable it
		//		bool disableSuccess = videohelper->disableMultiStreamVideo(pCamera->getDeviceId());
		//		if (disableSuccess) { wprintf(_T("%s\n"), "disable success"); }
		//		else { wprintf(_T("%s\n"), "disable failed"); }
		//	}
		//	else {
		//		strItem.Format(_T("%s***%s"), pCamera->getDeviceName(), "false");
		//		wprintf(_T("%s\n"), (LPCTSTR)strItem);
		//	}

		//	//search for USB devicewith the word Link or USB in it's name, and assign it to a temp variable
		//	CString deviceName(pCamera->getDeviceName());
		//	if (deviceName.Find(_T("Link")) != -1 || deviceName.Find(_T("USB")) != -1) {
		//		wprintf(_T("Camera ID for device with 'Link' in the name: %s\n"), (LPCTSTR)pCamera->getDeviceId());
		//		usbCaptureDevice = pCamera;
		//	}

		//}

		//if (usbCaptureDevice) {
		//	ZoomVideoSDKErrors err = ->startShare2ndCamera(usbCaptureDevice->getDeviceId());
		//	sharehelper->enableOptimizeForSharedVideo(true);
		//}


	}

}
void MainFrame::onSessionLeave() {}
void MainFrame::onSessionLeave(ZoomVideoSDKSessionLeaveReason eReason) {}
void MainFrame::onError(ZoomVideoSDKErrors errorCode, int detailErrorCode)
{
	std::cout << "onError(), errorCode " << errorCode << " detailedErrorCode: " << detailErrorCode << std::endl;
}
void MainFrame::onUserJoin(IZoomVideoSDKUserHelper* pUserHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList) {}
void MainFrame::onUserLeave(IZoomVideoSDKUserHelper* pUserHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList) {}
void MainFrame::onUserVideoStatusChanged(IZoomVideoSDKVideoHelper* pVideoHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList) {}
void MainFrame::onUserAudioStatusChanged(IZoomVideoSDKAudioHelper* pAudioHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList) {}
void MainFrame::onUserShareStatusChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, IZoomVideoSDKShareAction* pShareAction) {}
void MainFrame::onShareContentChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, IZoomVideoSDKShareAction* pShareAction) {}
void MainFrame::onFailedToStartShare(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser) {}
void MainFrame::onCalloutJoinSuccess(IZoomVideoSDKUser* pUser, const zchar_t* phoneNumber) {}
void MainFrame::onUserRecordingConsent(IZoomVideoSDKUser* pUser) {}
void MainFrame::onLiveStreamStatusChanged(IZoomVideoSDKLiveStreamHelper* pLiveStreamHelper, ZoomVideoSDKLiveStreamStatus status) {}

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
void MainFrame::onMixedAudioRawDataReceived(AudioRawData* data_) {}
void MainFrame::onOneWayAudioRawDataReceived(AudioRawData* data_, IZoomVideoSDKUser* pUser) {}
void MainFrame::onSharedAudioRawDataReceived(AudioRawData* data_) {}
void MainFrame::onUserManagerChanged(IZoomVideoSDKUser* pUser) {}
void MainFrame::onUserNameChanged(IZoomVideoSDKUser* pUser) {}
void MainFrame::onCameraControlRequestResult(IZoomVideoSDKUser* pUser, bool isApproved) {}
void MainFrame::onCameraControlRequestReceived(IZoomVideoSDKUser* pUser, ZoomVideoSDKCameraControlRequestType requestType, IZoomVideoSDKCameraControlRequestHandler* pCameraControlRequestHandler) {}
void MainFrame::onRemoteControlStatus(IZoomVideoSDKUser* pUser, IZoomVideoSDKShareAction* pShareAction, ZoomVideoSDKRemoteControlStatus status) {}
void MainFrame::onRemoteControlRequestReceived(IZoomVideoSDKUser* pUser, IZoomVideoSDKShareAction* pShareAction, IZoomVideoSDKRemoteControlRequestHandler* pRemoteControlRequestHandler) {}
void MainFrame::onRemoteControlServiceInstallResult(bool bSuccess) {}
void MainFrame::onCommandReceived(IZoomVideoSDKUser* pSender, const zchar_t* strCmd) {}
void MainFrame::onCommandChannelConnectResult(bool isSuccess) {}
void MainFrame::onInviteByPhoneStatus(PhoneStatus status, PhoneFailedReason reason) {}
void MainFrame::onCloudRecordingStatus(RecordingStatus status, IZoomVideoSDKRecordingConsentHandler* pHandler) {}
void MainFrame::onHostAskUnmute() {}
void MainFrame::onMultiCameraStreamStatusChanged(ZoomVideoSDKMultiCameraStreamStatus status, IZoomVideoSDKUser* pUser, IZoomVideoSDKRawDataPipe* pVideoPipe) {}
void MainFrame::onMicSpeakerVolumeChanged(unsigned int micVolume, unsigned int speakerVolume) {}
void MainFrame::onAudioDeviceStatusChanged(ZoomVideoSDKAudioDeviceType type, ZoomVideoSDKAudioDeviceStatus status) {}
void MainFrame::onTestMicStatusChanged(ZoomVideoSDK_TESTMIC_STATUS status) {}
void MainFrame::onSelectedAudioDeviceChanged() {}
void MainFrame::onCameraListChanged() {}
void MainFrame::onLiveTranscriptionStatus(ZoomVideoSDKLiveTranscriptionStatus status) {}
void MainFrame::onOriginalLanguageMsgReceived(ILiveTranscriptionMessageInfo* messageInfo) {}
void MainFrame::onLiveTranscriptionMsgInfoReceived(ILiveTranscriptionMessageInfo* messageInfo) {}
void MainFrame::onLiveTranscriptionMsgError(ILiveTranscriptionLanguage* spokenLanguage, ILiveTranscriptionLanguage* transcriptLanguage) {}
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
void MainFrame::onShareCanvasSubscribeFail(IZoomVideoSDKUser* pUser, void* handle, IZoomVideoSDKShareAction* pShareAction) {}
void MainFrame::onAnnotationHelperCleanUp(IZoomVideoSDKAnnotationHelper* helper) {}
void MainFrame::onAnnotationPrivilegeChange(IZoomVideoSDKUser* pUser, IZoomVideoSDKShareAction* pShareAction) {}
void MainFrame::onAnnotationHelperActived(void* handle) {}
void MainFrame::onVideoAlphaChannelStatusChanged(bool isAlphaModeOn) {}
void MainFrame::onSpotlightVideoChanged(IZoomVideoSDKVideoHelper* pVideoHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList) {}
void MainFrame::onBindIncomingLiveStreamResponse(bool bSuccess, const zchar_t* strStreamKeyID) {}
void MainFrame::onUnbindIncomingLiveStreamResponse(bool bSuccess, const zchar_t* strStreamKeyID) {}
void MainFrame::onIncomingLiveStreamStatusResponse(bool bSuccess, IVideoSDKVector<IncomingLiveStreamStatus>* pStreamsStatusList) {}
void MainFrame::onStartIncomingLiveStreamResponse(bool bSuccess, const zchar_t* strStreamKeyID) {}
void MainFrame::onStopIncomingLiveStreamResponse(bool bSuccess, const zchar_t* strStreamKeyID) {}
void MainFrame::onShareContentSizeChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, IZoomVideoSDKShareAction* pShareAction) {}


//IZoomVideoSDKRawDataPipeDelegate

void MainFrame::onRawDataFrameReceived(YUVRawDataI420* data_)
{
}

void MainFrame::onRawDataStatusChanged(RawDataStatus status)
{
}

void MainFrame::onShareCursorDataReceived(ZoomVideoSDKShareCursorData info)
{
}






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
	std::wstring sUserName = L"vsdk_skeletondemo";
	std::wstring session_password_ = password;
	std::wstring token = sdk_jwt;

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







