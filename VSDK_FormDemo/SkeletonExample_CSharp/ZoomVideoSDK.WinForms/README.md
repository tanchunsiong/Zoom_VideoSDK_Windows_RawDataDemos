# Zoom VideoSDK Windows Forms Application

## Setup Instructions

### 1. Configuration Files Location
- **config.json**: Located in the application output directory (`bin/Debug/net48/config.json`)
- **DLLs**: All required Zoom VideoSDK DLLs are included in the `libs/` folder and automatically copied during build

### 2. JWT Token Setup
The application reads initial values from `config.json` but **uses the values from the UI textboxes** when joining a session.

#### To join a session:
1. **Get your JWT token** from your Zoom VideoSDK app
2. **Enter the JWT token** in the "Token" textbox in the application UI
3. **Enter Session Name** and **User Name** in their respective textboxes
4. **[OPTIONAL] Click "PREVIEW TOKEN"** to verify your JWT token before joining
5. **Click "JOIN SESSION"**

#### Error 2003 10000403 - Authentication Failed
This error occurs when:
- JWT token is invalid or expired
- JWT token is still set to "your_jwt_token_here" placeholder
- Network connectivity issues

### 3. How to Generate JWT Token
1. Create a Zoom VideoSDK app at [Zoom Marketplace](https://marketplace.zoom.us/)
2. Get your API Key and API Secret
3. Generate JWT token with payload:
```json
{
  "iss": "YOUR_API_KEY",
  "exp": 1735689600,
  "alg": "HS256",
  "aud": "zoom",
  "appKey": "YOUR_API_KEY",
  "tokenExp": 1735689600,
  "sessionName": "TestSession",
  "userIdentity": "TestUser",
  "sessionKey": "session123",
  "geoRegions": "US"
}
```

### 4. File Structure
```
ZoomVideoSDK.WinForms/
├── libs/                    # Zoom VideoSDK DLLs (included in project)
│   ├── videosdk.dll
│   ├── libcrypto-3-zm.dll
│   ├── libssl-3-zm.dll
│   ├── turbojpeg.dll
│   ├── SDL2.dll
│   └── cares.dll
├── bin/Debug/net48/         # Output directory
│   ├── config.json          # Configuration file (copied automatically)
│   ├── ZoomVideoSDK.WinForms.exe
│   ├── ZoomVideoSDK.Wrapper.dll
│   └── [All Zoom DLLs]
└── [Source files]
```

### 5. JWT Token Preview Feature
The application includes a **"PREVIEW TOKEN"** button that allows you to:
- **Decode and validate** your JWT token before joining a session
- **View token payload** in formatted JSON
- **Check expiration** and other token details
- **Identify common issues** with invalid tokens

**How to use:**
1. Enter your JWT token in the Token textbox
2. Click "PREVIEW TOKEN" 
3. Review the decoded token information
4. Fix any issues before attempting to join

### 6. Application Features
- ✅ Real Zoom VideoSDK integration (no demo mode)
- ✅ Session join/leave functionality
- ✅ Audio controls (mute/unmute microphone)
- ✅ Video controls (start/stop camera)
- ✅ Device selection (microphone, speaker, camera)
- ✅ Status updates and error handling
- ✅ UI reads from textboxes for session parameters
- ✅ **JWT token preview and validation**

### 6. Troubleshooting
- **DLL not found errors**: All DLLs are now included in the project and copied automatically
- **Authentication errors**: Enter a valid JWT token in the UI textbox
- **Build errors**: Ensure Visual Studio Build Tools with C++ support are installed
