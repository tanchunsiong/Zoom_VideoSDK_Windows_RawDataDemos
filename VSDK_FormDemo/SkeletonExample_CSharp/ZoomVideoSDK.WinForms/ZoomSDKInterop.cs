using System;
using System.Drawing;
using System.Threading;
using System.Threading.Tasks;
using ZoomVideoSDKWrapper;

namespace ZoomVideoSDK.WinForms
{
    // Event args for video frames
    public class VideoFrameEventArgs : EventArgs
    {
        public Bitmap Frame { get; set; }
        public string UserId { get; set; }
        
        public VideoFrameEventArgs(Bitmap frame, string userId)
        {
            Frame = frame;
            UserId = userId;
        }
    }

    // Managed wrapper class using C++/CLI wrapper
    public class ZoomSDKInterop
    {
        private ZoomSDKManager _sdkManager;
        private bool _isInitialized = false;
        private bool _isInSession = false;
        private bool _isVideoStarted = false;

        // Events
        public event EventHandler SessionJoined;
        public event EventHandler SessionLeft;
        public event EventHandler<string> SessionError;
        public event EventHandler<string> StatusChanged;
        public event EventHandler<VideoFrameEventArgs> RemoteVideoReceived;
        public event EventHandler<VideoFrameEventArgs> PreviewVideoReceived;

        public bool IsInitialized => _isInitialized;
        public bool IsInSession => _isInSession;

        public ZoomSDKInterop()
        {
            try
            {
                // Create the C++/CLI wrapper instance
                _sdkManager = new ZoomSDKManager();
                
                // Subscribe to events from the C++/CLI wrapper
                _sdkManager.SessionStatusChanged += (sender, e) => 
                {
                    StatusChanged?.Invoke(this, $"Session status: {e.Status} - {e.Message}");
                    
                    switch (e.Status)
                    {
                        case SessionStatus.InSession:
                            _isInSession = true;
                            SessionJoined?.Invoke(this, EventArgs.Empty);
                            break;
                        case SessionStatus.Idle:
                            if (_isInSession) // Only fire if we were previously in session
                            {
                                _isInSession = false;
                                SessionLeft?.Invoke(this, EventArgs.Empty);
                            }
                            break;
                        case SessionStatus.Failed:
                        case SessionStatus.Error:
                            SessionError?.Invoke(this, e.Message);
                            break;
                    }
                };
                
                _sdkManager.RemoteVideoReceived += (sender, e) => 
                {
                    RemoteVideoReceived?.Invoke(this, new VideoFrameEventArgs(e.Frame, e.UserId));
                };
                
                _sdkManager.PreviewVideoReceived += (sender, e) => 
                {
                    PreviewVideoReceived?.Invoke(this, new VideoFrameEventArgs(e.Frame, "self"));
                };

                StatusChanged?.Invoke(this, "C++/CLI SDK Manager created successfully");
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Failed to create SDK Manager: {ex.Message}");
            }
        }

        public bool Initialize()
        {
            try
            {
                StatusChanged?.Invoke(this, "Initializing Zoom Video SDK with C++/CLI wrapper...");
                
                if (_sdkManager == null)
                {
                    StatusChanged?.Invoke(this, "SDK initialization failed - no SDK manager");
                    return false;
                }

                // Call the C++/CLI wrapper initialize method
                bool result = _sdkManager.Initialize();
                
                if (result)
                {
                    _isInitialized = true;
                    StatusChanged?.Invoke(this, "SDK initialized successfully with real Zoom SDK via C++/CLI");
                    return true;
                }
                else
                {
                    StatusChanged?.Invoke(this, "SDK initialization failed via C++/CLI wrapper");
                    return false;
                }
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
                StatusChanged?.Invoke(this, $"Attempting to join session '{sessionName}' as '{userName}' with C++/CLI wrapper");
                
                // Call the C++/CLI wrapper join session method
                bool result = _sdkManager.JoinSession(sessionName, token, userName, password);
                
                if (result)
                {
                    StatusChanged?.Invoke(this, $"Join session initiated for '{sessionName}' - waiting for connection...");
                    return true;
                }
                else
                {
                    StatusChanged?.Invoke(this, "Join session failed via C++/CLI wrapper");
                    return false;
                }
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
                StatusChanged?.Invoke(this, "Leaving session with C++/CLI wrapper...");
                
                // Stop video if running
                if (_isVideoStarted)
                {
                    StopVideo();
                }
                
                // Call the C++/CLI wrapper leave session method
                _sdkManager.LeaveSession();
                StatusChanged?.Invoke(this, "Leave session initiated via C++/CLI wrapper");
                return true;
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Leave session failed: {ex.Message}");
                // Still update local state even if exception occurred
                _isInSession = false;
                SessionLeft?.Invoke(this, EventArgs.Empty);
                return false;
            }
        }

