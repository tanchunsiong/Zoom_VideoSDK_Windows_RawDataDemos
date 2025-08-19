using System;
using ZoomVideoSDKWrapper;

namespace ZoomVideoSDK.WinForms
{
    // Managed wrapper class that uses the real C++/CLI wrapper
    public class ZoomSDKManager
    {
        private ZoomVideoSDKWrapper.ZoomSDKManager _nativeManager;
        private bool _isInitialized = false;
        private bool _isInSession = false;

        // Events
        public event EventHandler SessionJoined;
        public event EventHandler SessionLeft;
        public event EventHandler<string> SessionError;
        public event EventHandler<string> StatusChanged;
        public event EventHandler<VideoFrameEventArgs> RemoteVideoReceived;
        public event EventHandler<VideoFrameEventArgs> PreviewVideoReceived;

        public bool IsInitialized => _isInitialized;
        public bool IsInSession => _isInSession;

        public ZoomSDKManager()
        {
            _nativeManager = new ZoomVideoSDKWrapper.ZoomSDKManager();
            
            // Subscribe to native manager events
            _nativeManager.SessionStatusChanged += OnSessionStatusChanged;
            _nativeManager.RemoteVideoReceived += OnRemoteVideoReceived;
            _nativeManager.PreviewVideoReceived += OnPreviewVideoReceived;
        }

        public bool Initialize()
        {
            try
            {
                bool result = _nativeManager.Initialize();
                _isInitialized = result;
                return result;
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Initialization failed: {ex.Message}");
                return false;
            }
        }

        public bool JoinSession(string sessionName, string token, string userName, string password = "")
        {
            if (!_isInitialized) 
            {
                StatusChanged?.Invoke(this, "SDK not initialized");
                return false;
            }

            try
            {
                return _nativeManager.JoinSession(sessionName, token, userName, password);
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Join session failed: {ex.Message}");
                return false;
            }
        }

        public bool LeaveSession()
        {
            if (!_isInSession) return false;

            try
            {
                _nativeManager.LeaveSession();
                return true;
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Leave session failed: {ex.Message}");
                return false;
            }
        }

        public bool MuteAudio(bool mute)
        {
            if (!_isInSession) return false;

            try
            {
                return _nativeManager.MuteMicrophone(mute);
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Mute audio failed: {ex.Message}");
                return false;
            }
        }

        public bool IsAudioMuted()
        {
            if (!_isInSession) return false;

            try
            {
                return _nativeManager.IsMicrophoneMuted();
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Check audio mute failed: {ex.Message}");
                return false;
            }
        }

        public bool StartVideo()
        {
            if (!_isInSession) return false;

            try
            {
                return _nativeManager.StartCamera();
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Start video failed: {ex.Message}");
                return false;
            }
        }

        public bool StopVideo()
        {
            if (!_isInSession) return false;

            try
            {
                return _nativeManager.StopCamera();
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Stop video failed: {ex.Message}");
                return false;
            }
        }

        public bool IsVideoStarted()
        {
            if (!_isInSession) return false;

            try
            {
                return _nativeManager.IsCameraStarted();
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Check video status failed: {ex.Message}");
                return false;
            }
        }

        public void Cleanup()
        {
            try
            {
                if (_nativeManager != null)
                {
                    _nativeManager.Cleanup();
                    _isInitialized = false;
                    _isInSession = false;
                }
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Cleanup failed: {ex.Message}");
            }
        }

        // Handle events from the native C++/CLI wrapper
        private void OnSessionStatusChanged(object sender, SessionStatusEventArgs e)
        {
            StatusChanged?.Invoke(this, $"Status: {e.Status} - {e.Message}");
            
            switch (e.Status)
            {
                case SessionStatus.InSession:
                    _isInSession = true;
                    SessionJoined?.Invoke(this, EventArgs.Empty);
                    break;
                    
                case SessionStatus.Idle:
                    _isInSession = false;
                    SessionLeft?.Invoke(this, EventArgs.Empty);
                    break;
                    
                case SessionStatus.Error:
                case SessionStatus.Failed:
                    _isInSession = false;
                    SessionError?.Invoke(this, e.Message);
                    break;
            }
        }

        private void OnRemoteVideoReceived(object sender, VideoFrameEventArgs e)
        {
            RemoteVideoReceived?.Invoke(this, e);
        }

        private void OnPreviewVideoReceived(object sender, VideoFrameEventArgs e)
        {
            PreviewVideoReceived?.Invoke(this, e);
        }

        // Dispose pattern
        public void Dispose()
        {
            Cleanup();
            if (_nativeManager != null)
            {
                _nativeManager.SessionStatusChanged -= OnSessionStatusChanged;
                _nativeManager = null;
            }
        }
    }
}
