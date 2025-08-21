/**
 * @file zoom_video_sdk_def.h
 * @brief Zoom Video SDK Common Definition File.
 */

#ifndef _ZOOM_VIDEO_SDK_DEF_H_
#define _ZOOM_VIDEO_SDK_DEF_H_
#include <iostream>
#include "zoom_video_sdk_platform.h"
#include "zoom_sdk_raw_data_def.h"

#define ZOOM_VIDEO_SDK_NAMESPACE ZOOMVIDEOSDK
#define BEGIN_ZOOM_VIDEO_SDK_NAMESPACE namespace ZOOM_VIDEO_SDK_NAMESPACE {
#define END_ZOOM_VIDEO_SDK_NAMESPACE };
#define USING_ZOOM_VIDEO_SDK_NAMESPACE using namespace ZOOM_VIDEO_SDK_NAMESPACE;

BEGIN_ZOOM_VIDEO_SDK_NAMESPACE
/**
 * @brief Enumeration of common Zoom Video SDK errors.
 */
typedef enum
{ 
	/** Success. */
	ZoomVideoSDKErrors_Success = 0,
	/** Incorrect usage of the feature. */
	ZoomVideoSDKErrors_Wrong_Usage,
	/** Internal SDK error. */
	ZoomVideoSDKErrors_Internal_Error,
	/** SDK is not initialized before usage. */
	ZoomVideoSDKErrors_Uninitialize,
	/** No memory is allocated or memory allocation failed. */
	ZoomVideoSDKErrors_Memory_Error,
	/** Failed to load module. */
	ZoomVideoSDKErrors_Load_Module_Error,
	/** Failed to unload module. */
	ZoomVideoSDKErrors_UnLoad_Module_Error,
	/** Invalid parameter passed to a function. */
	ZoomVideoSDKErrors_Invalid_Parameter,
	/** API is being called too frequently. */
	ZoomVideoSDKErrors_Call_Too_Frequently,
	/** The requested feature is not implemented.  */
	ZoomVideoSDKErrors_No_Impl,
	/** The current session does not support the feature. */
	ZoomVideoSDKErrors_Dont_Support_Feature,
	/** Unknown error occurred. */
	ZoomVideoSDKErrors_Unknown,
	/** Failed to remove folder. */
	ZoomVideoSDKErrors_Remove_Folder_Fail,
	/** General authorization failure. */
	ZoomVideoSDKErrors_Auth_Error = 1001,
	/** Authorization failed due to missing key or secret. */
	ZoomVideoSDKErrors_Auth_Empty_Key_or_Secret,
	/** Authorization failed due to invalid key or secret. */
	ZoomVideoSDKErrors_Auth_Wrong_Key_or_Secret,
	/** Authorization does not support SDK. */
	ZoomVideoSDKErrors_Auth_DoesNot_Support_SDK,
	/** Authorization disable SDK. */
	ZoomVideoSDKErrors_Auth_Disable_SDK,
	/** Session name is missing when joining session. */
	ZoomVideoSDKErrors_JoinSession_NoSessionName = 1500,
	/** Session token is missing when joining session. */
	ZoomVideoSDKErrors_JoinSession_NoSessionToken,
	/** User name is missing when joining session. */
	ZoomVideoSDKErrors_JoinSession_NoUserName,
	/** Session name is invalid. */
	ZoomVideoSDKErrors_JoinSession_Invalid_SessionName,
	/** Session password is invalid. */
	ZoomVideoSDKErrors_JoinSession_Invalid_Password,
	/** Session token is invalid. */
	ZoomVideoSDKErrors_JoinSession_Invalid_SessionToken,
	/** Session name exceeds the allowed length. */
	ZoomVideoSDKErrors_JoinSession_SessionName_TooLong,
	/** Token does not match the session name. */
	ZoomVideoSDKErrors_JoinSession_Token_MismatchedSessionName,
	/** Token does not include session name. */
	ZoomVideoSDKErrors_JoinSession_Token_NoSessionName,
	/** Token has empty or invalid role type. */
	ZoomVideoSDKErrors_JoinSession_Token_RoleType_EmptyOrWrong,
	/** Token includes a user identity that is too long. */
	ZoomVideoSDKErrors_JoinSession_Token_UserIdentity_TooLong,
	/** Failed to find session module. */
	ZoomVideoSDKErrors_SessionModule_Not_Found = 2001,
	/** Invalid session service. */
	ZoomVideoSDKErrors_SessionService_Invalid,
	/** Failed to join the session. */
	ZoomVideoSDKErrors_Session_Join_Failed,
	/** User does not have the rights to join the session. */
	ZoomVideoSDKErrors_Session_No_Rights,
	/** Session is already in progress. */
	ZoomVideoSDKErrors_Session_Already_In_Progress,
	/** The current session type is not supported. */
	ZoomVideoSDKErrors_Session_Dont_Support_SessionType,
	/** SDK is reconnecting to the session. */
	ZoomVideoSDKErrors_Session_Reconnecting,
	/** SDK is disconnecting from the session. */
	ZoomVideoSDKErrors_Session_Disconnecting,
	/** Session has not started yet. */
	ZoomVideoSDKErrors_Session_Not_Started,
	/** Session requires a password. */
	ZoomVideoSDKErrors_Session_Need_Password,
	/** Incorrect session password. */
	ZoomVideoSDKErrors_Session_Password_Wrong,
	/** Error from remote database. */
	ZoomVideoSDKErrors_Session_Remote_DB_Error,
	/** Invalid session parameter. */
	ZoomVideoSDKErrors_Session_Invalid_Param,
	/** Client is incompatible with the session. */
	ZoomVideoSDKErrors_Session_Client_Incompatible,
	/** Session ended due to exceeding free minutes. */
	ZoomVideoSDKErrors_Session_Account_FreeMinutesExceeded,
	/** Audio module error. */
	ZoomVideoSDKErrors_Session_Audio_Error = 3000,
	/** No microphone detected. */
	ZoomVideoSDKErrors_Session_Audio_No_Microphone,
	/** No speaker detected. */
	ZoomVideoSDKErrors_Session_Audio_No_Speaker,
	/** General video error. */
	ZoomVideoSDKErrors_Session_Video_Error = 4000,
	/** Video device error. */
	ZoomVideoSDKErrors_Session_Video_Device_Error,
	/** Live stream error. */
	ZoomVideoSDKErrors_Session_Live_Stream_Error = 5000,
	/** Phone connection error. */
	ZoomVideoSDKErrors_Session_Phone_Error = 5500,

	/** Failed to allocate memory for raw data. */
	ZoomVideoSDKErrors_RAWDATA_MALLOC_FAILED = 6001,
	/** Not in a session for raw data access. */
	ZoomVideoSDKErrors_RAWDATA_NOT_IN_Session,
	/** No raw data license available. */
	ZoomVideoSDKErrors_RAWDATA_NO_LICENSE,
	/** Video module not ready for raw data. */
	ZoomVideoSDKErrors_RAWDATA_VIDEO_MODULE_NOT_READY,
	/** Video module error. */
	ZoomVideoSDKErrors_RAWDATA_VIDEO_MODULE_ERROR,
	/** Video device error. */
	ZoomVideoSDKErrors_RAWDATA_VIDEO_DEVICE_ERROR,
	/** No video data available. */
	ZoomVideoSDKErrors_RAWDATA_NO_VIDEO_DATA,
	/** Share module not ready. */
	ZoomVideoSDKErrors_RAWDATA_SHARE_MODULE_NOT_READY,
	/** Share module error. */
	ZoomVideoSDKErrors_RAWDATA_SHARE_MODULE_ERROR,
	/** No shared data available. */
	ZoomVideoSDKErrors_RAWDATA_NO_SHARE_DATA,
	/** Audio module not ready. */
	ZoomVideoSDKErrors_RAWDATA_AUDIO_MODULE_NOT_READY,
	/** Audio module error. */
	ZoomVideoSDKErrors_RAWDATA_AUDIO_MODULE_ERROR,
	/** No audio data available. */
	ZoomVideoSDKErrors_RAWDATA_NO_AUDIO_DATA,
	/** Raw data preprocessing error. */
	ZoomVideoSDKErrors_RAWDATA_PREPROCESS_RAWDATA_ERROR,
	/** No media device is currently running. */
	ZoomVideoSDKErrors_RAWDATA_NO_DEVICE_RUNNING,
	/** Failed to initialize media device. */
	ZoomVideoSDKErrors_RAWDATA_INIT_DEVICE,
	/** Using a virtual device. */
	ZoomVideoSDKErrors_RAWDATA_VIRTUAL_DEVICE,
	/** Cannot change virtual device during preview. */
	ZoomVideoSDKErrors_RAWDATA_CANNOT_CHANGE_VIRTUAL_DEVICE_IN_PREVIEW,
	/** Internal raw data error. */
	ZoomVideoSDKErrors_RAWDATA_INTERNAL_ERROR,
	/** Sending too much raw data in a single call. */
	ZoomVideoSDKErrors_RAWDATA_SEND_TOO_MUCH_DATA_IN_SINGLE_TIME,
	/** Sending raw data too frequently. */
	ZoomVideoSDKErrors_RAWDATA_SEND_TOO_FREQUENTLY,
	/** Virtual microphone has been terminated. */
	ZoomVideoSDKErrors_RAWDATA_VIRTUAL_MIC_IS_TERMINATE,
	/** General share error. */
	ZoomVideoSDKErrors_Session_Share_Error = 7001,
	/** Share module is not ready. */
	ZoomVideoSDKErrors_Session_Share_Module_Not_Ready,
	/** You are not sharing. */
	ZoomVideoSDKErrors_Session_Share_You_Are_Not_Sharing,
	/** Share type is not supported. */
	ZoomVideoSDKErrors_Session_Share_Type_Is_Not_Support,
	/** Internal sharing error. */
	ZoomVideoSDKErrors_Session_Share_Internal_Error,
	/** Multi-stream video users are not supported. */
	ZoomVideoSDKErrors_Dont_Support_Multi_Stream_Video_User,
	/** Failed to assign user privilege. */
	ZoomVideoSDKErrors_Fail_Assign_User_Privilege,
	/** No recording in process. */
	ZoomVideoSDKErrors_No_Recording_In_Process,
	/** Failed to set virtual background. */
	ZoomVideoSDKErrors_Set_Virtual_Background_Fail,
	/** Failed to start camera share due to video not started. */
	ZoomVideoSDKErrors_Session_Share_Camera_Video_Not_Start,
	/** Camera share conflicts with video effects. */
	ZoomVideoSDKErrors_Session_Share_Camera_Conflict_With_Video_Effects,
	/** Unknown file transfer error. */
	ZoomVideoSDKErrors_Filetransfer_UnknowError = 7500,
	/** File type is blocked for transfer. */
	ZoomVideoSDKErrors_Filetransfer_FileTypeBlocked,
	/** File size exceeds the transfer limit. */
	ZoomVideoSDKErrors_Filetransfer_FileSizelimited,
	/** Not enough users for spotlighting. */
	ZoomVideoSDKErrors_Spotlight_NotEnoughUsers = 7600,
	/** Too many users are spotlighted. */
	ZoomVideoSDKErrors_Spotlight_ToMuchSpotlightedUsers,
	/** User cannot be spotlighted. */
	ZoomVideoSDKErrors_Spotlight_UserCannotBeSpotlighted,
	/** User does not have video to be spotlighted. */
	ZoomVideoSDKErrors_Spotlight_UserWithoutVideo,
	/** User is not currently spotlighted. */
	ZoomVideoSDKErrors_Spotlight_UserNotSpotlighted

}ZoomVideoSDKErrors;

