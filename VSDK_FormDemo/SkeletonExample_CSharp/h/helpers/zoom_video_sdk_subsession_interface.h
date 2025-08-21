/**
 * @file zoom_video_sdk_subsession_interface.h
 * @brief Subsession helper
 */

#ifndef _ZOOM_VIDEO_SDK_SUBSESSION_INTERFACE_H_
#define _ZOOM_VIDEO_SDK_SUBSESSION_INTERFACE_H_
#include "zoom_video_sdk_def.h"
#include "zoom_video_sdk_vector_interface.h"

BEGIN_ZOOM_VIDEO_SDK_NAMESPACE

/**
 * @brief Enumerations of subsession statuses.
 */
enum ZoomVideoSDKSubSessionStatus
{
	/** The Initialized status. */
	ZoomVideoSDKSubSessionStatus_None = 0,
	/** When a subsession is committed. */
	ZoomVideoSDKSubSessionStatus_Committed,
	/** When a subsession is withdrawn. */
	ZoomVideoSDKSubSessionStatus_Withdrawn,
	/** When the subsession is started. */
	ZoomVideoSDKSubSessionStatus_Started,
	/** When the subsession is stopping. */
	ZoomVideoSDKSubSessionStatus_Stopping,
	/** When the subsession is stopped. */
	ZoomVideoSDKSubSessionStatus_Stopped,
	/** When commit subsession failed. */
	ZoomVideoSDKSubSessionStatus_CommitFailed,
	/** When withdraw subsession failed. */
	ZoomVideoSDKSubSessionStatus_WithdrawFailed,
	/** When start subsession failed. */
	ZoomVideoSDKSubSessionStatus_StartFailed,
	/** When stop subsession failed. */
	ZoomVideoSDKSubSessionStatus_StopFailed,
};

/**
 * @brief Enumerations of subsession user help request results.
 */
enum ZoomVideoSDKUserHelpRequestResult
{
	/** The host has received the help request, and no one else is currently requesting help. */
	ZoomVideoSDKUserHelpRequestResult_Idle = 0,
	/** The host is handling another user's request with the request dialog. */
	ZoomVideoSDKUserHelpRequestResult_Busy,
	/** The host is handling another user's request. */
	ZoomVideoSDKUserHelpRequestResult_Ignore,
	/** The host is already in this subsession. */
	ZoomVideoSDKUserHelpRequestResult_HostAlreadyInSubSession
};


/**
 * @class ISubSessionUser
 * @brief Subsession user.
 */
class ISubSessionUser
{
public:
	virtual ~ISubSessionUser() {};

	/**
	* @brief Get the subsession user's username.
	* @return The username.
	*/
	virtual const zchar_t* getUserName() = 0;

	/**
	* @brief Get the subsession user's GUID.
	* @return The user GUID.
	*/
	virtual const zchar_t* getUserGUID() = 0;
};


/**
 * @class ISubSessionKit
 * @brief Subsession kit.
 */
class ISubSessionKit
{
public:
	virtual ~ISubSessionKit() {};

	/**
	* @brief Get the subsession's name.
	* @return The subsession name.
	*/
	virtual const zchar_t* getSubSessionName() = 0;

	/**
	* @brief Get the subsessions's ID.
	* @return The subsession ID.
	*/
	virtual const zchar_t* getSubSessionID() = 0;

	/**
	* @brief Get the list of users in this subsession.
	* @return If the function succeeds, it returns an array of ISubSessionUser. Otherwise returns NULL. For more details, see \link ISubSessionUser \endlink. 
	*/
	virtual IVideoSDKVector<ISubSessionUser*>* getSubSessionUserList() = 0;

	/**
	* @brief Join this subsession.
	* @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success, otherwise not.
	*/
	virtual ZoomVideoSDKErrors joinSubSession() = 0;
};


/**
 * @class ISubSessionUserHelpRequestHandler
 * @brief Subsession user help request handler.
 */
class ISubSessionUserHelpRequestHandler
{
public:
	virtual ~ISubSessionUserHelpRequestHandler() {};

	/**
	 * @brief Get the username of the user who sent this request.
	 * @return The username.
	 */
	virtual const zchar_t* getRequestUserName() = 0;

	/**
	 * @brief Get the name of the subsession where the user sent this request.
	 * @return The subsession name.
	 */
	virtual const zchar_t* getRequestSubSessionName() = 0;

	/**
	 * @brief Ignore this request.
	 * @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success, otherwise not.
	 * @note 'onSubSessionUserHelpRequestResult' is the corresponding callback notification.
	 */
	virtual ZoomVideoSDKErrors ignore() = 0;

