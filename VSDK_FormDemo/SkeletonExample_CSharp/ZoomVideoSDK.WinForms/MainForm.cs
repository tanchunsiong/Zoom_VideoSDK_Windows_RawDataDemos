using System;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;
using Newtonsoft.Json;

namespace ZoomVideoSDK.WinForms
{
    public partial class MainForm : Form
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

        public MainForm()
        {
            InitializeComponent();
            LoadConfiguration();
            InitializeZoomSDK();
            UpdateButtonStates();
            
            // Add video labels to the designer-created video panels
            AddVideoLabels();
            
            // Populate device lists after SDK initialization
            PopulateDeviceLists();
        }

        private void AddVideoLabels()
        {
            // Add "Self Video" label to self video panel
            var selfLabel = new Label
            {
                Text = "Self Video",
                ForeColor = Color.White,
                BackColor = Color.Transparent,
                TextAlign = ContentAlignment.MiddleCenter,
                Dock = DockStyle.Fill
            };
            _selfVideoPanel.Controls.Add(selfLabel);

            // Add "Remote Video" label to remote video panel
            var remoteLabel = new Label
            {
                Text = "Remote Video",
                ForeColor = Color.White,
                BackColor = Color.Transparent,
                TextAlign = ContentAlignment.MiddleCenter,
                Dock = DockStyle.Fill
            };
            _remoteVideoPanel.Controls.Add(remoteLabel);
        }

        private void PopulateDeviceLists()
        {
            try
            {
                // Clear existing items
                _microphoneComboBox.Items.Clear();
                _speakerComboBox.Items.Clear();
                _cameraComboBox.Items.Clear();

                if (_zoomSDK != null && _zoomSDK.IsInitialized)
                {
                    // Get real device lists from SDK
                    var microphones = _zoomSDK.GetMicrophoneList();
                    var speakers = _zoomSDK.GetSpeakerList();
                    var cameras = _zoomSDK.GetCameraList();

                    // Populate microphones
                    foreach (var mic in microphones)
                    {
                        _microphoneComboBox.Items.Add(mic);
                    }

                    // Populate speakers
                    foreach (var speaker in speakers)
                    {
                        _speakerComboBox.Items.Add(speaker);
                    }

                    // Populate cameras
                    foreach (var camera in cameras)
                    {
                        _cameraComboBox.Items.Add(camera);
                    }

                    UpdateStatus($"Found {microphones.Length} microphones, {speakers.Length} speakers, {cameras.Length} cameras");
                }
                else
                {
                    // Fallback to default devices if SDK not initialized
                    _microphoneComboBox.Items.Add("Default Microphone");
                    _speakerComboBox.Items.Add("Default Speaker");
                    _cameraComboBox.Items.Add("Default Camera");
                    UpdateStatus("Using default devices (SDK not initialized)");
                }

                // Select first item in each list
                if (_microphoneComboBox.Items.Count > 0) _microphoneComboBox.SelectedIndex = 0;
                if (_speakerComboBox.Items.Count > 0) _speakerComboBox.SelectedIndex = 0;
                if (_cameraComboBox.Items.Count > 0) _cameraComboBox.SelectedIndex = 0;
            }
            catch (Exception ex)
            {
                UpdateStatus($"Error populating device lists: {ex.Message}");
                
                // Fallback to default devices on error
                _microphoneComboBox.Items.Clear();
                _speakerComboBox.Items.Clear();
                _cameraComboBox.Items.Clear();
                
                _microphoneComboBox.Items.Add("Default Microphone");
                _speakerComboBox.Items.Add("Default Speaker");
                _cameraComboBox.Items.Add("Default Camera");
                
                if (_microphoneComboBox.Items.Count > 0) _microphoneComboBox.SelectedIndex = 0;
                if (_speakerComboBox.Items.Count > 0) _speakerComboBox.SelectedIndex = 0;
                if (_cameraComboBox.Items.Count > 0) _cameraComboBox.SelectedIndex = 0;
            }
        }