/**
 * @brief Enumeration of session leave reasons in Zoom Video SDK.
 */
typedef enum 
{
	/** Unknown reason for leaving the session. */
	ZoomVideoSDKSessionLeaveReason_Unknown = 0,
	/** User left the session by themselves. */
	ZoomVideoSDKSessionLeaveReason_BySelf,
	/** User was removed from the session by the host. */
	ZoomVideoSDKSessionLeaveReason_KickByHost,
	/** Host ended the session. */
	ZoomVideoSDKSessionLeaveReason_EndByHost,
	/** User was disconnected due to network error. */
	ZoomVideoSDKSessionLeaveReason_NetworkError,
}ZoomVideoSDKSessionLeaveReason;

/**
 * @brief Enumeration of sharing status in Zoom Video SDK.
 */
typedef enum 
{
	/** No sharing activity. */
	ZoomVideoSDKShareStatus_None,
	/** Sharing has started. */
	ZoomVideoSDKShareStatus_Start,
	/** Sharing has been paused. */
	ZoomVideoSDKShareStatus_Pause,
	/** Sharing has resumed after being paused. */
	ZoomVideoSDKShareStatus_Resume,
	/** Sharing has been stopped. */
	ZoomVideoSDKShareStatus_Stop,
}ZoomVideoSDKShareStatus;

