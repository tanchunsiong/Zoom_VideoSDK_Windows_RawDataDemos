using System;
using System.IO;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Imaging;
using System.Windows.Threading;
using Newtonsoft.Json;

namespace ZoomVideoSDK.WPF
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        // Configuration
        private SessionConfig _config;
        
        // SDK Manager
        private ZoomSDKInterop _zoomSDK;
        private bool _isInSession = false;
        private bool _isMicMuted = false;
        private bool _isSpeakerMuted = false;
        private bool _isVideoStarted = false;
        
        // Frame rate throttling for smooth video
        private DateTime _lastRemoteVideoUpdate = DateTime.MinValue;
        private DateTime _lastPreviewVideoUpdate = DateTime.MinValue;
        private readonly TimeSpan _videoUpdateInterval = TimeSpan.FromMilliseconds(33); // ~30fps

        public MainWindow()
        {
            InitializeComponent();
            LoadConfiguration();
            InitializeZoomSDK();
            UpdateButtonStates();
            PopulateDeviceLists();
            SetupEventHandlers();
        }

        private void SetupEventHandlers()
        {
            // Wire up button click events
            PreviewTokenButton.Click += PreviewTokenButton_Click;
            JoinButton.Click += JoinButton_Click;
            LeaveButton.Click += LeaveButton_Click;
            MuteMicButton.Click += MuteMicButton_Click;
            MuteSpeakerButton.Click += MuteSpeakerButton_Click;
            StartVideoButton.Click += StartVideoButton_Click;
            StopVideoButton.Click += StopVideoButton_Click;
        }

        private void PopulateDeviceLists()
        {
            try
            {
                // Clear existing items
                MicrophoneComboBox.Items.Clear();
                SpeakerComboBox.Items.Clear();
                CameraComboBox.Items.Clear();

                if (_zoomSDK != null && _zoomSDK.IsInitialized)
                {
                    // Get real device lists from SDK
                    var microphones = _zoomSDK.GetMicrophoneList();
                    var speakers = _zoomSDK.GetSpeakerList();
                    var cameras = _zoomSDK.GetCameraList();

                    // Populate microphones
                    foreach (var mic in microphones)
                    {
                        MicrophoneComboBox.Items.Add(mic);
                    }

                    // Populate speakers
                    foreach (var speaker in speakers)
                    {
                        SpeakerComboBox.Items.Add(speaker);
                    }

                    // Populate cameras
                    foreach (var camera in cameras)
                    {
                        CameraComboBox.Items.Add(camera);
                    }

                    UpdateStatus($"Found {microphones.Length} microphones, {speakers.Length} speakers, {cameras.Length} cameras");
                }
                else
                {
                    // Fallback to default devices if SDK not initialized
                    MicrophoneComboBox.Items.Add("Default Microphone");
                    SpeakerComboBox.Items.Add("Default Speaker");
                    CameraComboBox.Items.Add("Default Camera");
                    UpdateStatus("Using default devices (SDK not initialized)");
                }

                // Select first item in each list
                if (MicrophoneComboBox.Items.Count > 0) MicrophoneComboBox.SelectedIndex = 0;
                if (SpeakerComboBox.Items.Count > 0) SpeakerComboBox.SelectedIndex = 0;
                if (CameraComboBox.Items.Count > 0) CameraComboBox.SelectedIndex = 0;
            }
            catch (Exception ex)
            {
                UpdateStatus($"Error populating device lists: {ex.Message}");
                
                // Fallback to default devices on error
                MicrophoneComboBox.Items.Clear();
                SpeakerComboBox.Items.Clear();
                CameraComboBox.Items.Clear();
                
                MicrophoneComboBox.Items.Add("Default Microphone");
                SpeakerComboBox.Items.Add("Default Speaker");
                CameraComboBox.Items.Add("Default Camera");
                
                if (MicrophoneComboBox.Items.Count > 0) MicrophoneComboBox.SelectedIndex = 0;
                if (SpeakerComboBox.Items.Count > 0) SpeakerComboBox.SelectedIndex = 0;
                if (CameraComboBox.Items.Count > 0) CameraComboBox.SelectedIndex = 0;
            }
        }

        private void LoadConfiguration()
        {
            try
            {
                string configPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "config.json");
                if (File.Exists(configPath))
                {
                    string json = File.ReadAllText(configPath);
                    _config = JsonConvert.DeserializeObject<SessionConfig>(json);
                }
                else
                {
                    _config = new SessionConfig
                    {
                        SessionName = "TestSession",
                        Token = "your_jwt_token_here",
                        UserName = "TestUser"
                    };
                }

                // Populate UI with config values
                if (_config != null)
                {
                    SessionNameTextBox.Text = _config.SessionName ?? "";
                    TokenTextBox.Text = _config.Token ?? "";
                    UserNameTextBox.Text = _config.UserName ?? "";
                    PasswordTextBox.Password = _config.Password ?? "";
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error loading configuration: {ex.Message}", "Configuration Error", 
                               MessageBoxButton.OK, MessageBoxImage.Warning);
            }
        }

        private void UpdateButtonStates()
        {
            // Always enable join button when not in session
            JoinButton.IsEnabled = !_isInSession;
            LeaveButton.IsEnabled = _isInSession;
            
            MuteMicButton.IsEnabled = _isInSession;
            MuteSpeakerButton.IsEnabled = _isInSession;
            StartVideoButton.IsEnabled = _isInSession && !_isVideoStarted;
            StopVideoButton.IsEnabled = _isInSession && _isVideoStarted;
            
            MuteMicButton.Content = _isMicMuted ? "Unmute Mic" : "Mute Mic";
            MuteSpeakerButton.Content = _isSpeakerMuted ? "Unmute Speaker" : "Mute Speaker";
        }

        private void UpdateStatus(string message)
        {
            // WPF threading - use Dispatcher to update UI from any thread
            if (!Dispatcher.CheckAccess())
            {
                Dispatcher.BeginInvoke(new Action<string>(UpdateStatus), message);
                return;
            }
            
            StatusLabel.Text = message;
        }

        private void InitializeZoomSDK()
        {
            try
            {
                _zoomSDK = new ZoomSDKInterop();
                
                // Set up event handlers
                _zoomSDK.SessionJoined += OnSessionJoined;
                _zoomSDK.SessionLeft += OnSessionLeft;
                _zoomSDK.SessionError += OnSessionError;
                _zoomSDK.StatusChanged += OnStatusChanged;
                _zoomSDK.RemoteVideoReceived += OnRemoteVideoReceived;
                _zoomSDK.PreviewVideoReceived += OnPreviewVideoReceived;

                if (_zoomSDK.Initialize())
                {
                    UpdateStatus("Zoom SDK initialized successfully");
                    // Refresh device lists now that SDK is initialized
                    PopulateDeviceLists();
                }
                else
                {
                    UpdateStatus("Failed to initialize Zoom SDK");
                    MessageBox.Show("Failed to initialize Zoom SDK. Please ensure the SDK libraries are properly installed.", 
                                   "SDK Error", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }
            catch (Exception ex)
            {
                UpdateStatus($"SDK initialization error: {ex.Message}");
                MessageBox.Show($"SDK initialization error: {ex.Message}", "SDK Error", 
                               MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void OnStatusChanged(object sender, string message)
        {
            UpdateStatus(message);
        }

        private void OnSessionJoined(object sender, EventArgs e)
        {
            if (!Dispatcher.CheckAccess())
            {
                Dispatcher.BeginInvoke(new Action<object, EventArgs>(OnSessionJoined), sender, e);
                return;
            }

            _isInSession = true;
            UpdateButtonStates();
            UpdateStatus("Successfully joined Zoom session - use Start Video button to begin video");
        }

        private void OnSessionLeft(object sender, EventArgs e)
        {
            if (!Dispatcher.CheckAccess())
            {
                Dispatcher.BeginInvoke(new Action<object, EventArgs>(OnSessionLeft), sender, e);
                return;
            }

            _isInSession = false;
            _isVideoStarted = false;
            UpdateButtonStates();
            UpdateStatus("Left Zoom session");
            
            // Clear video displays
            SelfVideoImage.Source = null;
            RemoteVideoImage.Source = null;
        }

        private void OnSessionError(object sender, string errorMessage)
        {
            if (!Dispatcher.CheckAccess())
            {
                Dispatcher.BeginInvoke(new Action<object, string>(OnSessionError), sender, errorMessage);
                return;
            }

            UpdateStatus($"Session error: {errorMessage}");
            MessageBox.Show($"Zoom session error: {errorMessage}", "Session Error", 
                           MessageBoxButton.OK, MessageBoxImage.Warning);
        }

        // Video event handlers - WPF version using BitmapSource
        private void OnRemoteVideoReceived(object sender, VideoFrameEventArgs e)
        {
            if (!Dispatcher.CheckAccess())
            {
                Dispatcher.BeginInvoke(new Action<object, VideoFrameEventArgs>(OnRemoteVideoReceived), sender, e);
                return;
            }

            try
            {
                // Frame rate throttling - limit to ~30fps to reduce choppiness
                DateTime now = DateTime.Now;
                if (now - _lastRemoteVideoUpdate < _videoUpdateInterval)
                {
                    // Skip this frame to maintain smooth frame rate
                    return;
                }
                _lastRemoteVideoUpdate = now;

                if (e.Frame != null)
                {
                    // Display the received video frame in the remote video panel
                    RemoteVideoImage.Source = e.Frame;
                    UpdateStatus($"Received video from user: {e.UserId}");
                }
                else
                {
                    // Clear video when user stops video or leaves
                    RemoteVideoImage.Source = null;
                    UpdateStatus($"Video stopped for user: {e.UserId}");
                }
            }
            catch (Exception ex)
            {
                UpdateStatus($"Error displaying remote video: {ex.Message}");
            }
        }

        private void OnPreviewVideoReceived(object sender, VideoFrameEventArgs e)
        {
            if (!Dispatcher.CheckAccess())
            {
                Dispatcher.BeginInvoke(new Action<object, VideoFrameEventArgs>(OnPreviewVideoReceived), sender, e);
                return;
            }

            try
            {
                // Frame rate throttling - limit to ~30fps to reduce choppiness
                DateTime now = DateTime.Now;
                if (now - _lastPreviewVideoUpdate < _videoUpdateInterval)
                {
                    // Skip this frame to maintain smooth frame rate
                    return;
                }
                _lastPreviewVideoUpdate = now;

                if (e.Frame != null)
                {
                    // Display the preview video frame in the self video panel
                    SelfVideoImage.Source = e.Frame;
                    UpdateStatus("Self video preview updated");
                }
                else
                {
                    // Clear self video
                    SelfVideoImage.Source = null;
                    UpdateStatus("Self video preview stopped");
                }
            }
            catch (Exception ex)
            {
                UpdateStatus($"Error displaying self video: {ex.Message}");
            }
        }

        // Event handlers for buttons
        private void JoinButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                string sessionName = SessionNameTextBox.Text.Trim();
                string token = TokenTextBox.Text.Trim();
                string userName = UserNameTextBox.Text.Trim();
                string password = PasswordTextBox.Password.Trim(); // Password is optional

                if (string.IsNullOrEmpty(sessionName) || string.IsNullOrEmpty(token) || string.IsNullOrEmpty(userName))
                {
                    MessageBox.Show("Please fill in all required session details (Session Name, Token, and User Name).\n\nPassword is optional.", 
                                   "Missing Information", MessageBoxButton.OK, MessageBoxImage.Warning);
                    return;
                }

                // Don't block join on SDK initialization - try to initialize if needed
                if (_zoomSDK == null)
                {
                    UpdateStatus("Initializing SDK...");
                    InitializeZoomSDK();
                }

                if (_zoomSDK == null || !_zoomSDK.IsInitialized)
                {
                    UpdateStatus("SDK initialization failed - attempting join anyway");
                    // Don't return here - let the join attempt proceed
                }

                UpdateStatus("Joining session...");
                
                bool success = _zoomSDK.JoinSession(sessionName, token, userName, password);
                
                if (!success)
                {
                    UpdateStatus("Failed to join session - check your credentials");
                    MessageBox.Show("Failed to join session. Please check your session name, token, password, and network connection.", 
                                   "Join Failed", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }
            catch (Exception ex)
            {
                UpdateStatus("Error joining session");
                MessageBox.Show($"Error joining session: {ex.Message}", "Error", 
                               MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void LeaveButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (_zoomSDK == null || !_zoomSDK.IsInSession)
                {
                    UpdateStatus("Not in session");
                    return;
                }

                UpdateStatus("Leaving session...");
                _zoomSDK.LeaveSession();
            }
            catch (Exception ex)
            {
                UpdateStatus("Error leaving session");
                MessageBox.Show($"Error leaving session: {ex.Message}", "Error", 
                               MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void PreviewTokenButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                string token = TokenTextBox.Text.Trim();
                
                if (string.IsNullOrEmpty(token))
                {
                    MessageBox.Show("Please enter a JWT token first.", "No Token", 
                                   MessageBoxButton.OK, MessageBoxImage.Warning);
                    return;
                }

                // Decode JWT token
                var tokenInfo = DecodeJwtToken(token);
                
                // Show token information in a dialog
                ShowTokenPreviewDialog(tokenInfo);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error previewing token: {ex.Message}", "Token Preview Error", 
                               MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private TokenInfo DecodeJwtToken(string token)
        {
            try
            {
                // Split JWT token into parts
                var parts = token.Split('.');
                if (parts.Length != 3)
                {
                    throw new ArgumentException("Invalid JWT token format. Expected 3 parts separated by dots.");
                }

                // Decode the payload (second part)
                var payload = parts[1];
                
                // Add padding if necessary for Base64 decoding
                switch (payload.Length % 4)
                {
                    case 2: payload += "=="; break;
                    case 3: payload += "="; break;
                }

                // Replace URL-safe Base64 characters
                payload = payload.Replace('-', '+').Replace('_', '/');

                // Decode from Base64
                var jsonBytes = Convert.FromBase64String(payload);
                var jsonString = Encoding.UTF8.GetString(jsonBytes);

                // Parse JSON
                var tokenData = JsonConvert.DeserializeObject(jsonString);
                
                return new TokenInfo
                {
                    IsValid = true,
                    Payload = tokenData,
                    JsonString = jsonString,
                    RawToken = token,
                    Error = null
                };
            }
            catch (Exception ex)
            {
                return new TokenInfo
                {
                    IsValid = false,
                    Payload = null,
                    JsonString = null,
                    RawToken = token,
                    Error = ex.Message
                };
            }
        }

        private void ShowTokenPreviewDialog(TokenInfo tokenInfo)
        {
            var dialog = new Window
            {
                Title = "JWT Token Preview",
                Width = 600,
                Height = 500,
                WindowStartupLocation = WindowStartupLocation.CenterOwner,
                Owner = this,
                ResizeMode = ResizeMode.NoResize
            };

            var textBox = new TextBox
            {
                IsReadOnly = true,
                TextWrapping = TextWrapping.Wrap,
                VerticalScrollBarVisibility = ScrollBarVisibility.Auto,
                HorizontalScrollBarVisibility = ScrollBarVisibility.Auto,
                FontFamily = new System.Windows.Media.FontFamily("Consolas"),
                FontSize = 10,
                Margin = new Thickness(10)
            };

            var buttonPanel = new StackPanel
            {
                Orientation = Orientation.Horizontal,
                HorizontalAlignment = HorizontalAlignment.Right,
                Margin = new Thickness(10)
            };

            var okButton = new Button
            {
                Content = "OK",
                Width = 75,
                Height = 30,
                Margin = new Thickness(5)
            };

            okButton.Click += (s, e) => dialog.Close();

            if (tokenInfo.IsValid)
            {
                var sb = new StringBuilder();
                sb.AppendLine("=== JWT TOKEN PREVIEW ===");
                sb.AppendLine();
                sb.AppendLine("✅ Token is properly formatted");
                sb.AppendLine();
                sb.AppendLine("=== DECODED PAYLOAD ===");
                sb.AppendLine(FormatJson(tokenInfo.JsonString));
                sb.AppendLine();
                sb.AppendLine("=== SESSION PARAMETERS ===");
                sb.AppendLine($"Session Name: {SessionNameTextBox.Text}");
                sb.AppendLine($"User Name: {UserNameTextBox.Text}");
                sb.AppendLine($"Password: {(PasswordTextBox.Password.Trim().Length > 0 ? "****** (set)" : "(not set)")}");
                sb.AppendLine();
                sb.AppendLine("=== RAW TOKEN (First 50 chars) ===");
                sb.AppendLine(tokenInfo.RawToken.Length > 50 ? 
                             tokenInfo.RawToken.Substring(0, 50) + "..." : 
                             tokenInfo.RawToken);

                textBox.Text = sb.ToString();
                textBox.Background = System.Windows.Media.Brushes.White;
            }
            else
            {
                var sb = new StringBuilder();
                sb.AppendLine("=== JWT TOKEN PREVIEW ===");
                sb.AppendLine();
                sb.AppendLine("❌ Token is INVALID");
                sb.AppendLine();
                sb.AppendLine("=== ERROR ===");
                sb.AppendLine(tokenInfo.Error);
                sb.AppendLine();
                sb.AppendLine("=== COMMON ISSUES ===");
                sb.AppendLine("• Token is still set to placeholder 'your_jwt_token_here'");
                sb.AppendLine("• Token is expired (check 'exp' field)");
                sb.AppendLine("• Token format is incorrect (should have 3 parts separated by dots)");
                sb.AppendLine("• Token was not generated with correct API key/secret");
                sb.AppendLine();
                sb.AppendLine("=== RAW TOKEN ===");
                sb.AppendLine(tokenInfo.RawToken);

                textBox.Text = sb.ToString();
                textBox.Background = System.Windows.Media.Brushes.LightPink;
            }

            buttonPanel.Children.Add(okButton);

            var mainPanel = new DockPanel();
            DockPanel.SetDock(buttonPanel, Dock.Bottom);
            mainPanel.Children.Add(buttonPanel);
            mainPanel.Children.Add(textBox);

            dialog.Content = mainPanel;
            dialog.ShowDialog();
        }

        private string FormatJson(string json)
        {
            try
            {
                var parsed = JsonConvert.DeserializeObject(json);
                return JsonConvert.SerializeObject(parsed, Formatting.Indented);
            }
            catch
            {
                return json; // Return original if formatting fails
            }
        }

        private void MuteMicButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (_zoomSDK == null || !_zoomSDK.IsInSession)
                {
                    UpdateStatus("Not in session");
                    return;
                }

                bool newMuteState = !_isMicMuted;
                if (_zoomSDK.MuteAudio(newMuteState))
                {
                    _isMicMuted = newMuteState;
                    UpdateButtonStates();
                    UpdateStatus(_isMicMuted ? "Microphone muted" : "Microphone unmuted");
                }
                else
                {
                    UpdateStatus("Failed to change microphone state");
                }
            }
            catch (Exception ex)
            {
                UpdateStatus("Error controlling microphone");
                MessageBox.Show($"Error controlling microphone: {ex.Message}", "Error", 
                               MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void MuteSpeakerButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (_zoomSDK == null || !_zoomSDK.IsInSession)
                {
                    UpdateStatus("Not in session");
                    return;
                }

                // Note: Speaker mute functionality would need to be implemented in the SDK wrapper
                _isSpeakerMuted = !_isSpeakerMuted;
                UpdateButtonStates();
                UpdateStatus(_isSpeakerMuted ? "Speaker muted" : "Speaker unmuted");
            }
            catch (Exception ex)
            {
                UpdateStatus("Error controlling speaker");
                MessageBox.Show($"Error controlling speaker: {ex.Message}", "Error", 
                               MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void StartVideoButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (_zoomSDK == null || !_zoomSDK.IsInSession)
                {
                    UpdateStatus("Not in session");
                    return;
                }

                if (_zoomSDK.StartVideo())
                {
                    _isVideoStarted = true;
                    UpdateButtonStates();
                    UpdateStatus("Video started - live preview should appear");
                }
                else
                {
                    UpdateStatus("Failed to start video");
                }
            }
            catch (Exception ex)
            {
                UpdateStatus("Error starting video");
                MessageBox.Show($"Error starting video: {ex.Message}", "Error", 
                               MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void StopVideoButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (_zoomSDK == null || !_zoomSDK.IsInSession)
                {
                    UpdateStatus("Not in session");
                    return;
                }

                if (_zoomSDK.StopVideo())
                {
                    _isVideoStarted = false;
                    UpdateButtonStates();
                    UpdateStatus("Video stopped");
                    SelfVideoImage.Source = null;
                }
                else
                {
                    UpdateStatus("Failed to stop video");
                }
            }
            catch (Exception ex)
            {
                UpdateStatus("Error stopping video");
                MessageBox.Show($"Error stopping video: {ex.Message}", "Error", 
                               MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        protected override void OnClosed(EventArgs e)
        {
            try
            {
                if (_zoomSDK != null)
                {
                    if (_zoomSDK.IsInSession)
                    {
                        _zoomSDK.LeaveSession();
                    }
                    _zoomSDK.Cleanup();
                }
            }
            catch (Exception ex)
            {
                // Log error but don't prevent closing
                System.Diagnostics.Debug.WriteLine($"Error during cleanup: {ex.Message}");
            }
            base.OnClosed(e);
        }
    }

    // Configuration class
    public class SessionConfig
    {
        public string SessionName { get; set; }
        public string Token { get; set; }
        public string UserName { get; set; }
        public string Password { get; set; }
    }

    // Token information class
    public class TokenInfo
    {
        public bool IsValid { get; set; }
        public object Payload { get; set; }
        public string JsonString { get; set; }
        public string RawToken { get; set; }
        public string Error { get; set; }
    }
}