	/**
	 * @brief Join the subsession from which the help request originated.
	 * @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success, otherwise not.
	 */
	virtual ZoomVideoSDKErrors joinSubSessionByUserRequest() = 0;
};


/**
 * @class IZoomVideoSDKSubSessionManager
 * @brief Subsession manager helper interface.
 */
class IZoomVideoSDKSubSessionManager
{
public:
	virtual ~IZoomVideoSDKSubSessionManager() {};

	/**
	 * @brief Start subsession.
	 * @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success, otherwise not.
	 * @note 'onSubSessionStatusChanged' is the corresponding callback notification.
	 */
	virtual ZoomVideoSDKErrors startSubSession() = 0;

	/**
	 * @brief Determine if the subsession has started.
	 * @return If the subsession is started, the return value is true, otherwise not.
	 */
	virtual bool isSubSessionStarted() = 0;

	/**
	 * @brief Stop subsession.
	 * @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success, otherwise not.
	 * @note 'onSubSessionStatusChanged' is the corresponding callback notification.
	 */
	virtual ZoomVideoSDKErrors stopSubSession() = 0;

	/**
	 * @brief Broadcast a message to all subsessions.
	 * @param sMessage The message to be broadcast.
	 * @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success, otherwise not.
	 * @note 'onBroadcastMessageFromMainSession' is the corresponding callback notification.
	 */
	virtual ZoomVideoSDKErrors broadcastMessage(const zchar_t* sMessage) = 0;
};


/**
 * @class IZoomVideoSDKSubSessionParticipant
 * @brief Subsession participant helper interface.
 */
class IZoomVideoSDKSubSessionParticipant
{
public:
	virtual ~IZoomVideoSDKSubSessionParticipant() {};

	/**
	 * @brief Return to main session.
	 * @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success, otherwise not.
	 */
	virtual ZoomVideoSDKErrors returnToMainSession() = 0;

	/**
	 * @brief Request help.
	 * @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success, otherwise not.
	 * @note 'onSubSessionUserHelpRequestResult'is the corresponding callback notification.
	 */
	virtual ZoomVideoSDKErrors requestForHelp() = 0;
};


/**
 * @class IZoomVideoSDKSubSessionHelper
 * @brief Subsession helper interface.
 * @note Only session host and manager can get this helper.
 */
class IZoomVideoSDKSubSessionHelper
{
public:
	virtual ~IZoomVideoSDKSubSessionHelper() {};

	/**
	 * @brief Add a subsession name to the prepared list.
	 * @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success, otherwise not.
	 */
	virtual ZoomVideoSDKErrors addSubSessionToPreList(const zchar_t* sSubSessionName) = 0;

	/**
	 * @brief Remove a subsession name from the prepared list.
	 * @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success, otherwise not.
	 */
	virtual ZoomVideoSDKErrors removeSubSessionFromPreList(const zchar_t* sSubSessionName) = 0;

	/**
	 * @brief Clear the prepared list.
	 * @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success, otherwise not.
	 */
	virtual ZoomVideoSDKErrors clearSubSessionPreList() = 0;

	/**
	 * @brief Get the list of prepared subsession names.
	 * @return If the function succeeds, it returns an array of subsession names.
	 */
	virtual IVideoSDKVector<const zchar_t*>* getSubSessionPreList() = 0;

	/**
	 * @brief Withdraw all committed subsessions and commit the prepared list.
	 * @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success, otherwise not.
	 * @note If the function succeeds, the prepared list will be cleared, 'onSubSessionStatusChanged'is the corresponding callback notification.
	 */
	virtual ZoomVideoSDKErrors commitSubSessionList() = 0;

	/**
	 * @brief Withdraw all committed subsessions.
	 * @return If the function succeeds, the return value is ZoomVideoSDKErrors_Success, otherwise not.
	 * @note If the function succeeds, the prepared list will be cleared, 'onSubSessionStatusChanged'is the corresponding callback notification.
	 */
	virtual ZoomVideoSDKErrors withdrawSubSessionList() = 0;

	/**
	 * @brief Get the list of commited subsessions.
	 * @return If the function succeeds, it returns an array of ISubSessionKit. Otherwise returns NULL. For more details, see \link ISubSessionKit \endlink. 
	 */
	virtual IVideoSDKVector<ISubSessionKit*>* getCommittedSubSessionList() = 0;
};
END_ZOOM_VIDEO_SDK_NAMESPACE
#endif