/**
 * @brief Enumeration of sharing types in Zoom Video SDK.
 */
typedef enum
{
	/** No sharing type specified. */
	ZoomVideoSDKShareType_None,
	/** Application or desktop sharing. */
	ZoomVideoSDKShareType_Normal,
	/** Pure computer audio sharing. */
	ZoomVideoSDKShareType_PureAudio,
	/** Camera sharing. */
	ZoomVideoSDKShareType_Camera, 
}ZoomVideoSDKShareType;

/**
 * @brief Enumeration of live streaming status in Zoom Video SDK.
 */
typedef enum
{
	/** No live streaming activity. */
	ZoomVideoSDKLiveStreamStatus_None,
	/** Live streaming is currently in progress. */
	ZoomVideoSDKLiveStreamStatus_InProgress,
	/** Live streaming is connecting. */
	ZoomVideoSDKLiveStreamStatus_Connecting,
	/** Live streaming failed due to timeout. */
	ZoomVideoSDKLiveStreamStatus_FailedTimeout,
	/** Live streaming failed to start. */
	ZoomVideoSDKLiveStreamStatus_StartFailed,
	/** Live streaming has ended. */
	ZoomVideoSDKLiveStreamStatus_Ended,
}ZoomVideoSDKLiveStreamStatus;

