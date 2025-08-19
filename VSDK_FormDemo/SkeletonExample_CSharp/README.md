# Zoom VideoSDK C# Windows Forms Wrapper

This project provides a C# Windows Forms wrapper for the Zoom VideoSDK, allowing you to integrate Zoom video conferencing capabilities into Windows desktop applications.

## Project Structure

- **ZoomVideoSDK.WinForms** - C# Windows Forms application with the main UI
- **ZoomVideoSDK.Wrapper** - C++/CLI managed wrapper that bridges the native Zoom VideoSDK with .NET

## Features

- **Session Management**: Join and leave Zoom sessions
- **Audio Controls**: Mute/unmute microphone, adjust volume
- **Video Controls**: Start/stop camera, switch cameras
- **Device Management**: Enumerate and select audio/video devices
- **User Management**: View session participants
- **Chat**: Send and receive chat messages
- **Screen Sharing**: Share your screen with other participants

## Building the Project

### Prerequisites

- Visual Studio 2022 with C++ and .NET Framework 4.8 support
- Zoom VideoSDK Windows libraries (included in this repository)

### Build Steps

1. Open the solution file:
   ```
   ZoomVideoSDK.CSharp.sln
   ```

2. Build using MSBuild:
   ```cmd
   "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" ZoomVideoSDK.CSharp.sln /p:Configuration=Debug /p:Platform="Any CPU"
   ```

3. The application will be built to:
   ```
   ZoomVideoSDK.WinForms\bin\Debug\net48\ZoomVideoSDK.WinForms.exe
   ```

## Running the Application

1. Ensure all required DLLs are in the output directory:
   - `videosdk.dll`
   - `SDL2.dll`
   - `turbojpeg.dll`
   - `libcrypto-3-zm.dll`
   - `libssl-3-zm.dll`
   - `ZoomVideoSDK.Wrapper.dll`

2. Run the executable:
   ```cmd
   ZoomVideoSDK.WinForms\bin\Debug\net48\ZoomVideoSDK.WinForms.exe
   ```

## Configuration

The application loads configuration from `config.json`:

```json
{
  "app_type": 1,
  "jwt_token": "your_jwt_token_here",
  "session_name": "test_session",
  "user_name": "test_user",
  "session_password": "",
  "log_level": 0,
  "log_dir": "./logs",
  "enable_log_by_default": true,
  "enable_sdk_log_ui": false
}
```

## Architecture

### C++/CLI Wrapper (ZoomVideoSDK.Wrapper)

The C++/CLI wrapper provides a managed interface to the native Zoom VideoSDK:

- **ZoomSDKManager**: Main managed class that wraps SDK functionality
- **Event Handling**: Converts native SDK callbacks to .NET events
- **Memory Management**: Handles marshaling between native and managed code

### C# Windows Forms Application (ZoomVideoSDK.WinForms)

The Windows Forms application provides the user interface:

- **MainForm**: Main application window with all controls
- **ZoomSDKInterop**: Helper class for SDK integration
- **Event Handlers**: Responds to SDK events and updates UI

## Current Implementation

The current implementation includes:

- ✅ Complete Windows Forms UI with all controls
- ✅ C++/CLI wrapper with proper event handling
- ✅ Session join/leave functionality (demo mode)
- ✅ Audio/video control interfaces
- ✅ Device enumeration interfaces
- ✅ Chat functionality interface
- ✅ Screen sharing interface

## Next Steps

To enable full Zoom SDK integration:

1. **Replace Demo Mode**: Update the C++/CLI wrapper to call actual Zoom SDK functions instead of demo simulations
2. **Implement Real Callbacks**: Connect native SDK callbacks to managed events
3. **Add Video Rendering**: Implement video frame rendering for local and remote participants
4. **Error Handling**: Add comprehensive error handling and logging
5. **Testing**: Test with real Zoom sessions

## Known Issues

- Architecture mismatch warning (MSIL vs AMD64) - this is expected and doesn't affect functionality
- Currently runs in demo mode - SDK integration needs to be completed for real functionality

## Dependencies

- .NET Framework 4.8
- Zoom VideoSDK Windows libraries
- Visual C++ Redistributable
- Windows 10/11

## License

This project is provided as an example implementation. Please ensure you comply with Zoom's SDK licensing terms.
