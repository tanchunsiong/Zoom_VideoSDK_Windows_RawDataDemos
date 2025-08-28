using System;
using System.Drawing;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;
using System.Windows.Media;
using System.IO;
using DrawingBrushes = System.Drawing.Brushes;
using MediaBrushes = System.Windows.Media.Brushes;
using DrawingColor = System.Drawing.Color;

namespace ZoomVideoSDK.WPF
{
    // Event args for video frames - WPF version using BitmapSource
    public class VideoFrameEventArgs : EventArgs
    {
        public BitmapSource Frame { get; set; }
        public string UserId { get; set; }
        
        public VideoFrameEventArgs(BitmapSource frame, string userId)
        {
            Frame = frame;
            UserId = userId;
        }
    }

    // Mock implementation for testing WPF functionality without C++ wrapper
    public class ZoomSDKInterop
    {
        private bool _isInitialized = false;
        private bool _isInSession = false;
        private bool _isVideoStarted = false;
        private bool _isAudioMuted = false;
        private Timer _videoTimer;
        private Random _random = new Random();

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
            StatusChanged?.Invoke(this, "Mock SDK Manager created for WPF testing");
        }

        public bool Initialize()
        {
            try
            {
                StatusChanged?.Invoke(this, "Initializing Mock Zoom Video SDK for WPF testing...");
                
                // Simulate initialization delay
                Thread.Sleep(500);
                
                _isInitialized = true;
                StatusChanged?.Invoke(this, "Mock SDK initialized successfully - ready for testing WPF UI");
                return true;
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Mock initialization failed: {ex.Message}");
                return false;
            }
        }

        public bool JoinSession(string sessionName, string token, string userName, string password = "")
        {
            if (!_isInitialized) 
            {
                StatusChanged?.Invoke(this, "Mock SDK not initialized");
                return false;
            }

            try
            {
                StatusChanged?.Invoke(this, $"Mock: Joining session '{sessionName}' as '{userName}'");
                
                // Simulate join delay
                Task.Run(async () =>
                {
                    await Task.Delay(1000);
                    _isInSession = true;
                    StatusChanged?.Invoke(this, $"Mock: Successfully joined session '{sessionName}'");
                    SessionJoined?.Invoke(this, EventArgs.Empty);
                });
                
                return true;
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Mock join session failed: {ex.Message}");
                return false;
            }
        }

        public bool LeaveSession()
        {
            if (!_isInSession) return false;

            try
            {
                StatusChanged?.Invoke(this, "Mock: Leaving session...");
                
                // Stop video if running
                if (_isVideoStarted)
                {
                    StopVideo();
                }
                
                _isInSession = false;
                StatusChanged?.Invoke(this, "Mock: Left session successfully");
                SessionLeft?.Invoke(this, EventArgs.Empty);
                return true;
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Mock leave session failed: {ex.Message}");
                return false;
            }
        }

        public bool MuteAudio(bool mute)
        {
            if (!_isInSession) return false;

            try
            {
                _isAudioMuted = mute;
                StatusChanged?.Invoke(this, $"Mock: Audio {(mute ? "muted" : "unmuted")}");
                return true;
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Mock mute audio failed: {ex.Message}");
                return false;
            }
        }

        public bool IsAudioMuted()
        {
            return _isAudioMuted;
        }

        public bool StartVideo()
        {
            if (!_isInSession) return false;

            try
            {
                StatusChanged?.Invoke(this, "Mock: Starting video...");
                
                _isVideoStarted = true;
                
                // Start mock video feed
                _videoTimer = new Timer(GenerateMockVideoFrame, null, 0, 100); // 10 FPS
                
                StatusChanged?.Invoke(this, "Mock: Video started - generating test frames");
                return true;
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Mock start video failed: {ex.Message}");
                return false;
            }
        }

        public bool StopVideo()
        {
            if (!_isInSession) return false;

            try
            {
                StatusChanged?.Invoke(this, "Mock: Stopping video...");
                
                _videoTimer?.Dispose();
                _videoTimer = null;
                _isVideoStarted = false;
                
                // Send null frames to clear video displays
                PreviewVideoReceived?.Invoke(this, new VideoFrameEventArgs(null, "self"));
                RemoteVideoReceived?.Invoke(this, new VideoFrameEventArgs(null, "remote_user"));
                
                StatusChanged?.Invoke(this, "Mock: Video stopped");
                return true;
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Mock stop video failed: {ex.Message}");
                return false;
            }
        }

        public bool IsVideoStarted()
        {
            return _isVideoStarted;
        }

        // Device enumeration methods - mock implementation
        public string[] GetMicrophoneList()
        {
            return new string[] 
            { 
                "Mock Microphone 1", 
                "Mock Microphone 2", 
                "Mock USB Headset Mic" 
            };
        }

        public string[] GetSpeakerList()
        {
            return new string[] 
            { 
                "Mock Speakers", 
                "Mock Headphones", 
                "Mock USB Audio Device" 
            };
        }