/**
 * @brief Enumeration of raw data memory mode in Zoom Video SDK.
 */
typedef enum
{
	/** Raw data is allocated on the stack. */
	ZoomVideoSDKRawDataMemoryModeStack = 0,
	/** Raw data is allocated on the heap. */
	ZoomVideoSDKRawDataMemoryModeHeap,
}ZoomVideoSDKRawDataMemoryMode;

/**
 * @brief Enumeration of raw data types in Zoom Video SDK.
 */
typedef enum
{
	/** Raw video data. */
	RAW_DATA_TYPE_VIDEO = 0,
	/** Raw share data (e.g., screen or application). */
	RAW_DATA_TYPE_SHARE,
}ZoomVideoSDKRawDataType;

/**
 * @brief Enumeration of camera control request types in Zoom Video SDK.
 */
typedef enum
{
	/** Request camera control. */
	ZoomVideoSDKCameraControlRequestType_RequestControl = 0,
	/** Give up camera control. */
	ZoomVideoSDKCameraControlRequestType_GiveUpControl
	
}ZoomVideoSDKCameraControlRequestType;

/**
 * @brief Enumeration of remote control status in Zoom Video SDK.
 */
enum ZoomVideoSDKRemoteControlStatus
{
	/** For initialization. */
	ZoomVideoSDKRemoteControlStatus_None = 0,			
	/** Remote control information. I can enable the remote control. */
	ZoomVideoSDKRemoteControlStatus_CanRequestControl,		
	/** Remote control information. I have received a refused information of remote control. */
	ZoomVideoSDKRemoteControlStatus_RequestDenied,	
	/** The authority of remote control. I get control when remote control begins. */
	ZoomVideoSDKRemoteControlStatus_GotControl,			
	/** The authority of remote control. I lose control when remote control ends. */
	ZoomVideoSDKRemoteControlStatus_LostControl,		
	 /** The status of remote control. I am remotely controlling another. */
	ZoomVideoSDKRemoteControlStatus_ControlStart,		
	/** The status of remote control. I am not remotely controlling another. */
	ZoomVideoSDKRemoteControlStatus_ControlStop,		
	/** The status of remote control. I can be controlled by the specified user. */
	ZoomVideoSDKRemoteControlStatus_GiveControlTo,		
	/** The status of remote control. I have revoked remote control. */
	ZoomVideoSDKRemoteControlStatus_ControlRevoked,		
	/** The status of remote control. I am being controlled by the specified user. */
	ZoomVideoSDKRemoteControlStatus_ControlledBy,		
	/** The status of remote control. I am not being controlled. */
	ZoomVideoSDKRemoteControlStatus_NotControlled,		
};

