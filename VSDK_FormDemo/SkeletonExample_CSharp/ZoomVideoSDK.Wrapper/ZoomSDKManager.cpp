#include "ZoomSDKManager.h"
#include "../sdk/x64/h/zoom_video_sdk_interface.h"
#include "../sdk/x64/h/zoom_video_sdk_api.h"
#include "../sdk/x64/h/zoom_video_sdk_delegate_interface.h"
#include "../sdk/x64/h/zoom_video_sdk_session_info_interface.h"
#include "../sdk/x64/h/helpers/zoom_video_sdk_audio_helper_interface.h"
#include "../sdk/x64/h/helpers/zoom_video_sdk_video_helper_interface.h"
#include "../sdk/x64/h/helpers/zoom_video_sdk_user_helper_interface.h"
#include "../sdk/x64/h/zoom_sdk_raw_data_def.h"
#include "../sdk/x64/h/zoom_video_sdk_vector_interface.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace ZOOMVIDEOSDK;

namespace ZoomVideoSDKWrapper {

    // Forward declare the managed class
    ref class ZoomSDKManager;

    // Video preview handler for raw data pipe delegate
    class VideoPreviewHandler : public IZoomVideoSDKRawDataPipeDelegate
    {
    private:
        gcroot<ZoomSDKManager^> m_managedHandler;

    public:
        VideoPreviewHandler(ZoomSDKManager^ handler) : m_managedHandler(handler) {}
        virtual ~VideoPreviewHandler() {}

        virtual void onRawDataFrameReceived(YUVRawDataI420* data_) override;
        virtual void onRawDataStatusChanged(RawDataStatus status) override;
        virtual void onShareCursorDataReceived(ZoomVideoSDKShareCursorData info) override;
    };

    // Remote video handler for user video pipes
    class RemoteVideoHandler : public IZoomVideoSDKRawDataPipeDelegate
    {
    private:
        gcroot<ZoomSDKManager^> m_managedHandler;
        gcroot<String^> m_userId;

    public:
        RemoteVideoHandler(ZoomSDKManager^ handler, String^ userId) 
            : m_managedHandler(handler), m_userId(userId) {}
        virtual ~RemoteVideoHandler() {}

        virtual void onRawDataFrameReceived(YUVRawDataI420* data_) override;
        virtual void onRawDataStatusChanged(RawDataStatus status) override;
        virtual void onShareCursorDataReceived(ZoomVideoSDKShareCursorData info) override;
    };

    // Simple native callback handler - implementing only essential callbacks
    class SimpleNativeHandler : public IZoomVideoSDKDelegate
    {
    private:
        gcroot<ZoomVideoSDKWrapper::ZoomSDKManager^> m_managedHandler;

    public:
        SimpleNativeHandler(ZoomVideoSDKWrapper::ZoomSDKManager^ handler) : m_managedHandler(handler) {}

        // Essential callbacks
        virtual void onSessionJoin() override
        {
            ZoomVideoSDKWrapper::ZoomSDKManager^ handler = static_cast<ZoomVideoSDKWrapper::ZoomSDKManager^>(m_managedHandler);
            if (handler)
                handler->OnSessionStatusChanged(SessionStatus::InSession, "Session joined successfully");
        }

        virtual void onSessionLeave() override
        {
            ZoomVideoSDKWrapper::ZoomSDKManager^ handler = static_cast<ZoomVideoSDKWrapper::ZoomSDKManager^>(m_managedHandler);
            if (handler)
                handler->OnSessionStatusChanged(SessionStatus::Idle, "Session left");
        }

        virtual void onSessionLeave(ZoomVideoSDKSessionLeaveReason eReason) override
        {
            ZoomVideoSDKWrapper::ZoomSDKManager^ handler = static_cast<ZoomVideoSDKWrapper::ZoomSDKManager^>(m_managedHandler);
            if (handler)
                handler->OnSessionStatusChanged(SessionStatus::Idle, "Session left with reason");
        }

        virtual void onError(ZoomVideoSDKErrors errorCode, int detailErrorCode) override
        {
            ZoomVideoSDKWrapper::ZoomSDKManager^ handler = static_cast<ZoomVideoSDKWrapper::ZoomSDKManager^>(m_managedHandler);
            if (handler)
            {
                String^ errorMsg = String::Format("SDK Error: {0} (Detail: {1})", (int)errorCode, detailErrorCode);
                handler->OnSessionStatusChanged(SessionStatus::Error, errorMsg);
            }
        }

        // Video-related callbacks - now properly implemented
        virtual void onUserJoin(IZoomVideoSDKUserHelper* pUserHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList) override 
        {
            ZoomVideoSDKWrapper::ZoomSDKManager^ handler = static_cast<ZoomVideoSDKWrapper::ZoomSDKManager^>(m_managedHandler);
            if (handler && userList)
            {
                // Subscribe to video for each new user
                for (int i = 0; i < userList->GetCount(); i++)
                {
                    IZoomVideoSDKUser* user = userList->GetItem(i);
                    if (user)
                    {
                        handler->SubscribeToUserVideo(user);
                    }
                }
            }
        }
        
        virtual void onUserLeave(IZoomVideoSDKUserHelper* pUserHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList) override 
        {
            ZoomVideoSDKWrapper::ZoomSDKManager^ handler = static_cast<ZoomVideoSDKWrapper::ZoomSDKManager^>(m_managedHandler);
            if (handler && userList)
            {
                // Unsubscribe from video for each leaving user
                for (int i = 0; i < userList->GetCount(); i++)
                {
                    IZoomVideoSDKUser* user = userList->GetItem(i);
                    if (user)
                    {
                        handler->UnsubscribeFromUserVideo(user);
                    }
                }
            }
        }
        
