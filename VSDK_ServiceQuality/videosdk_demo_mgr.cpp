#include "stdafx.h"
#include "videosdk_demo_mgr.h"



ZoomVideoSDKMgr::ZoomVideoSDKMgr()
	: video_sdk_obj_(nullptr)
	, is_inited_(false)
{

}

ZoomVideoSDKMgr::~ZoomVideoSDKMgr()
{

}

//dreamtcs experimental
IZoomVideoSDKSession* ZoomVideoSDKMgr::getSessionInfo()
{
	IZoomVideoSDKSession* videosdksession = nullptr;
	
		if (video_sdk_obj_)
		{
			videosdksession = video_sdk_obj_->getSessionInfo();
		}

	
	return videosdksession;
}


//dreamtcs experimental
IZoomVideoSDKAudioHelper* ZoomVideoSDKMgr::getAudioHelper()
{
	IZoomVideoSDKAudioHelper* audioHelper = nullptr;
	if (is_inited_)
	{
		if (video_sdk_obj_)
		{
			audioHelper= video_sdk_obj_->getAudioHelper();
		}
		
	}
	return audioHelper;
}

//dreamtcs experimental
IZoomVideoSDKShareHelper* ZoomVideoSDKMgr::getShareHelper()
{
	IZoomVideoSDKShareHelper* shareHelper = nullptr;
	if (is_inited_)
	{
		if (video_sdk_obj_)
		{
			shareHelper = video_sdk_obj_->getShareHelper();
		}

	}
	return shareHelper;
}

//dreamtcs experimental
IZoomVideoSDKRecordingHelper* ZoomVideoSDKMgr::getRecordingHelper()
{
	IZoomVideoSDKRecordingHelper* recordingHelper = nullptr;
	if (is_inited_)
	{
		if (video_sdk_obj_)
		{
			recordingHelper = video_sdk_obj_->getRecordingHelper();
		}

	}
	return recordingHelper;
}

//dreamtcs experimental
IZoomVideoSDKPhoneHelper* ZoomVideoSDKMgr::getPhoneHelper()
{
	IZoomVideoSDKPhoneHelper* phoneHelper = nullptr;
	if (is_inited_)
	{
		if (video_sdk_obj_)
		{
			phoneHelper = video_sdk_obj_->getPhoneHelper();
		}

	}
	return phoneHelper;
}

//dreamtcs experimental
IZoomVideoSDKVideoHelper* ZoomVideoSDKMgr::getVideoHelper()
{
	IZoomVideoSDKVideoHelper* videoHelper = nullptr;
	if (is_inited_)
	{
		if (video_sdk_obj_)
		{
			videoHelper = video_sdk_obj_->getVideoHelper();
		}

	}
	return videoHelper;
}

//dreamtcs experimental
IZoomVideoSDKTestAudioDeviceHelper* ZoomVideoSDKMgr::getAudioDeviceTestHelper()
{
	IZoomVideoSDKTestAudioDeviceHelper* audioDeviceHelper = nullptr;
	if (is_inited_)
	{
		if (video_sdk_obj_)
		{
			audioDeviceHelper = video_sdk_obj_->GetAudioDeviceTestHelper();
		}

	}
	return audioDeviceHelper;
}

ZoomVideoSDKMgr& ZoomVideoSDKMgr::GetInst()
{
	static ZoomVideoSDKMgr inst;
	
	return inst;
}

bool ZoomVideoSDKMgr::Init(IZoomVideoSDKDelegate* listener, ZoomVideoSDKInitParams init_params)
{
	if (!is_inited_)
	{
		video_sdk_obj_ = CreateZoomVideoSDKObj();

		if (video_sdk_obj_)
		{
			ZoomVideoSDKErrors err = video_sdk_obj_->initialize(init_params);

			if (ZoomVideoSDKErrors_Success == err)
			{
				is_inited_ = true;
				video_sdk_obj_->addListener(listener);
			}
		}
	}
	return is_inited_;
}

void ZoomVideoSDKMgr::UnInit()
{
	if (is_inited_)
	{
		if (video_sdk_obj_)
		{
			video_sdk_obj_->cleanup();
			DestroyZoomVideoSDKObj();
			video_sdk_obj_ = nullptr;
		}
		is_inited_ = false;
	}
}