/**
 * @brief Enumeration representing the status of recording.
 */
enum RecordingStatus
{
	/** The recording has successfully started or resumed. */
	Recording_Start,
	/** The recording has been stopped. */
	Recording_Stop,
	/** The recording failed due to insufficient disk space. */
	Recording_DiskFull,
	/** The recording has been paused. */
	Recording_Pause,
};

/**
 * @brief Enumeration of the microphone test status in Zoom Video SDK.
 */
typedef	enum
{
	/** Test the mic via TestMicStartRecording. It is useless to call TestMicStopTesting/TestMicPlayRecording in this status. */
	ZoomVideoSDKMic_CanTest = 0,
	 /**Test the mic via TestMicStopTesting/TestMicPlayRecording. It is useless to call TestMicStartRecording in this status. */
	ZoomVideoSDKMic_Recording,
	 /**Test the mic via TestMicStopTesting/TestMicPlayRecording. It is useless call TestMicStartRecording in this status. */
	ZoomVideoSDKMic_CanPlay,
}ZoomVideoSDK_TESTMIC_STATUS;

/**
 * @brief Enumeration of preferred video resolution in Zoom Video SDK.
 */
enum ZoomVideoSDKPreferVideoResolution
{
	/** Invalid value */
	ZoomVideoSDKPreferVideoResolution_None,
	/** The camera opens in 360p by default. */
	ZoomVideoSDKPreferVideoResolution_360P,
	/** The camera opens in 720p by default. */
	ZoomVideoSDKPreferVideoResolution_720P,
	/** The camera opens in 1080p by default. */
	ZoomVideoSDKPreferVideoResolution_1080P
};

/**
 * @brief The extended parameters for Zoom Video SDK initialization.
 */
struct ZoomVideoSDKExtendParams
{
	/**
	 * The file path for speaker test playback.
	 * Only supports MP3 format. The file size must not exceed 1 MB.
	 */
	const zchar_t* speakerTestFilePath; 
	int wrapperType;
	ZoomVideoSDKPreferVideoResolution preferVideoResolution;

	ZoomVideoSDKExtendParams()
	{
		speakerTestFilePath = nullptr;
		wrapperType = 0;
		preferVideoResolution = ZoomVideoSDKPreferVideoResolution_None;
	}
};

/**
 * @brief The initialization parameters for Zoom Video SDK.
 */
