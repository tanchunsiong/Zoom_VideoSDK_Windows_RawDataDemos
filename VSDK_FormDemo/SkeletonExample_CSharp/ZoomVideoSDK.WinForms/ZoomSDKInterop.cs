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
                // FIXED: Proper YUV420 to RGB conversion (based on Linux implementation)
                var bitmap = new Bitmap(width, height, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
                
                // YUV420 format: Y plane + U plane + V plane
                // Y plane: width * height bytes
                // U plane: (width/2) * (height/2) bytes  
                // V plane: (width/2) * (height/2) bytes
                int ySize = width * height;
                int uvSize = (width / 2) * (height / 2);
                
                if (yuvData.Length < ySize + 2 * uvSize)
                {
                    StatusChanged?.Invoke(this, $"Invalid YUV data size: {yuvData.Length}, expected: {ySize + 2 * uvSize}");
                    return CreateErrorBitmap(width, height, "Invalid YUV Size");
                }
                
                // Lock bitmap for direct pixel access (much faster)
                var bitmapData = bitmap.LockBits(new Rectangle(0, 0, width, height), 
                    System.Drawing.Imaging.ImageLockMode.WriteOnly, 
                    System.Drawing.Imaging.PixelFormat.Format24bppRgb);
                
                unsafe
                {
                    byte* rgbPtr = (byte*)bitmapData.Scan0;
                    int stride = bitmapData.Stride;
                    
                    // YUV420 to RGB conversion using standard coefficients
                    for (int y = 0; y < height; y++)
                    {
                        for (int x = 0; x < width; x++)
                        {
                            // Get Y, U, V values
                            int yIndex = y * width + x;
                            int uvIndex = (y / 2) * (width / 2) + (x / 2);
                            
                            int Y = yuvData[yIndex];
                            int U = yuvData[ySize + uvIndex];
                            int V = yuvData[ySize + uvSize + uvIndex];
                            
                            // YUV to RGB conversion (ITU-R BT.601)
                            int C = Y - 16;
                            int D = U - 128;
                            int E = V - 128;
                            
                            int R = (298 * C + 409 * E + 128) >> 8;
                            int G = (298 * C - 100 * D - 208 * E + 128) >> 8;
                            int B = (298 * C + 516 * D + 128) >> 8;
                            
                            // Clamp values to 0-255
                            R = Math.Max(0, Math.Min(255, R));
                            G = Math.Max(0, Math.Min(255, G));
                            B = Math.Max(0, Math.Min(255, B));
                            
                            // Set RGB pixel (BGR format for bitmap)
                            byte* pixel = rgbPtr + y * stride + x * 3;
                            pixel[0] = (byte)B; // Blue
                            pixel[1] = (byte)G; // Green  
                            pixel[2] = (byte)R; // Red
                        }
                    }
                }
                
                bitmap.UnlockBits(bitmapData);
                return bitmap;
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"YUV conversion failed: {ex.Message}");
                return CreateErrorBitmap(width, height, "YUV Conversion Error");
            }
        }
        
        private Bitmap CreateErrorBitmap(int width, int height, string message)
        {
            var errorBitmap = new Bitmap(Math.Max(320, width), Math.Max(240, height));
            using (var g = Graphics.FromImage(errorBitmap))
            {
                g.FillRectangle(Brushes.Red, 0, 0, errorBitmap.Width, errorBitmap.Height);
                g.DrawString(message, new Font("Arial", 12), Brushes.White, new PointF(10, 10));
            }
            return errorBitmap;
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