IZoomVideoSDKSession* ZoomVideoSDKMgr::JoinSession(ZoomVideoSDKSessionContext& session_context)
{
	IZoomVideoSDKSession* session = nullptr;

	if (video_sdk_obj_)
	{
		session = video_sdk_obj_->joinSession(session_context);
	}

	return session;
}

ZoomVideoSDKErrors ZoomVideoSDKMgr::LeaveSession(bool end)
{
	if (video_sdk_obj_)
	{
		return video_sdk_obj_->leaveSession(end);
	}

	return ZoomVideoSDKErrors_Uninitialize;
}

ZoomVideoSDKErrors ZoomVideoSDKMgr::MuteAudio()
{
	if (video_sdk_obj_)
	{
		IZoomVideoSDKAudioHelper* audio_helper = video_sdk_obj_->getAudioHelper();
		if (audio_helper)
		{
			IZoomVideoSDKUser* my_self = GetMySelf();
			if (my_self)
			{
				return audio_helper->muteAudio(my_self);
			}
		}
	}
	return ZoomVideoSDKErrors_Uninitialize;
}

ZoomVideoSDKErrors ZoomVideoSDKMgr::UnmuteAudio()
{
	if (video_sdk_obj_)
	{
		IZoomVideoSDKAudioHelper* audio_helper = video_sdk_obj_->getAudioHelper();
		if (audio_helper)
		{
			IZoomVideoSDKUser* my_self = GetMySelf();
			if (my_self)
			{
				return audio_helper->unMuteAudio(my_self);
			}
		}
	}
	return ZoomVideoSDKErrors_Uninitialize;
}

ZoomVideoSDKErrors ZoomVideoSDKMgr::MuteVideo()
{
	if (video_sdk_obj_)
	{
		IZoomVideoSDKVideoHelper* video_helper = video_sdk_obj_->getVideoHelper();
		if (video_helper)
		{
			return video_helper->stopVideo();
		}
	}
	return ZoomVideoSDKErrors_Uninitialize;
}

ZoomVideoSDKErrors ZoomVideoSDKMgr::UnmuteVideo()
{
	if (video_sdk_obj_)
	{
		IZoomVideoSDKVideoHelper* video_helper = video_sdk_obj_->getVideoHelper();
		if (video_helper)
		{
			return video_helper->startVideo();
		}
	}
	return ZoomVideoSDKErrors_Uninitialize;
}

bool ZoomVideoSDKMgr::IsMyselfVideoOn()
{
	if (video_sdk_obj_)
	{
		IZoomVideoSDKUser* myself = GetMySelf();
		if (myself)
		{
			ZoomVideoSDKVideoStatus video_status = myself->getVideoStatus();
			if (video_status.isOn)
				return true;
			else
				return false;
		}
	}
	return false;
}

bool ZoomVideoSDKMgr::IsMyselfAudioMuted()
{
	if (video_sdk_obj_)
	{
		IZoomVideoSDKUser* myself = GetMySelf();
		if (myself)
		{
			ZoomVideoSDKAudioStatus audio_status = myself->getAudioStatus();
			if (audio_status.isMuted)
				return true;
			else
				return false;
		}
	}
	return false;
}

bool ZoomVideoSDKMgr::SelectCamera(const zchar_t* camera_device_id)
{
	if (video_sdk_obj_)
	{
		IZoomVideoSDKVideoHelper* video_helper = video_sdk_obj_->getVideoHelper();
		if (video_helper)
		{
			return video_helper->selectCamera(camera_device_id);
		}
	}
	return false;
}

ZoomVideoSDKErrors ZoomVideoSDKMgr::SelectSpeaker(const zchar_t* device_id, const zchar_t* device_name)
{
	if (video_sdk_obj_)
	{
		IZoomVideoSDKAudioHelper* audio_helper = video_sdk_obj_->getAudioHelper();
		if (audio_helper)
		{
			return audio_helper->selectSpeaker(device_id, device_name);
		}
	}
	return ZoomVideoSDKErrors_Uninitialize;
}