struct ZoomVideoSDKInitParams
{
	/** The web domain used by the SDK. This is a required parameter. */
	const zchar_t* domain;
	/** Prefix for the generated log file names. Optional. If not set, a default prefix will be used. */
	const zchar_t* logFilePrefix;
	/** Whether to enable SDK logging. Optional. Default is false. */
	bool  enableLog; 
	/** Whether to enable indirect raw data access. Optional. Default is false. */
	bool  enableIndirectRawdata; 
	/** Memory allocation mode for audio raw data (stack or heap). Optional. Default is stack. */
	ZoomVideoSDKRawDataMemoryMode audioRawDataMemoryMode; 
	/** Memory allocation mode for video raw data (stack or heap). Optional. Default is stack. */
	ZoomVideoSDKRawDataMemoryMode videoRawDataMemoryMode; 
	/** Memory allocation mode for share raw data (stack or heap). Optional. Default is stack. */
	ZoomVideoSDKRawDataMemoryMode shareRawDataMemoryMode; 

	/** Extended optional parameters for SDK initialization. For more details, see the \link ZoomVideoSDKExtendParams \endlink. */
	ZoomVideoSDKExtendParams extendParam;
	ZoomVideoSDKInitParams()
	{
		domain = nullptr;
		logFilePrefix = nullptr;
		enableLog = false;
		enableIndirectRawdata = false;
		audioRawDataMemoryMode = ZoomVideoSDKRawDataMemoryModeStack;
		videoRawDataMemoryMode = ZoomVideoSDKRawDataMemoryModeStack;
		shareRawDataMemoryMode = ZoomVideoSDKRawDataMemoryModeStack;
	}
};

/**
*@brief Video option used by ZoomVideoSDKSessionContext.
*/
struct ZoomVideoSDKVideoOption 
{
	/**
	 * @brief Indicates whether the local video should be turned on by default.
	 * @note Default value is true.
	 */
    bool localVideoOn; 
	ZoomVideoSDKVideoOption()
	{
		localVideoOn = true;
	}
};
/**
*@brief Aduio option used by ZoomVideoSDKSessionContext.
*/
struct ZoomVideoSDKAudioOption 
{
	/**
	 * @brief Whether to connect the local audio when joining a session.
	 * @note Default is true.
	 */
    bool connect; ///<Whether to connect local audio or not.
	/**
	 * @brief Whether to mute the audio initially.
	 * @note Default is false.
	 */
    bool mute;  
	/**
	 * @brief Whether my voice should be included in the mixed audio raw data.
	 * @note Default is true.
	 */
	bool isMyVoiceInMix; 
	/**
	 * @brief Whether to automatically adjust the speaker volume.
	 * If true, the SDK will automatically raise the volume if it's muted or too low.
	 * @note Default is false.
	 */
	bool autoAdjustSpeakerVolume; 
	ZoomVideoSDKAudioOption()
	{
		connect = true;
		mute = false;
		isMyVoiceInMix = true;
		autoAdjustSpeakerVolume = false;
	}
};

class IZoomVideoSDKVideoSourcePreProcessor;
class IZoomVideoSDKVideoSource;
class IZoomVideoSDKVirtualAudioMic;
class IZoomVideoSDKVirtualAudioSpeaker;

/**
 * @brief Configuration context for joining a Zoom Video SDK session.
 */
struct ZoomVideoSDKSessionContext
{
	/**
	 * @brief The name of the session to join.
	 * @note This is required.
	 */
    const zchar_t* sessionName;		
	/**
	 * @brief The password for the session.
	 * @note Optional.
	 */
    const zchar_t* sessionPassword; 
	/**
	 * @brief The display name of the user in the session.
	 * @note This is required.
	 */
    const zchar_t* userName;        
	 /**
	 * @brief The JWT token used for session authentication.
	 * @note This is required.
	 */
    const zchar_t* token;			
	/**
	 * @brief Video configuration options.
	 * @note Optional.
	 */
    ZoomVideoSDKVideoOption videoOption; 
	/**
	 * @brief Audio configuration options.
	 * @note Optional.
	 */
    ZoomVideoSDKAudioOption audioOption; 
	/**
	 * @brief Pointer to the camera data preprocessor object.
	 * @note Optional.
	 */
	IZoomVideoSDKVideoSourcePreProcessor* preProcessor; 
	/**
	 * @brief Pointer to the virtual camera source object.
	 * Use this to provide your own external video stream.
	 * @note Optional.
	 */
	IZoomVideoSDKVideoSource* externalVideoSource; 
	/**
	 * @brief Pointer to the virtual microphone object.
	 * Use this to provide custom audio input.
	 * @note Optional.
	 */
	IZoomVideoSDKVirtualAudioMic* virtualAudioMic; 
	/**
	 * @brief Pointer to the virtual speaker object.
	 * Use this to receive and process audio output.
	 * @note Optional.
	 */
	IZoomVideoSDKVirtualAudioSpeaker* virtualAudioSpeaker; 
	/**
	 * @brief The duration (in minutes) before an idle session times out.
	 * If set to 0, the session will never timeout automatically.
	 * @note Optional. Default is 40 minutes.
	 */
	unsigned int sessionIdleTimeoutMins;
	/**
	 * @brief Whether to automatically load multiple video streams when joining a session.
	 * @note Optional. Default is true.
	 */
    bool autoLoadMutliStream; 

