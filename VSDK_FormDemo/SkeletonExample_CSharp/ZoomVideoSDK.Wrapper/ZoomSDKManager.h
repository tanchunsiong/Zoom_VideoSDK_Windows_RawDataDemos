#pragma once

#include <msclr\marshal_cppstd.h>
#include <string>

using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace System::Runtime::InteropServices;

namespace ZoomVideoSDKWrapper {

    public enum class SessionStatus {
        Idle,
        Joining,
        InSession,
        Leaving,
        Reconnecting,
        Failed,
        Error
    };

    public ref class SessionStatusEventArgs : EventArgs {
    public:
        property SessionStatus Status;
        property String^ Message;
        
        SessionStatusEventArgs(SessionStatus status, String^ message) {
            Status = status;
            Message = message;
        }
    };

    public ref class VideoFrameEventArgs : EventArgs {
    public:
        property Bitmap^ Frame;
        property String^ UserId;
        
        VideoFrameEventArgs(Bitmap^ frame, String^ userId) {
            Frame = frame;
            UserId = userId;
        }
    };

    public ref class ZoomSDKManager {
    private:
        void* m_pVideoSDK;
        void* m_pSession;
        bool m_bInitialized;
        
        // Native callback handlers
        void* m_pSessionHandler;
        void* m_pVideoHandler;
        void* m_pPreviewHandler;

    public:
        // Events
        event EventHandler<SessionStatusEventArgs^>^ SessionStatusChanged;
        event EventHandler<VideoFrameEventArgs^>^ RemoteVideoReceived;
        event EventHandler<VideoFrameEventArgs^>^ PreviewVideoReceived;

        ZoomSDKManager();
        ~ZoomSDKManager();
        !ZoomSDKManager();

        // SDK Management
        bool Initialize();
        void Cleanup();

        // Session Management
        bool JoinSession(String^ sessionName, String^ token, String^ userName, String^ password);
        void LeaveSession();
        
        // Audio Controls
        bool MuteMicrophone(bool mute);
        bool MuteSpeaker(bool mute);
        bool IsMicrophoneMuted();
        bool IsSpeakerMuted();

        // Video Controls
        bool StartCamera();
        bool StopCamera();
        bool IsCameraStarted();

        // Device Management
        array<String^>^ GetMicrophoneList();
        array<String^>^ GetSpeakerList();
        array<String^>^ GetCameraList();
        bool SelectMicrophone(String^ deviceId);
        bool SelectSpeaker(String^ deviceId);
        bool SelectCamera(String^ deviceId);

        // Properties
        property bool IsInSession { bool get(); }
        property String^ CurrentSessionName { String^ get(); }

        // Public event handlers for native callbacks
        void OnSessionStatusChanged(SessionStatus status, String^ message);
        void OnRemoteVideoReceived(Bitmap^ frame, String^ userId);
        void OnPreviewVideoReceived(Bitmap^ frame);
        
        // Video management methods (called from native callbacks)
        void SubscribeToUserVideo(void* user);
        void UnsubscribeFromUserVideo(void* user);
        void HandleUserVideoStatusChange(void* user);
        void CreateVideoPlaceholder(String^ userId);

    private:
        // Helper methods
        Bitmap^ ConvertYUVToBitmap(char* yBuffer, char* uBuffer, char* vBuffer, 
                                  int width, int height, int yStride, int uStride, int vStride);
        String^ ConvertToManagedString(const wchar_t* nativeString);
        std::wstring ConvertToNativeString(String^ managedString);
    };
}