ZoomVideoSDKErrors ZoomVideoSDKMgr::SelectMic(const zchar_t* device_id, const zchar_t* device_name)
{
	if (video_sdk_obj_)
	{
		IZoomVideoSDKAudioHelper* audio_helper = video_sdk_obj_->getAudioHelper();
		if (audio_helper)
		{
			return audio_helper->selectMic(device_id, device_name);
		}
	}
	return ZoomVideoSDKErrors_Uninitialize;
}

ZoomVideoSDKErrors ZoomVideoSDKMgr::StartShareScreen(const zchar_t* monitorID, ZoomVideoSDKShareOption option)
{
	if (video_sdk_obj_)
	{
		IZoomVideoSDKShareHelper* share_helper = video_sdk_obj_->getShareHelper();
		if (share_helper)
		{
			return share_helper->startShareScreen(monitorID, option);
		}
	}
	return ZoomVideoSDKErrors_Uninitialize;
}

ZoomVideoSDKErrors ZoomVideoSDKMgr::StartShareView(HWND hwnd, ZoomVideoSDKShareOption option)
{
	if (video_sdk_obj_)
	{
		IZoomVideoSDKShareHelper* share_helper = video_sdk_obj_->getShareHelper();
		if (share_helper)
		{
			return share_helper->startShareView(hwnd, option);
		}
	}
	return ZoomVideoSDKErrors_Uninitialize;
}

ZoomVideoSDKErrors ZoomVideoSDKMgr::StopShare()
{
	if (video_sdk_obj_)
	{
		IZoomVideoSDKShareHelper* share_helper = video_sdk_obj_->getShareHelper();
		if (share_helper)
		{
			return share_helper->stopShare();
		}
	}
	return ZoomVideoSDKErrors_Uninitialize;
}

ZoomVideoSDKErrors ZoomVideoSDKMgr::SendCommand(IZoomVideoSDKUser* receiver, const zchar_t* strCmd)
{
	if (video_sdk_obj_)
	{
		IZoomVideoSDKCmdChannel* cmd_channel = video_sdk_obj_->getCmdChannel();
		if (cmd_channel)
		{
			return cmd_channel->sendCommand(receiver, strCmd);
		}
	}
	return ZoomVideoSDKErrors_Uninitialize;
}

ZoomVideoSDKErrors ZoomVideoSDKMgr::SendChatToAll(const zchar_t* msgContent)
{
	if (video_sdk_obj_)
	{
		IZoomVideoSDKChatHelper* chat_helper = video_sdk_obj_->getChatHelper();
		if (chat_helper)
		{
			return chat_helper->sendChatToAll(msgContent);
		}
	}
	return ZoomVideoSDKErrors_Uninitialize;
}

const zchar_t* ZoomVideoSDKMgr::GetSessionName() const
{
	if (video_sdk_obj_)
	{
		IZoomVideoSDKSession* session = video_sdk_obj_->getSessionInfo();
		if (session)
		{
			return session->getSessionName();
		}
	}
	return nullptr;
}

int ZoomVideoSDKMgr::GetUserCountInSession()
{
	if (video_sdk_obj_)
	{
		return GetAllUsers().size();
	}
	return 0;
}

bool ZoomVideoSDKMgr::IsInSession()
{
	if (video_sdk_obj_)
	{
		return video_sdk_obj_->isInSession();
	}
	return false;
}

bool ZoomVideoSDKMgr::IsHost()
{
	if (video_sdk_obj_)
	{
		IZoomVideoSDKUser* my_self = GetMySelf();
		if (my_self)
		{
			return my_self->isHost();
		}
	}
	return false;
}

IZoomVideoSDKUser* ZoomVideoSDKMgr::GetSharingUser()
{
	if (video_sdk_obj_)
	{
		vector<IZoomVideoSDKUser*> user_list = GetAllUsers();

		for (size_t i = 0; i < user_list.size(); i++)
		{
			IZoomVideoSDKUser* user = user_list[i];
			if (!user) continue;

			ZoomVideoSDKShareStatus share_status = user->getShareStatus();
			if (share_status == ZoomVideoSDKShareStatus_Start || share_status == ZoomVideoSDKShareStatus_Resume)
			{
				return user;
			}
		}
	}
	return nullptr;
}

