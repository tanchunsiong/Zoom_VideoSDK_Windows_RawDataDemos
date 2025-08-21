/**
 * @file zoom_video_sdk_crc_helper_interface.h
 * @brief crc helper interface.
 */

#ifndef _ZOOM_VIDEO_SDK_CALL_OUT_INTERFACE_H_
#define _ZOOM_VIDEO_SDK_CALL_OUT_INTERFACE_H_
#include "zoom_video_sdk_def.h"

BEGIN_ZOOM_VIDEO_SDK_NAMESPACE

/**
 * @brief CRC call status.
 * Here are more detailed structural descriptions.
 */
enum ZoomVideoSDKCRCCallStatus
{
	/** Call out successfully. */
	ZoomVideoSDKCRCCallStatus_Success = 0,	
	/** Bell during the call. */
	ZoomVideoSDKCRCCallStatus_Ring,			
	/** Call timeout. */
	ZoomVideoSDKCRCCallStatus_Timeout,		
	/** Busy. */
	ZoomVideoSDKCRCCallStatus_Busy,			
	/** Decline. */
	ZoomVideoSDKCRCCallStatus_Decline,		
	/** Call fails. */
	ZoomVideoSDKCRCCallStatus_Failed		
};

/**
 * @brief Call CRC protocol.
 * Here are more detailed structural descriptions.
 */
enum ZoomVideoSDKCRCProtocol
{
	/** H.323 type. */
	ZoomVideoSDKCRCProtocol_H323 = 0,	
	/** SIP type. */
	ZoomVideoSDKCRCProtocol_SIP			
};

/**
 * @class IZoomVideoSDKCRCHelper
 * @brief CRC Helper Interface.
 */
class IZoomVideoSDKCRCHelper
{
public:
	virtual ~IZoomVideoSDKCRCHelper() {}
	
	/**
	 * @brief Query if the CRC feature enabled.
	 * @return True means that the CRC enabled, otherwise disabled.
	 * @note Only get the correct value after join session.
	 */
	virtual bool isCRCEnabled() = 0;
	
	/**
	 * @brief Call CRC device.
	 * @param sAddress The CRC device's ip.
	 * @param eProtocol The protocol of the CRC device.
	 * @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success. Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	 * @note If the function succeeds, the IZoomVideoSDKDelegate::onCallCRCDeviceStatusChanged(status) will be triggered once the call crc device status changes.
	 */
	virtual ZoomVideoSDKErrors callCRCDevice(const zchar_t* sAddress, ZoomVideoSDKCRCProtocol eProtocol) = 0;
	
	/**
	 * @brief Cancel call CRC device.
	 * @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success. Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	 */
	virtual ZoomVideoSDKErrors cancelCallCRCDevice() = 0;
	
	/**
     * @brief Get the dial-in SIP address of a live session when CRC is enabled.
     * @return If the function succeeds, the return value is the live session SIP address when the CRC is enabled. Otherwise it fails, and the return value is the string of length zero(0)
	 */
    virtual const zchar_t* getSessionSIPAddress() = 0;
};

END_ZOOM_VIDEO_SDK_NAMESPACE
#endif