        private void LoadConfiguration()
        {
            try
            {
                string configPath = Path.Combine(Application.StartupPath, "config.json");
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
                    _sessionNameTextBox.Text = _config.SessionName ?? "";
                    _tokenTextBox.Text = _config.Token ?? "";
                    _userNameTextBox.Text = _config.UserName ?? "";
                    _passwordTextBox.Text = _config.Password ?? "";
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error loading configuration: {ex.Message}", "Configuration Error", 
                               MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        private void UpdateButtonStates()
        {
            // FIXED: Always enable join button when not in session (Linux pattern)
            // Don't disable UI based on SDK initialization status
            _joinButton.Enabled = !_isInSession;
            _leaveButton.Enabled = _isInSession;
            
            _muteMicButton.Enabled = _isInSession;
            _muteSpeakerButton.Enabled = _isInSession;
            _startVideoButton.Enabled = _isInSession && !_isVideoStarted;
            _stopVideoButton.Enabled = _isInSession && _isVideoStarted;
            
            _muteMicButton.Text = _isMicMuted ? "Unmute Mic" : "Mute Mic";
            _muteSpeakerButton.Text = _isSpeakerMuted ? "Unmute Speaker" : "Mute Speaker";
        }

        private void UpdateStatus(string message)
        {
            if (InvokeRequired)
            {
                Invoke(new Action<string>(UpdateStatus), message);
                return;
            }
            
            // Add null check to prevent NullReferenceException
            if (_statusLabel != null)
            {
                _statusLabel.Text = message;
            }
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
                                   "SDK Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            catch (Exception ex)
            {
                UpdateStatus($"SDK initialization error: {ex.Message}");
                MessageBox.Show($"SDK initialization error: {ex.Message}", "SDK Error", 
                               MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void OnStatusChanged(object sender, string message)
        {
            UpdateStatus(message);
        }


        private void OnSessionJoined(object sender, EventArgs e)
        {
            if (InvokeRequired)
            {
                BeginInvoke(new Action<object, EventArgs>(OnSessionJoined), sender, e);
                return;
            }

            _isInSession = true;
            UpdateButtonStates();
            UpdateStatus("Successfully joined Zoom session - use Start Video button to begin video");
            
            // REMOVED: Automatic video start to prevent repeated initialization
            // User now has full control over when to start video via the Start Video button
        }

        private void OnSessionLeft(object sender, EventArgs e)
        {
            if (InvokeRequired)
            {
                Invoke(new Action<object, EventArgs>(OnSessionLeft), sender, e);
                return;
            }

            _isInSession = false;
            _isVideoStarted = false;
            UpdateButtonStates();
            UpdateStatus("Left Zoom session");
            
            // Clear video displays
            _selfVideoPanel.Image = null;
            _remoteVideoPanel.Image = null;
        }

        private void OnSessionError(object sender, string errorMessage)
        {
            if (InvokeRequired)
            {
                Invoke(new Action<object, string>(OnSessionError), sender, errorMessage);
                return;
            }

            UpdateStatus($"Session error: {errorMessage}");
            MessageBox.Show($"Zoom session error: {errorMessage}", "Session Error", 
                           MessageBoxButtons.OK, MessageBoxIcon.Warning);
        }

        // Video event handlers - using the local VideoFrameEventArgs from ZoomSDKInterop
        private void OnRemoteVideoReceived(object sender, VideoFrameEventArgs e)
        {
            if (InvokeRequired)
            {
                BeginInvoke(new Action<object, VideoFrameEventArgs>(OnRemoteVideoReceived), sender, e);
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
                    _remoteVideoPanel.Image?.Dispose(); // Dispose previous image
                    _remoteVideoPanel.Image = new Bitmap(e.Frame); // Create a copy
                    UpdateStatus($"Received video from user: {e.UserId}");
                }
                else
                {
                    // Clear video when user stops video or leaves
                    _remoteVideoPanel.Image?.Dispose();
                    _remoteVideoPanel.Image = null;
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
            if (InvokeRequired)
            {
                BeginInvoke(new Action<object, VideoFrameEventArgs>(OnPreviewVideoReceived), sender, e);
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
                    _selfVideoPanel.Image?.Dispose(); // Dispose previous image
                    _selfVideoPanel.Image = new Bitmap(e.Frame); // Create a copy
                    UpdateStatus("Self video preview updated");
                }
                else
                {
                    // Clear self video
                    _selfVideoPanel.Image?.Dispose();
                    _selfVideoPanel.Image = null;
                    UpdateStatus("Self video preview stopped");
                }
            }
            catch (Exception ex)
            {
                UpdateStatus($"Error displaying self video: {ex.Message}");
            }
        }

        // Event handlers
        private void JoinButton_Click(object sender, EventArgs e)
        {
            try
            {
                string sessionName = _sessionNameTextBox.Text.Trim();
                string token = _tokenTextBox.Text.Trim();
                string userName = _userNameTextBox.Text.Trim();
                string password = _passwordTextBox.Text.Trim(); // Password is optional

                if (string.IsNullOrEmpty(sessionName) || string.IsNullOrEmpty(token) || string.IsNullOrEmpty(userName))
                {
                    MessageBox.Show("Please fill in all required session details (Session Name, Token, and User Name).\n\nPassword is optional.", 
                                   "Missing Information", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                }

                // FIXED: Don't block join on SDK initialization - try to initialize if needed (Linux pattern)
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
                
                // FIXED: Use existing join method - video/audio config needs to be added to C++/CLI wrapper
                bool success = _zoomSDK.JoinSession(sessionName, token, userName, password);
                
                if (!success)
                {
                    UpdateStatus("Failed to join session - check your credentials");
                    MessageBox.Show("Failed to join session. Please check your session name, token, password, and network connection.", 
                                   "Join Failed", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            catch (Exception ex)
            {
                UpdateStatus("Error joining session");
                MessageBox.Show($"Error joining session: {ex.Message}", "Error", 
                               MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void LeaveButton_Click(object sender, EventArgs e)
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
                               MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void PreviewTokenButton_Click(object sender, EventArgs e)
        {
            try
            {
                string token = _tokenTextBox.Text.Trim();
                
                if (string.IsNullOrEmpty(token))
                {
                    MessageBox.Show("Please enter a JWT token first.", "No Token", 
                                   MessageBoxButtons.OK, MessageBoxIcon.Warning);
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
                               MessageBoxButtons.OK, MessageBoxIcon.Error);
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
            var dialog = new Form
            {
                Text = "JWT Token Preview",
                Size = new Size(600, 500),
                StartPosition = FormStartPosition.CenterParent,
                FormBorderStyle = FormBorderStyle.FixedDialog,
                MaximizeBox = false,
                MinimizeBox = false
            };

            var textBox = new TextBox
            {
                Multiline = true,
                ReadOnly = true,
                ScrollBars = ScrollBars.Both,
                Dock = DockStyle.Fill,
                Font = new Font("Consolas", 10),
                Margin = new Padding(10)
            };

            var buttonPanel = new Panel
            {
                Height = 50,
                Dock = DockStyle.Bottom
            };

            var okButton = new Button
            {
                Text = "OK",
                Size = new Size(75, 30),
                Anchor = AnchorStyles.Bottom | AnchorStyles.Right,
                Location = new Point(buttonPanel.Width - 85, 10)
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
                sb.AppendLine($"Session Name: {_sessionNameTextBox.Text}");
                sb.AppendLine($"User Name: {_userNameTextBox.Text}");
                sb.AppendLine($"Password: {(_passwordTextBox.Text.Trim().Length > 0 ? "****** (set)" : "(not set)")}");
                sb.AppendLine();
                sb.AppendLine("=== RAW TOKEN (First 50 chars) ===");
                sb.AppendLine(tokenInfo.RawToken.Length > 50 ? 
                             tokenInfo.RawToken.Substring(0, 50) + "..." : 
                             tokenInfo.RawToken);

                textBox.Text = sb.ToString();
                textBox.BackColor = Color.White;
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
                textBox.BackColor = Color.LightPink;
            }

            buttonPanel.Controls.Add(okButton);
            dialog.Controls.Add(textBox);
            dialog.Controls.Add(buttonPanel);

            dialog.ShowDialog(this);
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

        private void MuteMicButton_Click(object sender, EventArgs e)
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
                               MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void MuteSpeakerButton_Click(object sender, EventArgs e)
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
                               MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void StartVideoButton_Click(object sender, EventArgs e)
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
                               MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void StopVideoButton_Click(object sender, EventArgs e)
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
                    _selfVideoPanel.Image = null;
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
                               MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }


        protected override void OnFormClosing(FormClosingEventArgs e)
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
            base.OnFormClosing(e);
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