IZoomVideoSDKUser* ZoomVideoSDKMgr::GetMySelf()
{
	IZoomVideoSDKUser* my_self = nullptr;
	if (video_sdk_obj_)
	{
		IZoomVideoSDKSession* session = video_sdk_obj_->getSessionInfo();
		if (session)
		{
			my_self = session->getMyself();
		}
	}
	return my_self;
}

IZoomVideoSDKUser* ZoomVideoSDKMgr::GetSessionHost()
{
	if (video_sdk_obj_)
	{
		IZoomVideoSDKSession* session = video_sdk_obj_->getSessionInfo();
		if (session)
		{
			return session->getSessionHost();
		}
	}
	return NULL;
}

IVideoSDKVector<IZoomVideoSDKCameraDevice*>* ZoomVideoSDKMgr::GetCameraList()
{
	if (video_sdk_obj_)
	{
		IZoomVideoSDKVideoHelper* video_helper = video_sdk_obj_->getVideoHelper();
		if (video_helper)
		{
			return video_helper->getCameraList();
		}
	}
	return nullptr;
}

IVideoSDKVector<IZoomVideoSDKSpeakerDevice*>* ZoomVideoSDKMgr::GetSpeakerList()
{
	if (video_sdk_obj_)
	{
		IZoomVideoSDKAudioHelper* audio_helper = video_sdk_obj_->getAudioHelper();
		if (audio_helper)
		{
			return audio_helper->getSpeakerList();
		}
	}
	return nullptr;
}

IVideoSDKVector<IZoomVideoSDKMicDevice*>* ZoomVideoSDKMgr::GetMicList()
{
	if (video_sdk_obj_)
	{
		IZoomVideoSDKAudioHelper* audio_helper = video_sdk_obj_->getAudioHelper();
		if (audio_helper)
		{
			return audio_helper->getMicList();
		}
	}
	return nullptr;
}

vector<IZoomVideoSDKUser*> ZoomVideoSDKMgr::GetAllUsers()
{
	vector<IZoomVideoSDKUser*> vect_all_user;

	if (video_sdk_obj_)
	{
		IZoomVideoSDKSession* session = video_sdk_obj_->getSessionInfo();
		if (session)
		{
			IZoomVideoSDKUser* my_self = session->getMyself();
			if (my_self)
			{
				vect_all_user.push_back(my_self);
			}

			IVideoSDKVector<IZoomVideoSDKUser*>* remote_users = session->getRemoteUsers();

			if (remote_users)
			{
				for (int i = 0; i < remote_users->GetCount(); i++)
				{
					IZoomVideoSDKUser* user = remote_users->GetItem(i);
					if (!user) continue;
					vect_all_user.push_back(user);
				}
			}
		}
	}
	return vect_all_user;
}

ZoomVideoSDKErrors ZoomVideoSDKMgr::GetSessionAudioStatisticInfo(ZoomVideoSDKSessionAudioStatisticInfo& send_info, ZoomVideoSDKSessionAudioStatisticInfo& recv_info)
{
	if (video_sdk_obj_)
	{
		IZoomVideoSDKSession* session = video_sdk_obj_->getSessionInfo();
		if (session)
		{
			session->getSessionAudioStatisticInfo(send_info, recv_info);
		}
	}

	return ZoomVideoSDKErrors_Uninitialize;
}

ZoomVideoSDKErrors ZoomVideoSDKMgr::GetSessionVideoStatisticInfo(ZoomVideoSDKSessionASVStatisticInfo& send_info, ZoomVideoSDKSessionASVStatisticInfo& recv_info)
{
	if (video_sdk_obj_)
	{
		IZoomVideoSDKSession* session = video_sdk_obj_->getSessionInfo();
		if (session)
		{
			session->getSessionVideoStatisticInfo(send_info, recv_info);
		}
	}

	return ZoomVideoSDKErrors_Uninitialize;
}

ZoomVideoSDKErrors ZoomVideoSDKMgr::GetSessionShareStatisticInfo(ZoomVideoSDKSessionASVStatisticInfo& send_info, ZoomVideoSDKSessionASVStatisticInfo& recv_info)
{
	if (video_sdk_obj_)
	{
		IZoomVideoSDKSession* session = video_sdk_obj_->getSessionInfo();
		if (session)
		{
			session->getSessionShareStatisticInfo(send_info, recv_info);
		}
	}

	return ZoomVideoSDKErrors_Uninitialize;
}

