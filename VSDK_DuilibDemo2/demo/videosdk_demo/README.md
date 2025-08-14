# Zoom Video SDK Demo - Enhanced Single Window UI

This is an enhanced version of the Zoom Video SDK Windows demo application that provides a comprehensive single-window interface for video conferencing functionality.

## Overview

This demo application showcases the Zoom Video SDK capabilities through a modern, user-friendly single-window interface that consolidates all essential video conferencing features without requiring external configuration files.

## Features

### 🎯 **Single Window Interface**
- **1000x750 window** with professional dark theme
- All functionality consolidated into one comprehensive UI
- No need for multiple windows or external configuration files

### 🔧 **Session Configuration**
- **Session Name** input field
- **JWT Token** input field (required for authentication)
- **User Name** input field (defaults to "VideoSDK_User")
- **Password** input field (optional)
- Real-time validation with error feedback

### 📹 **Video Controls**
- **Local video preview** area with black background
- **Start/Stop Video** button with dynamic text updates
- **Camera selection** dropdown for multiple camera support
- Integration with Zoom Video SDK video helpers

### 🎵 **Audio Controls**
- **Mute/Unmute Audio** button with dynamic text updates
- **Microphone volume** slider (0-100 range)
- **Speaker volume** slider (0-100 range)
- Integration with Zoom Video SDK audio helpers

### 👥 **Session Information**
- **Live participant list** with names and status
- **Session duration timer** (HH:MM:SS format)
- **Connection status** indicator with color coding
- **Real-time updates** based on SDK events

### 💬 **Chat System**
- **Chat message display** with timestamps [HH:MM:SS]
- **Chat input field** with Send button
- **Enter key support** for quick messaging
- **Real-time message updates** through SDK callbacks

### 🎨 **Professional UI Design**
- Dark theme with proper contrast (#3B3C3C background)
- Organized sections using GroupBox containers
- Color-coded status indicators (green=connected, red=error)
- Professional button styling with hover effects
- Proper font hierarchy and spacing

## Prerequisites

### Required Software
- **Visual Studio 2019/2022** with C++ development tools
- **Windows 10/11** operating system
- **Zoom Video SDK for Windows** (place SDK files in appropriate directories)

### Required Dependencies
- **DuiLib UI Framework** (included in project)
- **Zoom Video SDK** libraries and headers
- **vcpkg** for dependency management (optional)

## Setup Instructions

### 1. Download Zoom Video SDK
1. Download the official **Zoom Video SDK for Windows** from [marketplace.zoom.us](https://marketplace.zoom.us)
2. Extract the SDK files
3. Place the SDK files in the project's SDK directory structure

### 2. Build the Project
1. Open `videosdk_demo.sln` in Visual Studio
2. Select your target platform (x86 or x64)
3. Choose Debug or Release configuration
4. Build the solution (F7 or Build → Build Solution)

### 3. Run the Application
1. Set `videosdk_demo` as the startup project
2. Press F5 or click "Start Debugging"
3. The enhanced single-window UI will launch

## Usage Guide

### Getting Started
1. **Launch the application** - The enhanced UI will appear by default
2. **Configure session parameters**:
   - Enter your **Session Name**
   - Enter your **JWT Token** (obtain from Zoom Video SDK)
   - Enter your **User Name** (or use default)
   - Enter **Password** if required (optional)

### Joining a Session
1. Fill in all required fields (Session Name, JWT Token, User Name)
2. Click **"Join Session"** button
3. Wait for connection status to show "Connected"
4. The main video conferencing interface will be active

### During a Session
- **Video Control**: Click "Start Video" to enable your camera
- **Audio Control**: Click "Unmute Audio" to enable your microphone
- **Volume Control**: Use sliders to adjust microphone and speaker volume
- **Camera Selection**: Use dropdown to switch between available cameras
- **Chat**: Type messages and click "Send" or press Enter
- **Monitor**: View participants list and session duration

### Leaving a Session
1. Click **"Leave Session"** button
2. Wait for disconnection confirmation
3. The UI will return to the configuration screen

## Technical Architecture

### Key Components
- **`main_frame.cpp/h`**: Core application framework and SDK integration
- **`join_session_wnd_ui.cpp/h`**: Enhanced single-window UI implementation
- **`videosdk_demo_mgr.cpp/h`**: SDK management and wrapper functions
- **`skin/join_session_wnd.xml`**: UI layout definition with enhanced controls

### SDK Integration
- Uses **ZoomVideoSDKMgr::GetInst()** for all SDK operations
- Implements **IZoomVideoSDKDelegate** for event callbacks
- Proper renderer initialization before session join
- Real-time UI updates based on SDK events

### UI Framework
- Built on **DuiLib** for modern Windows UI
- XML-based layout definition
- Event-driven architecture
- Professional styling with existing resources

## Configuration

### No External Configuration Required
Unlike the original demo, this enhanced version **does not require config.json**. All configuration is done through the UI:

- **Session Name**: Enter directly in the UI
- **JWT Token**: Enter directly in the UI  
- **User Name**: Enter directly in the UI
- **Password**: Enter directly in the UI (optional)

### JWT Token Generation
You need to generate a JWT token for authentication:
1. Visit the Zoom Video SDK documentation
2. Use your SDK credentials to generate a JWT token
3. Enter the token in the JWT Token field

## Troubleshooting

### Common Issues

**"Please fill in all required fields"**
- Ensure Session Name, JWT Token, and User Name are not empty

**"Missing UI controls"**
- The enhanced UI controls were not found - check XML layout

**Connection fails**
- Verify JWT token is valid and not expired
- Check internet connection
- Ensure Zoom Video SDK is properly installed

**Video/Audio not working**
- Check camera and microphone permissions
- Verify devices are not being used by other applications
- Check device selection in dropdown menus

### Debug Information
The application provides real-time status updates:
- **Status bar** shows current connection state
- **Connection status** updates during join/leave operations
- **Error messages** appear for validation failures

## Development Notes

### Recent Enhancements
- **Single Window UI**: Consolidated all functionality into one window
- **No Config File**: Removed dependency on external config.json
- **Enhanced Controls**: Added video/audio toggles, volume controls, chat
- **Real-time Updates**: Live status updates and session monitoring
- **Professional Design**: Modern dark theme with proper UX

### Code Structure
- **Event-driven architecture** with comprehensive UI event handling
- **SDK callback integration** for real-time updates
- **Type-safe string handling** between DuiLib and SDK
- **Memory management** with proper initialization and cleanup
- **Error handling** with user-friendly feedback

## Version Information

- **Base Project**: VSDK_DuilibDemo2
- **UI Framework**: DuiLib
- **Target Platform**: Windows 10/11
- **Architecture**: x86/x64
- **SDK Version**: Compatible with latest Zoom Video SDK for Windows

## Support

For issues related to:
- **Zoom Video SDK**: Refer to official Zoom Video SDK documentation
- **DuiLib Framework**: Check DuiLib documentation and examples
- **This Demo**: Review the source code and comments for implementation details

---

**Note**: This enhanced demo provides a complete single-window video conferencing experience with all essential features integrated into one professional interface.
