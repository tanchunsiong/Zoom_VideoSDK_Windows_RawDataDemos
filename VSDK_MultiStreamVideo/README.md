# Zoom Video SDK Windows MultiStream

This current sample app is based on version.txt
A Windows C++ Application demonstrate Zoom Video SDK's feature

# Install vcpkg for adding dependency libs.
You might need to use Powershell (as administrator) or Windows Terminal to execute the sh script files
```
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install ./vcpkg integrate install --vcpkg-root c:\vcpkg
```

# Add dependency libs
opencv might not be necessary if you are not going to do video/image processing before saving.
opencv might not be necessary if you are just saving raw audio to file..
opencv will take a while (10 mins) to complete

```
./vcpkg install jsoncpp
./vcpkg install opencv 
```


This project is in the VSDK_xxxxxxxx folder

You will need to create a config.json with the below parameters

```
{
  "sdk_jwt": "xxxxxx.yyyyyy.zzzzzzz",
  "sessionName": "yoursessionname",
  "password": "12345678",
  "jwt_webservice_url": "https://yourdomain.com/videoSDKAuthJWTTokenGenerator",
  "useJWTTokenFromWebService": "false"
}

```

Within `main_frame.cpp` you will need to enter your credentials in the `JoinSession` method

```
void CMainFrame::JoinSession()

		std::wstring session_name = L"webchun6871";
		std::wstring sUserName = L"vsdk_skeletondemo";
		std::wstring session_password_ = L"12345678";
		std::wstring token = L"xxxxx.yyyyyy.zzzzz";
```


## Download the official Video SDK for Windows, and add sdk files into a folder name `SDK`

The folder structure should look something like this

VSDK_SkeletonDemo
- SDK    <- #add sdk files into this directory
	-x86
	-x64
- x64
	-Debug
	-Release


## Open and Run Project

Open "VSDK_RawDataDemos.sln" file from Visual Studio 2022.

Hit F5 or click from menu "Debug" -> "Start Debugging" in x86 or x64 to launch the application.




  # Error

  what if i would like to use x64 environment?

  add this to your environment variable before installing openCV from vcpkg

  VCPKG_DEFAULT_TRIPLET = x64-windows

  and reinstall

  ```
  ./vcpkg install jsoncpp
  ./vcpkg install opencv
  ```

  There are some errors about opencv being unable to find certain libraries?

  They should not affect the basic functionality of this sample, but you can do a list of additional opencv libraries to install
  ```
  ./cvpkg list opencv
  ```

  to install addition libraries, do something like this. This might take around 1 hour, depending on the number of libraries you install
  ```
  ./vcpkg install opencv[contrib,ffmpeg,nonfree,opengl,openmp,world]
  ```
## Getting Started

The main method, or main entry point of this application is at `main_frame.cpp`

From a high level point of view it will showcase some of the functionalities below.
For ease of development understanding, I've used the below variables as "control keywords" in `main_frame.cpp` Search for these "control keywords" (bool variables) to understand the flow

Example searching for "control keyword" `getRawAudio` will highlight bookmark on 
 - header files which needs to be included, 
 - the session context options needed before joining a session, 
 - the methods to call after successfully joining a session and 
 - the callbacks to handle the raw audio frame


# Upgrading Guide

You will need to download the latest Video SDK Windows for c++ from marketplace.zoom.us

Replace the files in the folder `SDK` with those found in the SDK files downloaded from marketplace.zoom.us

You will need to ensure any missing abstract classes are implemented etc... before you can compile and upgrade to a newer SDK version.
Usually newer version of SDK will introduce new interfaces / abstract classes.