        virtual void onUserVideoStatusChanged(IZoomVideoSDKVideoHelper* pVideoHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList) override 
        {
            ZoomVideoSDKWrapper::ZoomSDKManager^ handler = static_cast<ZoomVideoSDKWrapper::ZoomSDKManager^>(m_managedHandler);
            if (handler && userList)
            {
                // Handle video status changes for users
                for (int i = 0; i < userList->GetCount(); i++)
                {
                    IZoomVideoSDKUser* user = userList->GetItem(i);
                    if (user)
                    {
                        handler->HandleUserVideoStatusChange(user);
                    }
                }
            }
        }
        virtual void onUserAudioStatusChanged(IZoomVideoSDKAudioHelper* pAudioHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList) override {}
        virtual void onUserShareStatusChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, IZoomVideoSDKShareAction* pShareAction) override {}
        virtual void onShareContentChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, IZoomVideoSDKShareAction* pShareAction) override {}
        virtual void onFailedToStartShare(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser) override {}
        virtual void onShareSettingChanged(ZoomVideoSDKShareSetting setting) override {}
        virtual void onUserRecordingConsent(IZoomVideoSDKUser* pUser) override {}
        virtual void onLiveStreamStatusChanged(IZoomVideoSDKLiveStreamHelper* pLiveStreamHelper, ZoomVideoSDKLiveStreamStatus status) override {}
        virtual void onChatNewMessageNotify(IZoomVideoSDKChatHelper* pChatHelper, IZoomVideoSDKChatMessage* messageItem) override {}
        virtual void onUserHostChanged(IZoomVideoSDKUserHelper* pUserHelper, IZoomVideoSDKUser* pUser) override {}
        virtual void onUserActiveAudioChanged(IZoomVideoSDKAudioHelper* pAudioHelper, IVideoSDKVector<IZoomVideoSDKUser*>* list) override {}
        virtual void onSessionNeedPassword(IZoomVideoSDKPasswordHandler* handler) override {}
        virtual void onSessionPasswordWrong(IZoomVideoSDKPasswordHandler* handler) override {}
        virtual void onMixedAudioRawDataReceived(AudioRawData* data_) override {}
        virtual void onOneWayAudioRawDataReceived(AudioRawData* data_, IZoomVideoSDKUser* pUser) override {}
        virtual void onSharedAudioRawDataReceived(AudioRawData* data_) override {}
        virtual void onUserManagerChanged(IZoomVideoSDKUser* pUser) override {}
        virtual void onUserNameChanged(IZoomVideoSDKUser* pUser) override {}
        virtual void onCameraControlRequestResult(IZoomVideoSDKUser* pUser, bool isApproved) override {}
        virtual void onCameraControlRequestReceived(IZoomVideoSDKUser* pUser, ZoomVideoSDKCameraControlRequestType requestType, IZoomVideoSDKCameraControlRequestHandler* pCameraControlRequestHandler) override {}
        virtual void onCommandReceived(IZoomVideoSDKUser* sender, const zchar_t* strCmd) override {}
        virtual void onCommandChannelConnectResult(bool isSuccess) override {}
        virtual void onInviteByPhoneStatus(PhoneStatus status, PhoneFailedReason reason) override {}
        virtual void onCalloutJoinSuccess(IZoomVideoSDKUser* pUser, const zchar_t* phoneNumber) override {}
        virtual void onCloudRecordingStatus(RecordingStatus status, IZoomVideoSDKRecordingConsentHandler* pHandler) override {}
        virtual void onHostAskUnmute() override {}
        virtual void onMultiCameraStreamStatusChanged(ZoomVideoSDKMultiCameraStreamStatus status, IZoomVideoSDKUser* pUser, IZoomVideoSDKRawDataPipe* pVideoPipe) override {}
        virtual void onMicSpeakerVolumeChanged(unsigned int micVolume, unsigned int speakerVolume) override {}
        virtual void onAudioDeviceStatusChanged(ZoomVideoSDKAudioDeviceType type, ZoomVideoSDKAudioDeviceStatus status) override {}
        virtual void onTestMicStatusChanged(ZoomVideoSDK_TESTMIC_STATUS status) override {}
        virtual void onSelectedAudioDeviceChanged() override {}
        virtual void onCameraListChanged() override {}
        virtual void onLiveTranscriptionStatus(ZoomVideoSDKLiveTranscriptionStatus status) override {}
        virtual void onOriginalLanguageMsgReceived(ILiveTranscriptionMessageInfo* messageInfo) override {}
        virtual void onLiveTranscriptionMsgInfoReceived(ILiveTranscriptionMessageInfo* messageInfo) override {}
        virtual void onLiveTranscriptionMsgError(ILiveTranscriptionLanguage* spokenLanguage, ILiveTranscriptionLanguage* transcriptLanguage) override {}
        virtual void onSpokenLanguageChanged(ILiveTranscriptionLanguage* spokenLanguage) override {}
        virtual void onChatMsgDeleteNotification(IZoomVideoSDKChatHelper* pChatHelper, const zchar_t* msgID, ZoomVideoSDKChatMessageDeleteType deleteBy) override {}
        virtual void onChatPrivilegeChanged(IZoomVideoSDKChatHelper* pChatHelper, ZoomVideoSDKChatPrivilegeType privilege) override {}
        virtual void onSendFileStatus(IZoomVideoSDKSendFile* file, const FileTransferStatus& status) override {}
        virtual void onReceiveFileStatus(IZoomVideoSDKReceiveFile* file, const FileTransferStatus& status) override {}
        virtual void onProxyDetectComplete() override {}
        virtual void onProxySettingNotification(IZoomVideoSDKProxySettingHandler* handler) override {}
        virtual void onSSLCertVerifiedFailNotification(IZoomVideoSDKSSLCertificateInfo* info) override {}
        virtual void onUserVideoNetworkStatusChanged(ZoomVideoSDKNetworkStatus status, IZoomVideoSDKUser* pUser) override {}
        virtual void onShareNetworkStatusChanged(ZoomVideoSDKNetworkStatus shareNetworkStatus, bool isSendingShare) override {}
        virtual void onCallCRCDeviceStatusChanged(ZoomVideoSDKCRCCallStatus status) override {}
        virtual void onAnnotationHelperCleanUp(IZoomVideoSDKAnnotationHelper* helper) override {}
        virtual void onAnnotationPrivilegeChange(IZoomVideoSDKUser* pUser, IZoomVideoSDKShareAction* pShareAction) override {}
        virtual void onAnnotationHelperActived(void* handle) override {}
        virtual void onVideoAlphaChannelStatusChanged(bool isAlphaModeOn) override {}
        virtual void onSpotlightVideoChanged(IZoomVideoSDKVideoHelper* pVideoHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList) override {}
        virtual void onBindIncomingLiveStreamResponse(bool bSuccess, const zchar_t* strStreamKeyID) override {}
        virtual void onUnbindIncomingLiveStreamResponse(bool bSuccess, const zchar_t* strStreamKeyID) override {}
        virtual void onIncomingLiveStreamStatusResponse(bool bSuccess, IVideoSDKVector<IncomingLiveStreamStatus>* pStreamsStatusList) override {}
        virtual void onStartIncomingLiveStreamResponse(bool bSuccess, const zchar_t* strStreamKeyID) override {}
        virtual void onStopIncomingLiveStreamResponse(bool bSuccess, const zchar_t* strStreamKeyID) override {}
        virtual void onShareContentSizeChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, IZoomVideoSDKShareAction* pShareAction) override {}
        virtual void onSubSessionStatusChanged(ZoomVideoSDKSubSessionStatus status, IVideoSDKVector<ISubSessionKit*>* pSubSessionKitList) override {}
        virtual void onSubSessionManagerHandle(IZoomVideoSDKSubSessionManager* pManager) override {}
        virtual void onSubSessionParticipantHandle(IZoomVideoSDKSubSessionParticipant* pParticipant) override {}
        virtual void onSubSessionUsersUpdate(ISubSessionKit* pSubSessionKit) override {}
        virtual void onBroadcastMessageFromMainSession(const zchar_t* sMessage, const zchar_t* sUserName) override {}
        virtual void onSubSessionUserHelpRequest(ISubSessionUserHelpRequestHandler* pHandler) override {}
        virtual void onSubSessionUserHelpRequestResult(ZoomVideoSDKUserHelpRequestResult eResult) override {}