	ZoomVideoSDKSessionContext()
	{
		sessionName = nullptr;
		sessionPassword = nullptr;
		userName = nullptr;
		token = nullptr;
		preProcessor = nullptr;
		externalVideoSource = nullptr;
		virtualAudioMic = nullptr;
		virtualAudioSpeaker = nullptr;
		sessionIdleTimeoutMins = 40;
        autoLoadMutliStream = true;
	}
};

/**
 * @brief Enumeration of specifying the format of frame data.
 */
enum FrameDataFormat
{
	/**
	 * @brief I420 format with limited (TV) range.
	 * Commonly used in video broadcasting and streaming.
	 */
	FrameDataFormat_I420_LIMITED,
	/**
	 * @brief I420 format with full (PC) range.
	 * Provides full-range color values, typically used in computer graphics.
	 */
	FrameDataFormat_I420_FULL,
};

/**
 * @brief Enumeration representing the status of a file transfer.
 */
typedef enum
{
	/** No file transfer activity is currently occurring. */
	FileTransferState_None = 0, 
	/** The file is ready to begin transfer. */
	FileTransferState_ReadyToTransfer, 
	/** The file transfer is currently in progress. */
	FileTransferState_Transfering, 
	/** The file transfer has failed. */
	FileTransferState_TransferFailed, 
	/** The file transfer completed successfully. */
	FileTransferState_TransferDone, 
}FileTransferStatus;

/**
 * @brief The progress information of an ongoing file transfer.
 */
struct FileTransferProgress
{
	/** @brief Transfer completion ratio. */
	uint32_t ratio; 
	/** @brief The total number of bytes transferred so far. */
	uint32_t completeSize; 
	/** @brief The current transfer speed in bits per second (bps). */
	uint32_t bitPerSecond;
	FileTransferProgress()
	{
		ratio = 0;
		completeSize = 0;
		bitPerSecond = 0;
	}
};

/**
 * @brief The status and progress of a file transfer in Zoom Video SDK.
 */
struct ZoomVideoSDKFileStatus
{
	/**
	* @brief Current status of the file transfer.
	*
	* See @ref FileTransferStatus for possible values.
	*/
	FileTransferStatus transStatus; 
	/**
	 * @brief Detailed progress information of the file transfer.
	 *
	 * See @ref FileTransferProgress for data members like ratio, transferred size, and speed.
	 */
	FileTransferProgress transProgress;
	ZoomVideoSDKFileStatus()
	{
		transStatus = FileTransferState_None;
		transProgress = {};
	}
};

/**
 * @brief Retrieving basic information about a file transfer in the Zoom Video SDK.
 */
