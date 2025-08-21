/**
 * @file zoom_video_sdk_livestream_helper_interface.h
 * @brief live stream helper
 */

#ifndef _ZOOM_VIDEO_SDK_LIVESTREAM_HELPER_INTERFACE_H_
#define _ZOOM_VIDEO_SDK_LIVESTREAM_HELPER_INTERFACE_H_
#include "zoom_video_sdk_def.h"
#include "zoom_video_sdk_vector_interface.h"

BEGIN_ZOOM_VIDEO_SDK_NAMESPACE
/**
 * @class IZoomVideoSDKLiveStreamHelper
 * @brief Live stream helper interface.
 */
class IZoomVideoSDKLiveStreamHelper
{
public:
	/**
	 * @brief Start live stream.
     * @param streamUrl The live stream url.
     * @param key The live stream key.
     * @param broadcastUrl The live stream broadcast url.
     * @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success. Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	 */
    virtual ZoomVideoSDKErrors startLiveStream(const zchar_t* streamUrl, const zchar_t* key, const zchar_t* broadcastUrl) = 0;
	
	/**
	 * @brief Stop live stream.
     * @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success. Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	 */
    virtual ZoomVideoSDKErrors stopLiveStream() = 0;
	
    /**
	 * @brief Determin if can start the live stream.
     * @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success. Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	 */
    virtual ZoomVideoSDKErrors canStartLiveStream() = 0;
};
/**
 * @brief incoming live stream status.
 */
struct IncomingLiveStreamStatus
{
	 /** The incoming live stream ID */
	const zchar_t* strStreamKeyID; 
	 /** Is the streaming software(such as OBS) connected to the Zoom platform? If false: no, if true: yes */
	bool isRTMPConnected; 
	 /** Has the video stream been pushed to the session? If false: no, if true: yes */
	bool isStreamPushed;  

	IncomingLiveStreamStatus()
	{
		strStreamKeyID = nullptr;
		isRTMPConnected = false;
		isStreamPushed = false;
	}
};
/**
 * @class IZoomVideoSDKIncomingLiveStreamHelper
 * @brief Incoming live stream helper interface.
 * @note Currently only supports one stream.
 */
class IZoomVideoSDKIncomingLiveStreamHelper
{
public:
	/**
	 * @brief Bind incoming live stream with a stream key ID.
	 * @param strStreamKeyID The stream key ID to bind.
	 * @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success. Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	 */
	virtual ZoomVideoSDKErrors bindIncomingLiveStream(const zchar_t* strStreamKeyID) = 0;
	
	/**
	 * @brief Unbind the bound incoming live stream.
	 * @param strStreamKeyID The stream key ID to unbind.
	 * @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success. Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	 */
	virtual ZoomVideoSDKErrors unbindIncomingLiveStream(const zchar_t* strStreamKeyID) = 0;
	
	/**
	 * @brief Get the status of bound streams.
	 * @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success. Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	 */
	virtual ZoomVideoSDKErrors getIncomingLiveStreamStatus() = 0;
	
	/**
	 * @brief Start the bound stream as a special participant.
	 * @param strStreamKeyID The stream key ID to start.
	 * @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success. Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	 */
	virtual ZoomVideoSDKErrors startIncomingLiveStream(const zchar_t* strStreamKeyID) = 0;
	
	/**
	 * @brief Stop the bound stream as a special participant.
	 * @param strStreamKeyID The stream key ID to stop.
	 * @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success. Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	 */
	virtual ZoomVideoSDKErrors stopIncomingLiveStream(const zchar_t* strStreamKeyID) = 0;
};

END_ZOOM_VIDEO_SDK_NAMESPACE
#endif