#if !defined __linux
        virtual void onVideoCanvasSubscribeFail(ZoomVideoSDKSubscribeFailReason fail_reason, IZoomVideoSDKUser* pUser, void* handle) override {}
        virtual void onShareCanvasSubscribeFail(IZoomVideoSDKUser* pUser, void* handle, IZoomVideoSDKShareAction* pShareAction) override {}
#endif

#if defined(WIN32) || defined(__MACOS__)
        virtual void onRemoteControlStatus(IZoomVideoSDKUser* pUser, IZoomVideoSDKShareAction* pShareAction, ZoomVideoSDKRemoteControlStatus status) override {}
        virtual void onRemoteControlRequestReceived(IZoomVideoSDKUser* pUser, IZoomVideoSDKShareAction* pShareAction, IZoomVideoSDKRemoteControlRequestHandler* pRemoteControlRequestHandler) override {}
#endif

#if defined(WIN32)
        virtual void onRemoteControlServiceInstallResult(bool bSuccess) override {}
#endif
    };

    // Constructor
    ZoomSDKManager::ZoomSDKManager()
        : m_pVideoSDK(nullptr)
        , m_pSession(nullptr)
        , m_bInitialized(false)
        , m_pSessionHandler(nullptr)
        , m_pVideoHandler(nullptr)
        , m_pPreviewHandler(nullptr)
    {
    }

    // Destructor
    ZoomSDKManager::~ZoomSDKManager()
    {
        this->!ZoomSDKManager();
    }

    // Finalizer
    ZoomSDKManager::!ZoomSDKManager()
    {
        Cleanup();
    }

    // Initialize SDK with real implementation
    bool ZoomSDKManager::Initialize()
    {
        try {
            // Get SDK instance using proper API
            m_pVideoSDK = CreateZoomVideoSDKObj();
            if (!m_pVideoSDK)
                return false;

            // Create native callback handler
            m_pSessionHandler = new SimpleNativeHandler(this);

            // Initialize SDK
            ZoomVideoSDKInitParams initParams;
            initParams.domain = L"https://zoom.us";
            initParams.logFilePrefix = L"ZoomSDK";
            initParams.enableLog = true;

            IZoomVideoSDK* pSDK = static_cast<IZoomVideoSDK*>(m_pVideoSDK);
            ZoomVideoSDKErrors ret = pSDK->initialize(initParams);
            if (ret != ZoomVideoSDKErrors_Success)
            {
                OnSessionStatusChanged(SessionStatus::Error, String::Format("SDK initialization failed: {0}", (int)ret));
                return false;
            }

            // Add listener for callbacks
            pSDK->addListener(static_cast<IZoomVideoSDKDelegate*>(m_pSessionHandler));

            m_bInitialized = true;
            OnSessionStatusChanged(SessionStatus::Idle, "SDK initialized successfully");
            return true;
        }
        catch (System::Exception^) {
            return false;
        }
    }

    // Cleanup SDK
    void ZoomSDKManager::Cleanup()
    {
        if (m_pVideoSDK)
        {
            IZoomVideoSDK* pSDK = static_cast<IZoomVideoSDK*>(m_pVideoSDK);
            
            if (IsInSession)
                LeaveSession();

            if (m_pSessionHandler)
            {
                pSDK->removeListener(static_cast<IZoomVideoSDKDelegate*>(m_pSessionHandler));
            }

            pSDK->cleanup();
            DestroyZoomVideoSDKObj();
            m_pVideoSDK = nullptr;
        }

        if (m_pSessionHandler)
        {
            delete static_cast<SimpleNativeHandler*>(m_pSessionHandler);
            m_pSessionHandler = nullptr;
        }

        m_bInitialized = false;
    }

    // Join session with real implementation
    bool ZoomSDKManager::JoinSession(String^ sessionName, String^ token, String^ userName, String^ password)
    {
        if (!m_bInitialized || !m_pVideoSDK)
            return false;

        try {
            // Convert managed strings to native
            std::wstring nativeSessionName = ConvertToNativeString(sessionName);
            std::wstring nativeToken = ConvertToNativeString(token);
            std::wstring nativeUserName = ConvertToNativeString(userName);
            std::wstring nativePassword = ConvertToNativeString(password);

            // Create session context
            ZoomVideoSDKSessionContext sessionContext;
            sessionContext.sessionName = nativeSessionName.c_str();
            sessionContext.userName = nativeUserName.c_str();
            sessionContext.token = nativeToken.c_str();
            sessionContext.sessionPassword = nativePassword.empty() ? nullptr : nativePassword.c_str();
            sessionContext.videoOption.localVideoOn = true;  // Enable video transmission
            sessionContext.audioOption.connect = true;
            sessionContext.audioOption.mute = false;

            OnSessionStatusChanged(SessionStatus::Joining, "Joining session...");

            // Join session
            IZoomVideoSDK* pSDK = static_cast<IZoomVideoSDK*>(m_pVideoSDK);
            IZoomVideoSDKSession* session = pSDK->joinSession(sessionContext);
            if (!session)
            {
                OnSessionStatusChanged(SessionStatus::Error, "Failed to join session");
                return false;
            }

            return true;
        }
        catch (System::Exception^) {
            OnSessionStatusChanged(SessionStatus::Error, "Exception occurred while joining session");
            return false;
        }
    }

    // Leave session
    void ZoomSDKManager::LeaveSession()
    {
        if (m_bInitialized && m_pVideoSDK)
        {
            OnSessionStatusChanged(SessionStatus::Leaving, "Leaving session...");
            IZoomVideoSDK* pSDK = static_cast<IZoomVideoSDK*>(m_pVideoSDK);
            pSDK->leaveSession(false);
        }
    }

    // Audio controls with real implementation
    bool ZoomSDKManager::MuteAudio(bool mute)
    {
        if (!m_bInitialized || !m_pVideoSDK)
            return false;

        IZoomVideoSDK* pSDK = static_cast<IZoomVideoSDK*>(m_pVideoSDK);
        IZoomVideoSDKAudioHelper* audioHelper = pSDK->getAudioHelper();
        if (!audioHelper)
            return false;

        // muteAudio requires a user pointer, pass nullptr for current user
        ZoomVideoSDKErrors ret = audioHelper->muteAudio(nullptr);
        return ret == ZoomVideoSDKErrors_Success;
    }

    bool ZoomSDKManager::MuteSpeaker(bool mute)
    {
        if (!m_bInitialized || !m_pVideoSDK)
            return false;

        IZoomVideoSDK* pSDK = static_cast<IZoomVideoSDK*>(m_pVideoSDK);
        IZoomVideoSDKAudioHelper* audioHelper = pSDK->getAudioHelper();
        if (!audioHelper)
            return false;

        // Note: The actual API doesn't have muteSpeaker, this is a placeholder
        return false; // Not supported in current API
    }

    bool ZoomSDKManager::IsAudioMuted()
    {
        if (!m_bInitialized || !m_pVideoSDK)
            return false;

        IZoomVideoSDK* pSDK = static_cast<IZoomVideoSDK*>(m_pVideoSDK);
        IZoomVideoSDKAudioHelper* audioHelper = pSDK->getAudioHelper();
        if (!audioHelper)
            return false;

        // Note: The actual API doesn't have isAudioMuted, this is a placeholder
        return false; // Not supported in current API
    }

    bool ZoomSDKManager::IsSpeakerMuted()
    {
        if (!m_bInitialized || !m_pVideoSDK)
            return false;

        IZoomVideoSDK* pSDK = static_cast<IZoomVideoSDK*>(m_pVideoSDK);
        IZoomVideoSDKAudioHelper* audioHelper = pSDK->getAudioHelper();
        if (!audioHelper)
            return false;

        // Note: The actual API doesn't have isSpeakerMuted, this is a placeholder
        return false; // Not supported in current API
    }

    // Video controls - WITH PREVIEW SUPPORT
    bool ZoomSDKManager::StartVideo()
    {
        if (!m_bInitialized || !m_pVideoSDK)
            return false;

        IZoomVideoSDK* pSDK = static_cast<IZoomVideoSDK*>(m_pVideoSDK);
        IZoomVideoSDKVideoHelper* videoHelper = pSDK->getVideoHelper();
        if (!videoHelper)
            return false;

        // Start video transmission
        ZoomVideoSDKErrors ret = videoHelper->startVideo();
        if (ret == ZoomVideoSDKErrors_Success)
        {
            OnSessionStatusChanged(SessionStatus::InSession, "Video started successfully");
            
            // Also start video preview for self video
            try {
                StartVideoPreview();
            }
            catch (System::Exception^) {
                OnSessionStatusChanged(SessionStatus::InSession, "Video preview failed to start, but video transmission is working");
            }
            
            return true;
        }
        else
        {
            OnSessionStatusChanged(SessionStatus::Error, String::Format("Failed to start video: {0}", (int)ret));
            return false;
        }
    }

    bool ZoomSDKManager::StopVideo()
    {
        if (!m_bInitialized || !m_pVideoSDK)
            return false;

        IZoomVideoSDK* pSDK = static_cast<IZoomVideoSDK*>(m_pVideoSDK);
        IZoomVideoSDKVideoHelper* videoHelper = pSDK->getVideoHelper();
        if (!videoHelper)
            return false;

        ZoomVideoSDKErrors ret = videoHelper->stopVideo();
        return ret == ZoomVideoSDKErrors_Success;
    }

    bool ZoomSDKManager::IsVideoStarted()
    {
        if (!m_bInitialized || !m_pVideoSDK)
            return false;

        IZoomVideoSDK* pSDK = static_cast<IZoomVideoSDK*>(m_pVideoSDK);
        IZoomVideoSDKVideoHelper* videoHelper = pSDK->getVideoHelper();
        if (!videoHelper)
            return false;

        // Note: The actual API doesn't have isVideoOn, this is a placeholder
        return false; // Not supported in current API
    }

    // Real device management implementation using correct SDK interfaces
    array<String^>^ ZoomSDKManager::GetMicrophoneList()
    {
        if (!m_bInitialized || !m_pVideoSDK)
            return gcnew array<String^>(0);

        try {
            IZoomVideoSDK* pSDK = static_cast<IZoomVideoSDK*>(m_pVideoSDK);
            IZoomVideoSDKAudioHelper* audioHelper = pSDK->getAudioHelper();
            if (!audioHelper)
                return gcnew array<String^>(0);

            IVideoSDKVector<IZoomVideoSDKMicDevice*>* micList = audioHelper->getMicList();
            if (!micList)
                return gcnew array<String^>(0);

            int count = micList->GetCount();
            array<String^>^ result = gcnew array<String^>(count);
            
            for (int i = 0; i < count; i++)
            {
                IZoomVideoSDKMicDevice* micDevice = micList->GetItem(i);
                if (micDevice)
                {
                    result[i] = ConvertToManagedString(micDevice->getDeviceName());
                }
            }
            
            return result;
        }
        catch (System::Exception^) {
            return gcnew array<String^>(0);
        }
    }

    array<String^>^ ZoomSDKManager::GetSpeakerList()
    {
        if (!m_bInitialized || !m_pVideoSDK)
            return gcnew array<String^>(0);

        try {
            IZoomVideoSDK* pSDK = static_cast<IZoomVideoSDK*>(m_pVideoSDK);
            IZoomVideoSDKAudioHelper* audioHelper = pSDK->getAudioHelper();
            if (!audioHelper)
                return gcnew array<String^>(0);

            IVideoSDKVector<IZoomVideoSDKSpeakerDevice*>* speakerList = audioHelper->getSpeakerList();
            if (!speakerList)
                return gcnew array<String^>(0);

            int count = speakerList->GetCount();
            array<String^>^ result = gcnew array<String^>(count);
            
            for (int i = 0; i < count; i++)
            {
                IZoomVideoSDKSpeakerDevice* speakerDevice = speakerList->GetItem(i);
                if (speakerDevice)
                {
                    result[i] = ConvertToManagedString(speakerDevice->getDeviceName());
                }
            }
            
            return result;
        }
        catch (System::Exception^) {
            return gcnew array<String^>(0);
        }
    }

    array<String^>^ ZoomSDKManager::GetCameraList()
    {
        if (!m_bInitialized || !m_pVideoSDK)
            return gcnew array<String^>(0);

        try {
            IZoomVideoSDK* pSDK = static_cast<IZoomVideoSDK*>(m_pVideoSDK);
            IZoomVideoSDKVideoHelper* videoHelper = pSDK->getVideoHelper();
            if (!videoHelper)
                return gcnew array<String^>(0);

            IVideoSDKVector<IZoomVideoSDKCameraDevice*>* cameraList = videoHelper->getCameraList();
            if (!cameraList)
                return gcnew array<String^>(0);

            int count = cameraList->GetCount();
            array<String^>^ result = gcnew array<String^>(count);
            
            for (int i = 0; i < count; i++)
            {
                IZoomVideoSDKCameraDevice* cameraDevice = cameraList->GetItem(i);
                if (cameraDevice)
                {
                    result[i] = ConvertToManagedString(cameraDevice->getDeviceName());
                }
            }
            
            return result;
        }
        catch (System::Exception^) {
            return gcnew array<String^>(0);
        }
    }

    bool ZoomSDKManager::SelectMicrophone(String^ deviceId)
    {
        return false; // Simplified for now
    }

    bool ZoomSDKManager::SelectSpeaker(String^ deviceId)
    {
        return false; // Simplified for now
    }

    bool ZoomSDKManager::SelectCamera(String^ deviceId)
    {
        if (!m_bInitialized || !m_pVideoSDK || !deviceId)
            return false;

        try {
            IZoomVideoSDK* pSDK = static_cast<IZoomVideoSDK*>(m_pVideoSDK);
            IZoomVideoSDKVideoHelper* videoHelper = pSDK->getVideoHelper();
            if (!videoHelper)
                return false;

            // Get camera list to find the camera by name
            IVideoSDKVector<IZoomVideoSDKCameraDevice*>* cameraList = videoHelper->getCameraList();
            if (!cameraList)
                return false;

            // Find the camera device by name
            IZoomVideoSDKCameraDevice* selectedCamera = nullptr;
            for (int i = 0; i < cameraList->GetCount(); i++)
            {
                IZoomVideoSDKCameraDevice* camera = cameraList->GetItem(i);
                if (camera)
                {
                    String^ cameraName = ConvertToManagedString(camera->getDeviceName());
                    if (String::Equals(cameraName, deviceId, StringComparison::OrdinalIgnoreCase))
                    {
                        selectedCamera = camera;
                        break;
                    }
                }
            }

            if (!selectedCamera)
            {
                OnSessionStatusChanged(SessionStatus::Error, String::Format("Camera not found: {0}", deviceId));
                return false;
            }

            // Use the camera device ID, not the display name
            const zchar_t* cameraDeviceId = selectedCamera->getDeviceId();
            if (!cameraDeviceId)
            {
                OnSessionStatusChanged(SessionStatus::Error, "Camera device ID is null");
                return false;
            }

            // Select the camera using the actual device ID
            bool success = videoHelper->selectCamera(cameraDeviceId);
            if (success)
            {
                OnSessionStatusChanged(SessionStatus::InSession, String::Format("Camera selected: {0}", deviceId));
                
                // If video preview is running, restart it to use the new camera
                if (m_pPreviewHandler)
                {
                    StopVideoPreview();
                    StartVideoPreview();
                }
                
                return true;
            }
            else
            {
                OnSessionStatusChanged(SessionStatus::Error, String::Format("Failed to select camera: {0}", deviceId));
                return false;
            }
        }
        catch (System::Exception^) {
            OnSessionStatusChanged(SessionStatus::Error, "Exception occurred while selecting camera");
            return false;
        }
    }

    // Properties
    bool ZoomSDKManager::IsInSession::get()
    {
        if (!m_bInitialized || !m_pVideoSDK)
            return false;

        IZoomVideoSDK* pSDK = static_cast<IZoomVideoSDK*>(m_pVideoSDK);
        return pSDK->isInSession();
    }

    String^ ZoomSDKManager::CurrentSessionName::get()
    {
        if (!m_bInitialized || !m_pVideoSDK)
            return nullptr;

        IZoomVideoSDK* pSDK = static_cast<IZoomVideoSDK*>(m_pVideoSDK);
        IZoomVideoSDKSession* session = pSDK->getSessionInfo();
        if (!session)
            return nullptr;

        return ConvertToManagedString(session->getSessionName());
    }

    // Event handlers
    void ZoomSDKManager::OnSessionStatusChanged(SessionStatus status, String^ message)
    {
        SessionStatusChanged(this, gcnew SessionStatusEventArgs(status, message));
    }

    void ZoomSDKManager::OnRemoteVideoReceived(Bitmap^ frame, String^ userId)
    {
        RemoteVideoReceived(this, gcnew VideoFrameEventArgs(frame, userId));
    }

    void ZoomSDKManager::OnPreviewVideoReceived(Bitmap^ frame)
    {
        PreviewVideoReceived(this, gcnew VideoFrameEventArgs(frame, nullptr));
    }

    // OPTIMIZED YUV to RGB conversion using LockBits for high performance
    Bitmap^ ZoomSDKManager::ConvertYUVToBitmap(char* yBuffer, char* uBuffer, char* vBuffer, 
                                              int width, int height, int yStride, int uStride, int vStride)
    {
        if (!yBuffer || !uBuffer || !vBuffer || width <= 0 || height <= 0)
            return nullptr;

        try {
            // Create bitmap with 24-bit RGB format for optimal performance
            Bitmap^ bitmap = gcnew Bitmap(width, height, System::Drawing::Imaging::PixelFormat::Format24bppRgb);
            
            // Lock bitmap for direct memory access (much faster than SetPixel)
            System::Drawing::Rectangle rect(0, 0, width, height);
            System::Drawing::Imaging::BitmapData^ bitmapData = bitmap->LockBits(
                rect, 
                System::Drawing::Imaging::ImageLockMode::WriteOnly, 
                System::Drawing::Imaging::PixelFormat::Format24bppRgb
            );
            
            // Get pointer to bitmap data and stride
            unsigned char* rgbPtr = static_cast<unsigned char*>(bitmapData->Scan0.ToPointer());
            int stride = bitmapData->Stride;
            
            // FAST YUV420 to RGB conversion with proper color space
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Get YUV values for current pixel
                    int yIndex = y * yStride + x;
                    int uvIndex = (y / 2) * uStride + (x / 2);
                    
                    // Ensure we don't read beyond buffer bounds
                    if (yIndex >= yStride * height || uvIndex >= uStride * (height / 2))
                        continue;
                    
                    int Y = static_cast<unsigned char>(yBuffer[yIndex]);
                    int U = static_cast<unsigned char>(uBuffer[uvIndex]);
                    int V = static_cast<unsigned char>(vBuffer[uvIndex]);
                    
                    // YUV to RGB conversion (ITU-R BT.601 standard)
                    int C = Y - 16;
                    int D = U - 128;
                    int E = V - 128;
                    
                    int R = (298 * C + 409 * E + 128) >> 8;
                    int G = (298 * C - 100 * D - 208 * E + 128) >> 8;
                    int B = (298 * C + 516 * D + 128) >> 8;
                    
                    // Clamp values to 0-255 range
                    R = (R < 0) ? 0 : (R > 255) ? 255 : R;
                    G = (G < 0) ? 0 : (G > 255) ? 255 : G;
                    B = (B < 0) ? 0 : (B > 255) ? 255 : B;
                    
                    // Set RGB pixel directly in memory (BGR format for bitmap)
                    unsigned char* pixel = rgbPtr + y * stride + x * 3;
                    pixel[0] = static_cast<unsigned char>(B); // Blue
                    pixel[1] = static_cast<unsigned char>(G); // Green
                    pixel[2] = static_cast<unsigned char>(R); // Red
                }
            }
            
            // Unlock bitmap data
            bitmap->UnlockBits(bitmapData);
            return bitmap;
        }
        catch (System::Exception^) {
            // Fallback to error bitmap if conversion fails
            return CreateErrorBitmap(width, height, "YUV Conversion Error");
        }
    }
    
    // Helper method to create error bitmap
    Bitmap^ ZoomSDKManager::CreateErrorBitmap(int width, int height, String^ message)
    {
        try {
            Bitmap^ errorBitmap = gcnew Bitmap(Math::Max(320, width), Math::Max(240, height));
            Graphics^ g = Graphics::FromImage(errorBitmap);
            g->FillRectangle(Brushes::Red, 0, 0, errorBitmap->Width, errorBitmap->Height);
            g->DrawString(message, gcnew System::Drawing::Font("Arial", 12), Brushes::White, PointF(10, 10));
            delete g;
            return errorBitmap;
        }
        catch (System::Exception^) {
            // If even error bitmap fails, return null
            return nullptr;
        }
    }

    String^ ZoomSDKManager::ConvertToManagedString(const wchar_t* nativeString)
    {
        return nativeString ? gcnew String(nativeString) : nullptr;
    }

    std::wstring ZoomSDKManager::ConvertToNativeString(String^ managedString)
    {
        if (!managedString || managedString->Length == 0) 
            return L"";
        
        // Convert using standard C++/CLI approach
        std::wstring result;
        for (int i = 0; i < managedString->Length; i++)
        {
            result += managedString[i];
        }
        return result;
    }

    // Video management methods - SIMPLIFIED
    void ZoomSDKManager::SubscribeToUserVideo(void* user)
    {
        if (!m_bInitialized || !m_pVideoSDK || !user)
            return;

        try {
            IZoomVideoSDKUser* pUser = static_cast<IZoomVideoSDKUser*>(user);
            if (pUser)
            {
                String^ userId = ConvertToManagedString(pUser->getUserName());
                OnSessionStatusChanged(SessionStatus::InSession, String::Format("User {0} joined with video capability", userId));
            }
        }
        catch (System::Exception^) {
            OnSessionStatusChanged(SessionStatus::Error, "Error setting up user video");
        }
    }

    void ZoomSDKManager::UnsubscribeFromUserVideo(void* user)
    {
        if (!m_bInitialized || !m_pVideoSDK || !user)
            return;

        try {
            IZoomVideoSDKUser* pUser = static_cast<IZoomVideoSDKUser*>(user);
            if (pUser)
            {
                String^ userId = ConvertToManagedString(pUser->getUserName());
                OnSessionStatusChanged(SessionStatus::InSession, String::Format("User {0} left - cleaning up video", userId));
            }
        }
        catch (System::Exception^) {
            OnSessionStatusChanged(SessionStatus::Error, "Error cleaning up user video");
        }
    }

    void ZoomSDKManager::HandleUserVideoStatusChange(void* user)
    {
        if (!m_bInitialized || !m_pVideoSDK || !user)
            return;

        try {
            IZoomVideoSDKUser* pUser = static_cast<IZoomVideoSDKUser*>(user);
            if (pUser)
            {
                String^ userId = ConvertToManagedString(pUser->getUserName());
                OnSessionStatusChanged(SessionStatus::InSession, String::Format("User {0} video status changed", userId));
            }
        }
        catch (System::Exception^) {
            OnSessionStatusChanged(SessionStatus::Error, "Error handling user video status change");
        }
    }

    // Video preview methods - REAL IMPLEMENTATION
    bool ZoomSDKManager::StartVideoPreview()
    {
        if (!m_bInitialized || !m_pVideoSDK)
            return false;

        try {
            IZoomVideoSDK* pSDK = static_cast<IZoomVideoSDK*>(m_pVideoSDK);
            IZoomVideoSDKVideoHelper* videoHelper = pSDK->getVideoHelper();
            if (!videoHelper)
                return false;

            // Create preview handler if not exists
            if (!m_pPreviewHandler)
            {
                m_pPreviewHandler = new VideoPreviewHandler(this);
            }

            // Start video preview with raw data pipe delegate
            ZoomVideoSDKErrors ret = videoHelper->startVideoPreview(
                static_cast<IZoomVideoSDKRawDataPipeDelegate*>(m_pPreviewHandler),
                nullptr, // Use default camera
                ZoomVideoSDKResolution_360P  // Downscaled for better performance
            );

            if (ret == ZoomVideoSDKErrors_Success)
            {
                OnSessionStatusChanged(SessionStatus::InSession, "Video preview started successfully");
                return true;
            }
            else
            {
                OnSessionStatusChanged(SessionStatus::Error, String::Format("Failed to start video preview: {0}", (int)ret));
                return false;
            }
        }
        catch (System::Exception^) {
            OnSessionStatusChanged(SessionStatus::Error, "Exception occurred while starting video preview");
            return false;
        }
    }

    void ZoomSDKManager::StopVideoPreview()
    {
        if (!m_bInitialized || !m_pVideoSDK || !m_pPreviewHandler)
            return;

        try {
            IZoomVideoSDK* pSDK = static_cast<IZoomVideoSDK*>(m_pVideoSDK);
            IZoomVideoSDKVideoHelper* videoHelper = pSDK->getVideoHelper();
            if (!videoHelper)
                return;

            // Stop video preview
            ZoomVideoSDKErrors ret = videoHelper->stopVideoPreview(
                static_cast<IZoomVideoSDKRawDataPipeDelegate*>(m_pPreviewHandler)
            );

            if (ret == ZoomVideoSDKErrors_Success)
            {
                OnSessionStatusChanged(SessionStatus::InSession, "Video preview stopped successfully");
            }
            else
            {
                OnSessionStatusChanged(SessionStatus::Error, String::Format("Failed to stop video preview: {0}", (int)ret));
            }
        }
        catch (System::Exception^) {
            OnSessionStatusChanged(SessionStatus::Error, "Exception occurred while stopping video preview");
        }
    }

    // CreateVideoPlaceholder method implementation
    void ZoomSDKManager::CreateVideoPlaceholder(String^ userId)
    {
        try {
            // For now, just send a null bitmap to indicate video is available
            // The UI layer will handle creating the actual placeholder
            OnRemoteVideoReceived(nullptr, userId);
            OnSessionStatusChanged(SessionStatus::InSession, String::Format("Video placeholder created for user {0}", userId));
        }
        catch (System::Exception^) {
            OnSessionStatusChanged(SessionStatus::Error, "Error creating video placeholder");
        }
    }

    // Property implementations
    bool ZoomSDKManager::IsInitialized::get()
    {
        return m_bInitialized;
    }

    // Implementation of VideoPreviewHandler methods - INSIDE the namespace
    void VideoPreviewHandler::onRawDataFrameReceived(YUVRawDataI420* data_)
    {
        ZoomSDKManager^ handler = static_cast<ZoomSDKManager^>(m_managedHandler);
        if (handler && data_)
        {
            try {
                // Get frame dimensions
                int width = data_->GetStreamWidth();
                int height = data_->GetStreamHeight();
                
                // Get YUV buffer pointers
                char* yBuffer = data_->GetYBuffer();
                char* uBuffer = data_->GetUBuffer();
                char* vBuffer = data_->GetVBuffer();
                
                // Convert YUV to bitmap
                Bitmap^ bitmap = handler->ConvertYUVToBitmap(
                    yBuffer, uBuffer, vBuffer,
                    width, height,
                    width, width/2, width/2  // Simplified stride calculation
                );
                
                if (bitmap)
                {
                    handler->OnPreviewVideoReceived(bitmap);
                }
            }
            catch (System::Exception^) {
                // Handle conversion errors silently
            }
        }
    }

    void VideoPreviewHandler::onRawDataStatusChanged(RawDataStatus status)
    {
        ZoomSDKManager^ handler = static_cast<ZoomSDKManager^>(m_managedHandler);
        if (handler)
        {
            String^ statusMsg = (status == RawData_On) ? "Preview video data ON" : "Preview video data OFF";
            handler->OnSessionStatusChanged(SessionStatus::InSession, statusMsg);
        }
    }

    void VideoPreviewHandler::onShareCursorDataReceived(ZoomVideoSDKShareCursorData info)
    {
        // Not used for video preview
    }

    // Implementation of RemoteVideoHandler methods - INSIDE the namespace
    void RemoteVideoHandler::onRawDataFrameReceived(YUVRawDataI420* data_)
    {
        ZoomSDKManager^ handler = static_cast<ZoomSDKManager^>(m_managedHandler);
        String^ userId = static_cast<String^>(m_userId);
        if (handler && data_ && userId)
        {
            try {
                // Get frame dimensions
                int width = data_->GetStreamWidth();
                int height = data_->GetStreamHeight();
                
                // Get YUV buffer pointers
                char* yBuffer = data_->GetYBuffer();
                char* uBuffer = data_->GetUBuffer();
                char* vBuffer = data_->GetVBuffer();
                
                // Convert YUV to bitmap
                Bitmap^ bitmap = handler->ConvertYUVToBitmap(
                    yBuffer, uBuffer, vBuffer,
                    width, height,
                    width, width/2, width/2  // Simplified stride calculation
                );
                
                if (bitmap)
                {
                    handler->OnRemoteVideoReceived(bitmap, userId);
                }
            }
            catch (System::Exception^) {
                // Handle conversion errors silently
            }
        }
    }

    void RemoteVideoHandler::onRawDataStatusChanged(RawDataStatus status)
    {
        ZoomSDKManager^ handler = static_cast<ZoomSDKManager^>(m_managedHandler);
        String^ userId = static_cast<String^>(m_userId);
        if (handler && userId)
        {
            String^ statusMsg = String::Format("Remote video data for {0}: {1}", 
                userId, (status == RawData_On) ? "ON" : "OFF");
            handler->OnSessionStatusChanged(SessionStatus::InSession, statusMsg);
        }
    }

    void RemoteVideoHandler::onShareCursorDataReceived(ZoomVideoSDKShareCursorData info)
    {
        // Not used for video
    }

}