class IZoomVideoSDKFileTransferBaseInfo
{
public:
	virtual ~IZoomVideoSDKFileTransferBaseInfo() {}
	/**
	 * @brief Get the timestamp when the file transfer started.
	 * @return The UNIX timestamp (in seconds) of the file transfer.
	 */
	virtual time_t getTimeStamp() = 0;
	/**
	 * @brief Check whether the file was sent to all participants.
	 * @return true if the file was sent to all, false otherwise.
	 */
	virtual bool isSendToAll() = 0;
	/**
	 * @brief Get the size of the file in bytes.
	 * @return File size in bytes.
	 */
	virtual uint32_t getFileSize() = 0;
	/**
	 * @brief Get the name of the file being transferred.
	 * @return Pointer to a string containing the file name.
	 */
	virtual const zchar_t* getFileName() = 0;
	/**
	 * @brief Get the current transfer status of the file.
	 * @return A ZoomVideoSDKFileStatus structure with current status and progress.
	 */
	virtual ZoomVideoSDKFileStatus getStatus() = 0;

};
class IZoomVideoSDKUser;
/**
 * @brief Interface for managing a file that is being sent in the Zoom Video SDK.
 * Inherits from IZoomVideoSDKFileTransferBaseInfo.
 */
class IZoomVideoSDKSendFile : public IZoomVideoSDKFileTransferBaseInfo
{
public:
	virtual ~IZoomVideoSDKSendFile() {}
	/**
	 * @brief Get the receiver of the file transfer.
	 * @return A pointer to the @ref IZoomVideoSDKUser representing the recipient.
	 */
	virtual IZoomVideoSDKUser* getReceiver() = 0;
	/**
	 * @brief Cancel the ongoing file transfer.
	 * @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success. Otherwise failed. For detailed error codes, see the \link ZoomVideoSDKErrors \endlink enum.
	 */
	virtual ZoomVideoSDKErrors cancelSend() = 0;
};

/**
 * @brief Interface for managing a file that is being received in the Zoom Video SDK.
 * Inherits from IZoomVideoSDKFileTransferBaseInfo.
 */
class IZoomVideoSDKReceiveFile : public IZoomVideoSDKFileTransferBaseInfo
{
public:
	virtual ~IZoomVideoSDKReceiveFile() {}
	/**
	 * @brief Get the sender of the file transfer.
	 * @return A pointer to the @ref IZoomVideoSDKUser representing the sender.
	 */
	virtual IZoomVideoSDKUser* getSender() = 0;
	/**
	 * @brief Cancel the ongoing file receive operation.
	 * @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success. Otherwise failed. For detailed error codes, see the \link ZoomVideoSDKErrors \endlink enum.
	 */
	virtual ZoomVideoSDKErrors cancelReceive() = 0;
   /**
	* @brief Start receiving the file and specify the download path.
	* @param path The local file system path where the file will be saved.
	* @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success. Otherwise failed. For detailed error codes, see the \link ZoomVideoSDKErrors \endlink enum.
	*/
	virtual ZoomVideoSDKErrors startReceive(const zchar_t* path) = 0;
};

/**
 * @brief Enumeration of audio channel type in Zoom Video SDK.
 */
enum ZoomVideoSDKAudioChannel
{
	/** Mono audio channel. */
	ZoomVideoSDKAudioChannel_Mono,
	/** Stereo audio channel. */
	ZoomVideoSDKAudioChannel_Stereo,
};

/**
 * @brief Enumeration of Zoom Video SDK session types.
 */
enum ZoomVideoSDKSessionType
{
	/** Main session. */
	ZoomVideoSDKSessionType_MainSession = 0,
	/** Sub session. */
	ZoomVideoSDKSessionType_SubSession,
};

/**
 * @brief Enumeration of Zoom Video SDK share setting types.
 */
enum ZoomVideoSDKShareSetting
{
    /** For initialization. */
    ZoomVideoSDKShareSetting_None = 0,
    /** Only host and manager can share. */
    ZoomVideoSDKShareSetting_LockedShare,
    /** Anyone can share, but only one at a time. Only host and manager can take over. */
    ZoomVideoSDKShareSetting_SingleShare,
    /** Multiple participants can share simultaneously. */
    ZoomVideoSDKShareSetting_MultiShare
};

END_ZOOM_VIDEO_SDK_NAMESPACE
#endif