        public bool MuteAudio(bool mute)
        {
            if (!_isInSession) return false;

            try
            {
                // Call the C++/CLI wrapper mute audio method
                bool result = _sdkManager.MuteAudio(mute);
                
                if (result)
                {
                    StatusChanged?.Invoke(this, $"Audio {(mute ? "muted" : "unmuted")} via C++/CLI wrapper");
                    return true;
                }
                else
                {
                    StatusChanged?.Invoke(this, $"Failed to {(mute ? "mute" : "unmute")} audio via C++/CLI wrapper");
                    return false;
                }
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Mute audio failed: {ex.Message}");
                return false;
            }
        }

        public bool IsAudioMuted()
        {
            try
            {
                if (_sdkManager != null && _isInSession)
                {
                    return _sdkManager.IsAudioMuted();
                }
                return false;
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Failed to get audio mute status: {ex.Message}");
                return false;
            }
        }

        public bool StartVideo()
        {
            if (!_isInSession) return false;

            try
            {
                StatusChanged?.Invoke(this, "Starting video via C++/CLI wrapper...");
                
                // Call the C++/CLI wrapper start video method
                bool result = _sdkManager.StartVideo();
                
                if (result)
                {
                    _isVideoStarted = true;
                    StatusChanged?.Invoke(this, "Video started successfully via C++/CLI wrapper");
                    return true;
                }
                else
                {
                    StatusChanged?.Invoke(this, "Failed to start video via C++/CLI wrapper");
                    return false;
                }
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
                StatusChanged?.Invoke(this, "Stopping video via C++/CLI wrapper...");
                
                // Call the C++/CLI wrapper stop video method
                bool result = _sdkManager.StopVideo();
                
                if (result)
                {
                    _isVideoStarted = false;
                    StatusChanged?.Invoke(this, "Video stopped successfully via C++/CLI wrapper");
                    return true;
                }
                else
                {
                    StatusChanged?.Invoke(this, "Failed to stop video via C++/CLI wrapper");
                    return false;
                }
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Stop video failed: {ex.Message}");
                return false;
            }
        }

        public bool IsVideoStarted()
        {
            try
            {
                if (_sdkManager != null && _isInSession)
                {
                    return _sdkManager.IsVideoStarted();
                }
                return _isVideoStarted;
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Failed to get video status: {ex.Message}");
                return _isVideoStarted;
            }
        }

        // Device enumeration methods using C++/CLI wrapper
        public string[] GetMicrophoneList()
        {
            try
            {
                if (_sdkManager != null)
                {
                    return _sdkManager.GetMicrophoneList();
                }
                return new string[] { "Default Microphone" };
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Failed to get microphone list: {ex.Message}");
                return new string[] { "Default Microphone" };
            }
        }

        public string[] GetSpeakerList()
        {
            try
            {
                if (_sdkManager != null)
                {
                    return _sdkManager.GetSpeakerList();
                }
                return new string[] { "Default Speaker" };
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Failed to get speaker list: {ex.Message}");
                return new string[] { "Default Speaker" };
            }
        }

        public string[] GetCameraList()
        {
            try
            {
                if (_sdkManager != null)
                {
                    return _sdkManager.GetCameraList();
                }
                return new string[] { "Default Camera" };
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Failed to get camera list: {ex.Message}");
                return new string[] { "Default Camera" };
            }
        }

        // Helper method to convert YUV420 data to RGB bitmap
        private Bitmap ConvertYUVToBitmap(byte[] yuvData, int width, int height)
        {
            try
            {
                // This is a simplified YUV420 to RGB conversion
                // In a real implementation, you'd use proper YUV to RGB conversion
                var bitmap = new Bitmap(width, height);
                
                // For now, create a placeholder with the actual dimensions
                using (var g = Graphics.FromImage(bitmap))
                {
                    g.FillRectangle(Brushes.DarkBlue, 0, 0, width, height);
                    g.DrawString($"Video Frame {width}x{height}", 
                               new Font("Arial", 12), 
                               Brushes.White, 
                               new PointF(10, 10));
                    g.DrawString($"YUV Data: {yuvData.Length} bytes", 
                               new Font("Arial", 10), 
                               Brushes.LightGray, 
                               new PointF(10, 30));
                }
                
                return bitmap;
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"YUV conversion failed: {ex.Message}");
                
                // Return a simple error bitmap
                var errorBitmap = new Bitmap(320, 240);
                using (var g = Graphics.FromImage(errorBitmap))
                {
                    g.FillRectangle(Brushes.Red, 0, 0, 320, 240);
                    g.DrawString("Video Error", new Font("Arial", 12), Brushes.White, new PointF(10, 10));
                }
                return errorBitmap;
            }
        }

        public void Cleanup()
        {
            try
            {
                if (_isInSession)
                {
                    LeaveSession();
                }

                if (_sdkManager != null)
                {
                    _sdkManager.Cleanup();
                    _sdkManager = null;
                }

                _isInitialized = false;
                StatusChanged?.Invoke(this, "SDK cleaned up successfully via C++/CLI wrapper");
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Cleanup failed: {ex.Message}");
            }
        }

        // Dispose pattern
        public void Dispose()
        {
            Cleanup();
        }
    }
}