        public string[] GetCameraList()
        {
            return new string[] 
            { 
                "Mock Camera 1", 
                "Mock USB Webcam", 
                "Mock Integrated Camera" 
            };
        }

        private void GenerateMockVideoFrame(object state)
        {
            if (!_isVideoStarted) return;

            try
            {
                // Generate mock video frames
                var selfFrame = CreateMockVideoFrame(320, 240, "SELF VIDEO", DrawingColor.Blue);
                var remoteFrame = CreateMockVideoFrame(320, 240, "REMOTE VIDEO", DrawingColor.Green);

                PreviewVideoReceived?.Invoke(this, new VideoFrameEventArgs(selfFrame, "self"));
                RemoteVideoReceived?.Invoke(this, new VideoFrameEventArgs(remoteFrame, "remote_user"));
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Mock video frame generation failed: {ex.Message}");
            }
        }

        private BitmapSource CreateMockVideoFrame(int width, int height, string text, DrawingColor backgroundColor)
        {
            try
            {
                var bitmap = new Bitmap(width, height);
                using (var g = Graphics.FromImage(bitmap))
                {
                    // Fill background with random color variation
                    var bgColor = DrawingColor.FromArgb(
                        Math.Max(0, backgroundColor.R + _random.Next(-30, 30)),
                        Math.Max(0, backgroundColor.G + _random.Next(-30, 30)),
                        Math.Max(0, backgroundColor.B + _random.Next(-30, 30))
                    );
                    
                    g.FillRectangle(new SolidBrush(bgColor), 0, 0, width, height);
                    
                    // Add some animated elements
                    var time = DateTime.Now.Millisecond;
                    var x = (int)(Math.Sin(time * 0.01) * 50 + width / 2);
                    var y = (int)(Math.Cos(time * 0.01) * 30 + height / 2);
                    
                    g.FillEllipse(DrawingBrushes.White, x - 10, y - 10, 20, 20);
                    
                    // Add text
                    using (var font = new Font("Arial", 12, FontStyle.Bold))
                    {
                        var textSize = g.MeasureString(text, font);
                        var textX = (width - textSize.Width) / 2;
                        var textY = height - textSize.Height - 10;
                        
                        g.DrawString(text, font, DrawingBrushes.White, textX, textY);
                        g.DrawString($"Frame: {DateTime.Now:HH:mm:ss.fff}", 
                                   new Font("Arial", 8), DrawingBrushes.Yellow, 5, 5);
                    }
                }
                
                return ConvertBitmapToBitmapSource(bitmap);
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Mock frame creation failed: {ex.Message}");
                return CreateErrorBitmapSource(width, height, "Frame Error");
            }
        }

        // WPF-specific method to convert System.Drawing.Bitmap to WPF BitmapSource
        private BitmapSource ConvertBitmapToBitmapSource(Bitmap bitmap)
        {
            if (bitmap == null) return null;

            try
            {
                using (var memory = new MemoryStream())
                {
                    bitmap.Save(memory, System.Drawing.Imaging.ImageFormat.Png);
                    memory.Position = 0;
                    
                    var bitmapImage = new BitmapImage();
                    bitmapImage.BeginInit();
                    bitmapImage.StreamSource = memory;
                    bitmapImage.CacheOption = BitmapCacheOption.OnLoad;
                    bitmapImage.EndInit();
                    bitmapImage.Freeze(); // Make it thread-safe
                    
                    return bitmapImage;
                }
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Bitmap conversion failed: {ex.Message}");
                return CreateErrorBitmapSource(bitmap?.Width ?? 320, bitmap?.Height ?? 240, "Conversion Error");
            }
        }

        private BitmapSource CreateErrorBitmapSource(int width, int height, string message)
        {
            try
            {
                var writeableBitmap = new WriteableBitmap(width, height, 96, 96, PixelFormats.Bgr24, null);
                writeableBitmap.Lock();
                
                unsafe
                {
                    var backBuffer = (byte*)writeableBitmap.BackBuffer;
                    int stride = writeableBitmap.BackBufferStride;
                    
                    for (int y = 0; y < height; y++)
                    {
                        for (int x = 0; x < width; x++)
                        {
                            byte* pixel = backBuffer + y * stride + x * 3;
                            pixel[0] = 0;   // Blue
                            pixel[1] = 0;   // Green
                            pixel[2] = 255; // Red
                        }
                    }
                }
                
                writeableBitmap.AddDirtyRect(new System.Windows.Int32Rect(0, 0, width, height));
                writeableBitmap.Unlock();
                writeableBitmap.Freeze();
                
                return writeableBitmap;
            }
            catch
            {
                return null;
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

                _videoTimer?.Dispose();
                _videoTimer = null;
                _isInitialized = false;
                
                StatusChanged?.Invoke(this, "Mock SDK cleaned up successfully");
            }
            catch (Exception ex)
            {
                StatusChanged?.Invoke(this, $"Mock cleanup failed: {ex.Message}");
            }
        }

        // Dispose pattern
        public void Dispose()
        {
            Cleanup();
        }
    }
}