std::wstring ZoomVideoSDKMgr::GetErrorStringByErrorCode(ZoomVideoSDKErrors err)
{
	std::wstring sValue;

	switch (err)
	{
	case ZoomVideoSDKErrors_Success:
	{
		sValue = _T("ZoomVideoSDKErrors_Success");
		break;
	}
	case ZoomVideoSDKErrors_Wrong_Usage:
	{
		sValue = _T("ZoomVideoSDKErrors_Wrong_Usage");
		break;
	}
	case ZoomVideoSDKErrors_Internal_Error:
	{
		sValue = _T("ZoomVideoSDKErrors_Internal_Error");
		break;
	}
	case ZoomVideoSDKErrors_Uninitialize:
	{
		sValue = _T("ZoomVideoSDKErrors_Uninitialize");
		break;
	}
	case ZoomVideoSDKErrors_Memory_Error:
	{
		sValue = _T("ZoomVideoSDKErrors_Memory_Error");
		break;
	}
	case ZoomVideoSDKErrors_Load_Module_Error:
	{
		sValue = _T("ZoomVideoSDKErrors_Load_Module_Error");
		break;
	}
	case ZoomVideoSDKErrors_UnLoad_Module_Error:
	{
		sValue = _T("ZoomVideoSDKErrors_UnLoad_Module_Error");
		break;
	}
	case ZoomVideoSDKErrors_Invalid_Parameter:
	{
		sValue = _T("ZoomVideoSDKErrors_Invalid_Parameter");
		break;
	}
	case ZoomVideoSDKErrors_Call_Too_Frequently:
	{
		sValue = _T("ZoomVideoSDKErrors_Call_Too_Frequently");
		break;
	}
	case ZoomVideoSDKErrors_Unknown:
	{
		sValue = _T("ZoomVideoSDKErrors_Unknown");
		break;
	}
	case ZoomVideoSDKErrors_Auth_Error:
	{
		sValue = _T("ZoomVideoSDKErrors_Auth_Error");
		break;
	}
	case ZoomVideoSDKErrors_Auth_Empty_Key_or_Secret:
	{
		sValue = _T("ZoomVideoSDKErrors_Auth_Empty_Key_or_Secret");
		break;
	}
	case ZoomVideoSDKErrors_Auth_Wrong_Key_or_Secret:
	{
		sValue = _T("ZoomVideoSDKErrors_Auth_Wrong_Key_or_Secret");
		break;
	}
	case ZoomVideoSDKErrors_Auth_DoesNot_Support_SDK:
	{
		sValue = _T("ZoomVideoSDKErrors_Auth_DoesNot_Support_SDK");
		break;
	}
	case ZoomVideoSDKErrors_Auth_Disable_SDK:
	{
		sValue = _T("ZoomVideoSDKErrors_Auth_Disable_SDK");
		break;
	}
	case ZoomVideoSDKErrors_SessionModule_Not_Found:
	{
		sValue = _T("ZoomVideoSDKErrors_SessionModule_Not_Found");
		break;
	}
	case ZoomVideoSDKErrors_SessionService_Invaild:
	{
		sValue = _T("ZoomVideoSDKErrors_SessionService_Invaild");
		break;
	}
	case ZoomVideoSDKErrors_Session_Join_Failed:
	{
		sValue = _T("ZoomVideoSDKErrors_Session_Join_Failed");
		break;
	}
	case ZoomVideoSDKErrors_Session_No_Rights:
	{
		sValue = _T("ZoomVideoSDKErrors_Session_No_Rights");
		break;
	}
	case ZoomVideoSDKErrors_Session_Already_In_Progress:
	{
		sValue = _T("ZoomVideoSDKErrors_Session_Already_In_Progress");
		break;
	}
	case ZoomVideoSDKErrors_Session_Dont_Support_SessionType:
	{
		sValue = _T("ZoomVideoSDKErrors_Session_Dont_Support_SessionType");
		break;
	}
	case ZoomVideoSDKErrors_Session_Reconnecting:
	{
		sValue = _T("ZoomVideoSDKErrors_Session_Reconnecting");
		break;
	}
	case ZoomVideoSDKErrors_Session_Disconnecting:
	{
		sValue = _T("ZoomVideoSDKErrors_Session_Disconnecting");
		break;
	}
	case ZoomVideoSDKErrors_Session_Not_Started:
	{
		sValue = _T("ZoomVideoSDKErrors_Session_Not_Started");
		break;
	}
	case ZoomVideoSDKErrors_Session_Need_Password:
	{
		sValue = _T("ZoomVideoSDKErrors_Session_Need_Password");
		break;
	}
	case ZoomVideoSDKErrors_Session_Password_Wrong:
	{
		sValue = _T("ZoomVideoSDKErrors_Session_Password_Wrong");
		break;
	}
	case ZoomVideoSDKErrors_Session_Remote_DB_Error:
	{
		sValue = _T("ZoomVideoSDKErrors_Session_Remote_DB_Error");
		break;
	}
	case ZoomVideoSDKErrors_Session_Invalid_Param:
	{
		sValue = _T("ZoomVideoSDKErrors_Session_Invalid_Param");
		break;
	}
	case ZoomVideoSDKErrors_Session_Audio_Error:
	{
		sValue = _T("ZoomVideoSDKErrors_Session_Audio_Error");
		break;
	}
	case ZoomVideoSDKErrors_Session_Audio_No_Microphone:
	{
		sValue = _T("ZoomVideoSDKErrors_Session_Audio_No_Microphone");
		break;
	}
	case ZoomVideoSDKErrors_Session_Video_Error:
	{
		sValue = _T("ZoomVideoSDKErrors_Session_Video_Error");
		break;
	}
	case ZoomVideoSDKErrors_Session_Video_Device_Error:
	{
		sValue = _T("ZoomVideoSDKErrors_Session_Video_Device_Error");
		break;
	}
	case ZoomVideoSDKErrors_Session_Live_Stream_Error:
	{
		sValue = _T("ZoomVideoSDKErrors_Session_Live_Stream_Error");
		break;
	}
	case ZoomVideoSDKErrors_Session_Phone_Error:
	{
		sValue = _T("ZoomVideoSDKErrors_Session_Phone_Error");
		break;
	}
	case ZoomVideoSDKErrors_RAWDATA_MALLOC_FAILED:
	{
		sValue = _T("ZoomVideoSDKErrors_RAWDATA_MALLOC_FAILED");
		break;
	}
	case ZoomVideoSDKErrors_RAWDATA_NOT_IN_Session:
	{
		sValue = _T("ZoomVideoSDKErrors_RAWDATA_NOT_IN_Session");
		break;
	}
	case ZoomVideoSDKErrors_RAWDATA_NO_LICENSE:
	{
		sValue = _T("ZoomVideoSDKErrors_RAWDATA_NO_LICENSE");
		break;
	}
	case ZoomVideoSDKErrors_RAWDATA_VIDEO_MODULE_NOT_READY:
	{
		sValue = _T("ZoomVideoSDKErrors_RAWDATA_VIDEO_MODULE_NOT_READY");
		break;
	}
	case ZoomVideoSDKErrors_RAWDATA_VIDEO_MODULE_ERROR:
	{
		sValue = _T("ZoomVideoSDKErrors_RAWDATA_VIDEO_MODULE_ERROR");
		break;
	}
	case ZoomVideoSDKErrors_RAWDATA_VIDEO_DEVICE_ERROR:
	{
		sValue = _T("ZoomVideoSDKErrors_RAWDATA_VIDEO_DEVICE_ERROR");
		break;
	}
	case ZoomVideoSDKErrors_RAWDATA_NO_VIDEO_DATA:
	{
		sValue = _T("ZoomVideoSDKErrors_RAWDATA_NO_VIDEO_DATA");
		break;
	}
	case ZoomVideoSDKErrors_RAWDATA_SHARE_MODULE_NOT_READY:
	{
		sValue = _T("ZoomVideoSDKErrors_RAWDATA_SHARE_MODULE_NOT_READY");
		break;
	}
	case ZoomVideoSDKErrors_RAWDATA_SHARE_MODULE_ERROR:
	{
		sValue = _T("ZoomVideoSDKErrors_RAWDATA_SHARE_MODULE_ERROR");
		break;
	}
	case ZoomVideoSDKErrors_RAWDATA_AUDIO_MODULE_NOT_READY:
	{
		sValue = _T("ZoomVideoSDKErrors_RAWDATA_AUDIO_MODULE_NOT_READY");
		break;
	}
	case ZoomVideoSDKErrors_RAWDATA_AUDIO_MODULE_ERROR:
	{
		sValue = _T("ZoomVideoSDKErrors_RAWDATA_AUDIO_MODULE_ERROR");
		break;
	}
	case ZoomVideoSDKErrors_RAWDATA_NO_AUDIO_DATA:
	{
		sValue = _T("ZoomVideoSDKErrors_RAWDATA_NO_AUDIO_DATA");
		break;
	}
	case ZoomVideoSDKErrors_RAWDATA_PREPROCESS_RAWDATA_ERROR:
	{
		sValue = _T("ZoomVideoSDKErrors_RAWDATA_PREPROCESS_RAWDATA_ERROR");
		break;
	}
	case ZoomVideoSDKErrors_RAWDATA_NO_DEVICE_RUNNING:
	{
		sValue = _T("ZoomVideoSDKErrors_RAWDATA_NO_DEVICE_RUNNING");
		break;
	}
	case ZoomVideoSDKErrors_RAWDATA_INIT_DEVICE:
	{
		sValue = _T("ZoomVideoSDKErrors_RAWDATA_INIT_DEVICE");
		break;
	}
	case ZoomVideoSDKErrors_RAWDATA_VIRTUAL_DEVICE:
	{
		sValue = _T("ZoomVideoSDKErrors_RAWDATA_VIRTUAL_DEVICE");
		break;
	}
	case ZoomVideoSDKErrors_RAWDATA_CANNOT_CHANGE_VIRTUAL_DEVICE_IN_PREVIEW:
	{
		sValue = _T("ZoomVideoSDKErrors_RAWDATA_CANNOT_CHANGE_VIRTUAL_DEVICE_IN_PREVIEW");
		break;
	}
	case ZoomVideoSDKErrors_RAWDATA_INTERNAL_ERROR:
	{
		sValue = _T("ZoomVideoSDKErrors_RAWDATA_INTERNAL_ERROR");
		break;
	}
	case ZoomVideoSDKErrors_RAWDATA_SEND_TOO_MUCH_DATA_IN_SINGLE_TIME:
	{
		sValue = _T("ZoomVideoSDKErrors_RAWDATA_SEND_TOO_MUCH_DATA_IN_SINGLE_TIME");
		break;
	}
	case ZoomVideoSDKErrors_RAWDATA_SEND_TOO_FREQUENTLY:
	{
		sValue = _T("ZoomVideoSDKErrors_RAWDATA_SEND_TOO_FREQUENTLY");
		break;
	}
	case ZoomVideoSDKErrors_RAWDATA_VIRTUAL_MIC_IS_TERMINATE:
	{
		sValue = _T("ZoomVideoSDKErrors_RAWDATA_VIRTUAL_MIC_IS_TERMINATE");
		break;
	}
	case ZoomVideoSDKErrors_Session_Share_Error:
	{
		sValue = _T("ZoomVideoSDKErrors_Session_Share_Error");
		break;
	}
	case ZoomVideoSDKErrors_Session_Share_Module_Not_Ready:
	{
		sValue = _T("ZoomVideoSDKErrors_Session_Share_Module_Not_Ready");
		break;
	}
	case ZoomVideoSDKErrors_Session_Share_You_Are_Not_Sharing:
	{
		sValue = _T("ZoomVideoSDKErrors_Session_Share_You_Are_Not_Sharing");
		break;
	}
	case ZoomVideoSDKErrors_Session_Share_Type_Is_Not_Support:
	{
		sValue = _T("ZoomVideoSDKErrors_Session_Share_Type_Is_Not_Support");
		break;
	}
	case ZoomVideoSDKErrors_Session_Share_Internal_Error:
	{
		sValue = _T("ZoomVideoSDKErrors_Session_Share_Internal_Error");
		break;
	}
	default:
	{
		sValue = _T("ZoomVideoSDKErrors_UNKNOWN");
		break;
	}
	}
	return sValue;
}
