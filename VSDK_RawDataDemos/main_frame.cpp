#include "stdafx.h"
//#include "zoom_video_sdk_include_interface.h"
#include "main_frame.h"
#include "videosdk_demo_mgr.h"

//custom message
#define WM_ICON_NOTIFY WM_USER + 1

#define CPATION_HEIGHT 100
#define BOTTOM_BAR_HEIGHT 48
#define HIDE_BOTTOM_BAR_TIMER 1000
#define HIDE_ERROR_TIP_TIMER 1001

#define VIDEO_CANTAINER_DEFAULT_INSET 20,20,20,20
#define DEFAULT_ROW_GAP 20
#define DEFAULT_COLUMN_GAP 20
#define BOTTOM_BAR_MARGIN 20
#define CHAT_CONTENT_LEFT_MARGIN 10
#define SESSION_SETTING_WND_GAP 10
#define TURN_PAGE_MARIGN 5

#define LOWERTHIRDS_PADDING_LEFT 8
#define LOWERTHIRDS_PADDING_TOP 42

#define LOWERTHIRDS_WND_FULL_HEIGHT 62
#define LOWERTHIRDS_WND_LITE_HEIGHT 42

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{


	UninitVideoSDK();
}

void CMainFrame::DestroyAllOwnedWnd()
{
	if (join_session_wnd_)
	{
		/*if (::IsWindow(join_session_wnd_->GetHWND()))
		{
			DestroyWindow(join_session_wnd_->GetHWND());
		}*/
		
	}

	if (title_bar_wnd_)
	{
	/*	if (::IsWindow(title_bar_wnd_->GetHWND()))
		{
			DestroyWindow(title_bar_wnd_->GetHWND());
		}*/
		
	}

	if (bottom_bar_wnd_)
	{
	/*	if (::IsWindow(bottom_bar_wnd_->GetHWND()))
		{
			DestroyWindow(bottom_bar_wnd_->GetHWND());
		}
		DestroyWindow(bottom_bar_wnd_->GetHWND());*/
	
	}

	if (share_tool_bar_wnd_)
	{
		/*if (::IsWindow(share_tool_bar_wnd_->GetHWND()))
		{
			DestroyWindow(share_tool_bar_wnd_->GetHWND());
		}*/
	
	}

	//for (auto iter = cmd_channel_wnd_map_.begin(); iter != cmd_channel_wnd_map_.end(); iter++)
	//{
	//	IParseChannelCmdWnd* cmd_channel_wnd = iter->second;
	//	if (cmd_channel_wnd)
	//	{
	//		if (::IsWindow(cmd_channel_wnd->GetHWND()))
	//			DestroyWindow(cmd_channel_wnd->GetHWND());

	//		cmd_channel_wnd = nullptr;
	//	}
	//}

}


CMainFrame& CMainFrame::GetInstance()
{
	static CMainFrame s_MainFramInstance;
	return s_MainFramInstance;
}

void CMainFrame::InitVideoSDK()
{
	ZoomVideoSDKInitParams init_params;
	init_params.domain = _T("https://zoom.us");
	init_params.enableLog = true;
	init_params.logFilePrefix = _T("zoom_win_video_demo");
	init_params.videoRawDataMemoryMode = ZoomVideoSDKRawDataMemoryModeHeap;
	init_params.shareRawDataMemoryMode = ZoomVideoSDKRawDataMemoryModeHeap;
	init_params.audioRawDataMemoryMode = ZoomVideoSDKRawDataMemoryModeHeap;
	init_params.enableIndirectRawdata = false;

	ZoomVideoSDKMgr::GetInst().Init(this, init_params);
}

void CMainFrame::UninitVideoSDK()
{
	ZoomVideoSDKMgr::GetInst().UnInit();
}

void CMainFrame::InitControls()
{

}

//void CMainFrame::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	UpdateAllOwnedWndPos();
//
//	ShowFeedBackLaunchWnd(false);
//	ShowFeedBackSubmitWnd(false);
//}

//void CMainFrame::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	if (wParam == HIDE_BOTTOM_BAR_TIMER)
//	{
//		KillBottomBarHideTimer();
//		ShowBottomBarWnd(false);
//		ShowEmojiBarWnd(false);
//	}
//	else if (wParam == HIDE_ERROR_TIP_TIMER)
//	{
//		KillTimer(m_hWnd, HIDE_ERROR_TIP_TIMER);
//		ShowMessageTipWnd(false);
//	}
//}

//void CMainFrame::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	if (wParam == 1)
//	{
//		if (title_bar_wnd_ && ::IsWindow(title_bar_wnd_->GetHWND()))
//		{
//			title_bar_wnd_->ShowWindow(true);
//		}
//	}
//	else
//	{
//		HideAllOwnedWnd();
//	}
//}

//void CMainFrame::PopupAppWindow()
//{
//	if (join_session_wnd_ && ::IsWindow(join_session_wnd_->GetHWND()))
//	{
//		if (::IsWindowVisible(join_session_wnd_->GetHWND()) && !::IsWindowVisible(m_hWnd)) //current is in join session window
//		{
//			::SetForegroundWindow(join_session_wnd_->GetHWND());
//		}
//		else
//		{
//			if (::IsIconic(m_hWnd)) //if minimize
//			{
//				::ShowWindow(CMainFrame::GetInstance().GetHWND(), SW_RESTORE);
//			}
//			::SetForegroundWindow(m_hWnd);
//		}
//	}
//}

//void CMainFrame::OnCopyData(UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	COPYDATASTRUCT Copy_data = *(COPYDATASTRUCT*)lParam;
//	if (Copy_data.dwData == POPUP_EXIST_APP)
//	{
//		PopupAppWindow();
//	}
//}


//void CMainFrame::OnIconNotify(UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	if (lParam == WM_LBUTTONDOWN)
//	{
//		PopupAppWindow();
//	}
//}




void CMainFrame::OnLeaveSessionUIUpdate()
{

	StopShare();

	//ShowWindow(false);
}

void CMainFrame::OnJoinMettingJoinFailed(int error_code)
{
	/*if (join_session_wnd_)
	{
		join_session_wnd_->OnJoinMettingJoinFailed(error_code);
	}*/
}

void CMainFrame::OnInvalidParamterNotification()
{
	/*if (join_session_wnd_)
		join_session_wnd_->OnInvalidParamterNotification();*/
}

void CMainFrame::OnMeetingDisconnecting()
{/*
	if (video_show_mgr_)
	{
		is_show_gallery_ = false;
		video_show_mgr_->ContainerInit();
	}*/

	StartPreview();
}

//void CMainFrame::SetNotifyIconData(NOTIFYICONDATA& nid)
//{
//	SecureZeroMemory(&nid, sizeof(NOTIFYICONDATA));
//	nid.cbSize = sizeof(NOTIFYICONDATA);
//	nid.hWnd = m_hWnd;
//	nid.uID = 1;
//	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
//	nid.hIcon = ::LoadIcon(m_PaintManager.GetInstance(), (LPCWSTR)IDI_ICON_MAIN);
//	lstrcpyn(nid.szTip, APP_NAME, sizeof(nid.szTip) / sizeof(TCHAR));
//	nid.uCallbackMessage = WM_ICON_NOTIFY;
//}

void CMainFrame::AddTheTray()
{
	//NOTIFYICONDATA	nid;
	//SetNotifyIconData(nid);

	//::Shell_NotifyIcon(NIM_ADD, &nid);
}

void CMainFrame::DeleteTheTray()
{
	/*NOTIFYICONDATA	nid;
	SetNotifyIconData(nid);

	::Shell_NotifyIcon(NIM_DELETE, &nid);*/
}





void CMainFrame::StartPreview()
{
	
}


//void CMainFrame::ShowSessionInfoWnd(RECT rc)
//{
//	if (session_infos_wnd_ && ::IsWindow(session_infos_wnd_->GetHWND()))
//	{
//		session_infos_wnd_->ShowWindow(true);
//		session_infos_wnd_->UpdateSessionInfoUI();
//
//		RECT rcClient;
//		GetClientRect(session_infos_wnd_->GetHWND(), &rcClient);
//
//		int iXpos = 0, iYpos = 0;
//		iXpos = rc.left + (rc.right - rc.left) / 2 - (rcClient.right - rcClient.left) / 2;
//		iYpos = rc.bottom + 10;
//
//		::SetWindowPos(session_infos_wnd_->GetHWND(), NULL, iXpos, iYpos, 0, 0, SWP_NOSIZE);
//	}
//}
//
//void CMainFrame::ShowSessionSettingWnd(bool show, RECT rc)
//{
//	if (session_setting_wnd_ && ::IsWindow(session_setting_wnd_->GetHWND()))
//	{
//		if (show && !::IsWindowVisible(session_setting_wnd_->GetHWND()))
//		{
//			ShowLeaveSessionTipsWnd(false);
//			bool is_in_session = ZoomVideoSDKMgr::GetInst().IsInSession();
//
//			RECT rcClient;
//			GetClientRect(session_setting_wnd_->GetHWND(), &rcClient);
//
//			int iXpos = 0, iYpos = 0;
//			if (!is_in_session)
//			{
//				iXpos = rc.left + ((rc.right - rc.left) - (rcClient.right - rcClient.left)) / 2;
//				iYpos = rc.top + ((rc.bottom - rc.top) - (rcClient.bottom - rcClient.top)) / 2;;
//			}
//			else
//			{
//				iXpos = rc.right - SESSION_SETTING_WND_GAP - (rcClient.right - rcClient.left);
//				iYpos = rc.top - SESSION_SETTING_WND_GAP - (rcClient.bottom - rcClient.top);
//			}
//
//			::SetWindowPos(session_setting_wnd_->GetHWND(), NULL, iXpos, iYpos, 0, 0, SWP_NOSIZE);
//
//			session_setting_wnd_->ShowWindow(true);
//			session_setting_wnd_->UpdateSettingsInfoUI();
//			session_setting_wnd_->UpdateControl();
//		}
//		else
//		{
//			session_setting_wnd_->ShowWindow(false);
//		}
//	}
//}
//
//void CMainFrame::ShowLowerThirdsSettingWndUI(bool show, RECT rc)
//{
//	if (lower_thirds_setting_wnd_ && ::IsWindow(lower_thirds_setting_wnd_->GetHWND()))
//	{
//		if (show && !::IsWindowVisible(lower_thirds_setting_wnd_->GetHWND()))
//		{
//			RECT rcClient;
//			GetClientRect(lower_thirds_setting_wnd_->GetHWND(), &rcClient);
//
//			int iXpos = 0, iYpos = 0;
//
//			iXpos = (rc.left + (rc.right - rc.left) / 2) - (rcClient.right - rcClient.left) / 2;
//			iYpos = (rc.top + (rc.bottom - rc.top) / 2) - (rcClient.bottom - rcClient.top) / 2;
//
//			::SetWindowPos(lower_thirds_setting_wnd_->GetHWND(), NULL, iXpos, iYpos, 0, 0, SWP_NOSIZE);
//
//			lower_thirds_setting_wnd_->ShowWindow(true);
//			lower_thirds_setting_wnd_->UpdateControl();
//		}
//		else
//		{
//			session_setting_wnd_->ShowWindow(false);
//		}
//	}
//}
//
//void CMainFrame::ShowShareSelectAppsWnd(bool show, RECT rc)
//{
//	if (share_select_apps_wnd_ && ::IsWindow(share_select_apps_wnd_->GetHWND()))
//	{
//		if (show)
//		{
//			share_select_apps_wnd_->ShowWindow(true);
//			share_select_apps_wnd_->UpdateAllApps();
//
//			RECT rcClient;
//			GetClientRect(share_select_apps_wnd_->GetHWND(), &rcClient);
//
//			int iXpos = 0, iYpos = 0;
//			iXpos = rc.right + 250 - (rcClient.right - rcClient.left);
//			iYpos = rc.top - SESSION_SETTING_WND_GAP - (rcClient.bottom - rcClient.top);
//
//			::SetWindowPos(share_select_apps_wnd_->GetHWND(), NULL, iXpos, iYpos, 0, 0, SWP_NOSIZE);
//		}
//		else
//		{
//			share_select_apps_wnd_->ShowWindow(false);
//		}
//	}
//}

//void CMainFrame::ShowEmojiBarWnd(bool show, RECT rc)
//{
//	if (reaction_bar_wnd_ && ::IsWindow(reaction_bar_wnd_->GetHWND()))
//	{
//		if (show)
//		{
//			RECT rcClient;
//			GetClientRect(reaction_bar_wnd_->GetHWND(), &rcClient);
//
//			int iXpos = 0, iYpos = 0;
//			iXpos = rc.right + 250 - (rcClient.right - rcClient.left);
//			iYpos = rc.top - SESSION_SETTING_WND_GAP - (rcClient.bottom - rcClient.top);
//
//			::SetWindowPos(reaction_bar_wnd_->GetHWND(), NULL, iXpos, iYpos, 0, 0, SWP_NOSIZE);
//			reaction_bar_wnd_->ShowWindow(true);
//		}
//		else
//		{
//			reaction_bar_wnd_->ShowWindow(false);
//		}
//	}
//}
//
//void CMainFrame::ShowMorePopWnd(bool show, RECT rc)
//{
//	if (more_pop_wnd_ && ::IsWindow(more_pop_wnd_->GetHWND()))
//	{
//		if (show)
//		{
//			RECT rcClient;
//			GetClientRect(more_pop_wnd_->GetHWND(), &rcClient);
//
//			int iXpos = 0, iYpos = 0;
//			iXpos = rc.left;
//			iYpos = rc.top - SESSION_SETTING_WND_GAP - (rcClient.bottom - rcClient.top);
//
//			::SetWindowPos(more_pop_wnd_->GetHWND(), NULL, iXpos, iYpos, 0, 0, SWP_NOSIZE);
//			more_pop_wnd_->ShowWindow(true);
//		}
//		else
//		{
//			more_pop_wnd_->ShowWindow(false);
//		}
//	}
//}

//void CMainFrame::ShowMessageTipWnd(bool show, wstring error_text)
//{
//	if (message_tip_wnd_ && ::IsWindow(message_tip_wnd_->GetHWND()))
//	{
//		if (show)
//		{
//			message_tip_wnd_->ShowWindow(true);
//			SetTimer(m_hWnd, HIDE_ERROR_TIP_TIMER, 5 * 1000, nullptr);
//			message_tip_wnd_->SetErrorTipText(error_text);
//
//			UpdateMessageTipWndPos();
//		}
//		else
//		{
//			message_tip_wnd_->ShowWindow(false);
//		}
//	}
//}


void CMainFrame::StopShare()
{
	
	ZoomVideoSDKErrors err = ZoomVideoSDKMgr::GetInst().StopShare();
}

void CMainFrame::SendChatToAll(const zchar_t* msgContent)
{
	ZoomVideoSDKMgr::GetInst().SendChatToAll(msgContent);
}

/*void CMainFrame::UpdateRenderModeLowerThird(IZoomVideoSDKUser* pUser, wstring lower_third_name, wstring lower_third_description, LowerThirdColorInfo& color_info)
{
	if (!video_show_mgr_)
		return;

	video_show_mgr_->OnUpdateRenderModeLowerThird(pUser, lower_third_name, lower_third_description, color_info);
}*/


void CMainFrame::SetCommandChannelConnect(bool is_connect)
{
	is_command_channel_connected_ = is_connect;
}

bool CMainFrame::IsCommandChannelConnect()
{
	return is_command_channel_connected_;
}

void CMainFrame::JoinSession(ZoomVideoSDKSessionContext& session_context)
{
	IZoomVideoSDKSession* pSession = ZoomVideoSDKMgr::GetInst().JoinSession(session_context);
	if (pSession)
	{
	/*	if (nullptr == renderer_)
		{
			renderer_ = CreateZoomVideoSDKRendererObj();
			if (renderer_)
			{
				renderer_->SetParentHandle((void*)m_hWnd);
				renderer_->SetVideoItemBkColor(RendererColor(58, 59, 59));
				renderer_->SetRendererBkColor(RendererColor(35, 36, 36, 255));
			}
		}*/

		if (!video_show_mgr_)
		{
			/*RECT rect_main = { 0 };
			GetClientRect(m_hWnd, &rect_main);

			RendererRect rect_renderer = { rect_main.left,rect_main.top,rect_main.right - rect_main.left,rect_main.bottom - rect_main.top };

			video_show_mgr_ = new VideoShowMgr(renderer_, rect_renderer);*/
		}

		//VideoContainerParam param;

		//param.rect_inset = { 0,0,0,0 };
		//param.row_gap = DEFAULT_ROW_GAP;
		//param.column_gap = DEFAULT_COLUMN_GAP;
		//param.layout_type = kVideoRenderLayoutType1x6;
		//param.mode = render_mode_;

		//SwitchVideoRenderLayoutType(param);

		if (video_show_mgr_)
		{
		/*	is_show_gallery_ = false;
			video_show_mgr_->ContainerInit();*/
		}

		StartPreview();

		//join_session_wnd_->ShowWindow(false);
		//ShowWindow(true);
	}
}

void CMainFrame::LeaveSession(bool end)
{
	ZoomVideoSDKErrors err = ZoomVideoSDKMgr::GetInst().LeaveSession(end);
}

//void CMainFrame::HanldeRenderModeChanged(RendererMode mode)
//{
//	render_mode_ = mode;
//	if (video_show_mgr_)
//		video_show_mgr_->SwitchRenderMode(mode);
//}

//void CMainFrame::SwitchVideoRenderLayoutType(VideoContainerParam& param)
//{
//	if (video_show_mgr_)
//	{
//		video_show_mgr_->SwitchVideoRenderLayoutType(param);
//	}
//}

//void CMainFrame::RemoveUserReaction(IZoomVideoSDKUser* pUser)
//{
//	if (video_show_mgr_)
//	{
//		video_show_mgr_->OnUserEmojiStatusChanged(pUser, 0);
//	}
//}


void CMainFrame::onSessionJoin()
{
	//if (join_session_wnd_ && ::IsWindow(join_session_wnd_->GetHWND()))
	//{
	//	join_session_wnd_->ShowWindow(false);
	//}

	//ShowWindow(true);

	//ShowBottomBarWnd(false);

	//if (bottom_bar_wnd_)
	//	bottom_bar_wnd_->OnSessionJoin();

	//if (lower_thirds_info_wnd_)
	//	lower_thirds_info_wnd_->OnSessionJoin();

	//bool isHost = ZoomVideoSDKMgr::GetInst().IsHost();
	//if (feedback_launch_wnd_ && isHost)
	//	feedback_launch_wnd_->OnSessionJoin();

	//const zchar_t* szSessionName = ZoomVideoSDKMgr::GetInst().GetSessionName();
	//if (title_bar_wnd_)
	//	title_bar_wnd_->UpdateSessionName(szSessionName);

	//UpdateParticipantNum();

	//if (session_setting_wnd_ && session_setting_wnd_->IsShowLowerThirdsInfoWnd())
	//{
	//	ShowLowerThirdsInfoWnd(true);
	//}

	//if (session_infos_wnd_)
	//{
	//	session_infos_wnd_->UpdateSessionInfoUI();
	//}
}

void CMainFrame::onSessionLeave()
{
	if (video_show_mgr_)
	{
		//video_show_mgr_->OnSessionLeave();
	}

	//update ui
	//OnLeaveSessionUIUpdate();
}

void CMainFrame::onError(ZoomVideoSDKErrors errorCode, int detailErrorCode)
{
	if (errorCode == ZoomVideoSDKErrors_Session_Disconnecting || errorCode == ZoomVideoSDKErrors_Session_Reconnecting)
	{
		OnMeetingDisconnecting();
	}
	else if (errorCode == ZoomVideoSDKErrors_Invalid_Parameter)
	{
		OnInvalidParamterNotification();
	}
	else
	{
		OnJoinMettingJoinFailed(errorCode);
	}
}

void CMainFrame::onUserJoin(IZoomVideoSDKUserHelper* pUserHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{
	if (video_show_mgr_)
	{
		//video_show_mgr_->OnUserJoin(userList);
	}

	//update ui
	//UpdateParticipantNum();
	//if (session_infos_wnd_)
	//{
	//	session_infos_wnd_->UpdateSessionInfoUI();
	//}


}

void CMainFrame::onUserLeave(IZoomVideoSDKUserHelper* pUserHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{
	//if (video_show_mgr_)
	//{
	//	video_show_mgr_->OnUserLeave(userList);
	//}

	//if (lower_thirds_info_wnd_)
	//{
	//	lower_thirds_info_wnd_->OnUserLeave(userList);
	//}
	////update ui
	//if (session_infos_wnd_)
	//{
	//	session_infos_wnd_->UpdateSessionInfoUI();
	//}
	//UpdateParticipantNum();
	//HideOrShowTurnPageWnd();
}

//bool CMainFrame::JudgeIsIncludeMySelf(IVideoSDKVector<IZoomVideoSDKUser*>* userList)
//{
//	IZoomVideoSDKUser* myself = ZoomVideoSDKMgr::GetInst().GetMySelf();
//
//	bool is_inculde_myself = false;
//	int count = userList->GetCount();
//	for (int i = 0; i < count; i++)
//	{
//		IZoomVideoSDKUser* user = userList->GetItem(i);
//		if (!user) continue;
//
//		if (user == myself)
//		{
//			is_inculde_myself = true;
//			break;
//		}
//	}
//
//	return is_inculde_myself;
//}

void CMainFrame::onUserVideoStatusChanged(IZoomVideoSDKVideoHelper* pVideoHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{
	//if (!pVideoHelper || !userList || !video_show_mgr_)
	//	return;

	//if (JudgeIsIncludeMySelf(userList))
	//{
	//	//update ui
	//	bool is_video_on = false;
	//	if (ZoomVideoSDKMgr::GetInst().IsMyselfVideoOn())
	//		is_video_on = true;

	//	if (bottom_bar_wnd_)
	//	{
	//		bottom_bar_wnd_->UpdateUIForVideoStatus(is_video_on);
	//	}

	//	if (share_tool_bar_wnd_)
	//	{
	//		share_tool_bar_wnd_->UpdateUIForVideoStatus(is_video_on);
	//	}

	//	if (!video_show_mgr_)
	//		return;

	//	if (video_show_mgr_ && is_video_on)
	//	{
	//		IZoomVideoSDKUser* myself = ZoomVideoSDKMgr::GetInst().GetMySelf();
	//		video_show_mgr_->Subscribe(myself, RAW_DATA_TYPE_VIDEO);
	//	}
	//}
}

void CMainFrame::onUserAudioStatusChanged(IZoomVideoSDKAudioHelper* pAudioHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{
	//if (!pAudioHelper || !userList || !video_show_mgr_)
	//	return;

	//video_show_mgr_->OnUserAudioStatusChanged(pAudioHelper, userList);

	////update ui
	//if (JudgeIsIncludeMySelf(userList))
	//{
	//	bool is_audio_on = false;
	//	if (!ZoomVideoSDKMgr::GetInst().IsMyselfAudioMuted())
	//		is_audio_on = true;

	//	if (is_audio_on)
	//	{
	//		ShowMessageTipWnd(true, _T("You are unmuted now"));
	//	}
	//	else
	//	{
	//		ShowMessageTipWnd(true, _T("You are muted now"));
	//	}

	//	if (bottom_bar_wnd_)
	//	{
	//		bottom_bar_wnd_->UpdateUIForAudioStatus(is_audio_on);
	//	}

	//	if (share_tool_bar_wnd_)
	//	{
	//		share_tool_bar_wnd_->UpdateUIForAudioStatus(is_audio_on);
	//	}
	//}
}

void CMainFrame::onUserShareStatusChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, ZoomVideoSDKShareStatus status, ZoomVideoSDKShareType type)
{
	//if (!pUser)
	//	return;

	//if (video_show_mgr_)
	//{
	//	video_show_mgr_->OnUserShareStatusChanged(pShareHelper, pUser, status, type);
	//}

	////update ui
	//wstring user_name = pUser->getUserName();
	//wstring str_message;
	//ZoomVideoSDKShareStatus share_status = pUser->getShareStatus();
	//if (share_status == ZoomVideoSDKShareStatus_Start || share_status == ZoomVideoSDKShareStatus_Resume)
	//{
	//	str_message = user_name + _T(" is sharing");
	//}
	//else
	//{
	//	str_message = user_name + _T(" sharing stopped");
	//}
	//ShowMessageTipWnd(true, str_message.c_str());
}

void CMainFrame::onLiveStreamStatusChanged(IZoomVideoSDKLiveStreamHelper* pLiveStreamHelper, ZoomVideoSDKLiveStreamStatus status)
{

}

void CMainFrame::onChatNewMessageNotify(IZoomVideoSDKChatHelper* pChatHelper, IZoomVideoSDKChatMessage* messageItem)
{
	if (!messageItem)
		return;
	IZoomVideoSDKUser* send_user = messageItem->getSendUser();
	if (send_user)
	{
		const zchar_t* user_name = send_user->getUserName();
		const zchar_t* content = messageItem->getContent();

	
	}
}

void CMainFrame::onUserHostChanged(IZoomVideoSDKUserHelper* pUserHelper, IZoomVideoSDKUser* pUser)
{
	/*if (pUser)
	{
		const zchar_t* szUserName = pUser->getUserName();
		ZoomVideoSDKAudioStatus audioStatus = pUser->getAudioStatus();
		if (audioStatus.isMuted)
		{

		}

		ZoomVideoSDKVideoStatus videoStatus = pUser->getVideoStatus();
	}

	if (bottom_bar_wnd_)
		bottom_bar_wnd_->OnSessionJoin();*/
}

void CMainFrame::onUserActiveAudioChanged(IZoomVideoSDKAudioHelper* pAudioHelper, IVideoSDKVector<IZoomVideoSDKUser*>* list)
{

}

void CMainFrame::onSessionNeedPassword(IZoomVideoSDKPasswordHandler* handler)
{
	/*if (!handler)
		return;

	if (join_session_wnd_)
		join_session_wnd_->OnJoinPasswordWrong();

	handler->leaveSessionIgnorePassword();*/
}

void CMainFrame::onSessionPasswordWrong(IZoomVideoSDKPasswordHandler* handler)
{
	//if (!handler)
	//	return;

	//if (join_session_wnd_)
	//	join_session_wnd_->OnJoinPasswordWrong();

	//handler->leaveSessionIgnorePassword();
}

void CMainFrame::onMixedAudioRawDataReceived(AudioRawData* data_)
{
}

void CMainFrame::onOneWayAudioRawDataReceived(AudioRawData* data_, IZoomVideoSDKUser* pUser)
{

}

void CMainFrame::onSharedAudioRawDataReceived(AudioRawData* data_)
{

}

void CMainFrame::onUserManagerChanged(IZoomVideoSDKUser* pUser)
{

}

void CMainFrame::onUserNameChanged(IZoomVideoSDKUser* pUser)
{

}

void CMainFrame::onCameraControlRequestResult(IZoomVideoSDKUser* pUser, bool isApproved)
{

}

void CMainFrame::onCameraControlRequestReceived(IZoomVideoSDKUser* pUser, ZoomVideoSDKCameraControlRequestType requestType, IZoomVideoSDKCameraControlRequestHandler* pCameraControlRequestHandler)
{
}

void CMainFrame::onCommandReceived(IZoomVideoSDKUser* pSender, const zchar_t* strCmd)
{
	/*wstring wstrCmd(strCmd);
	if (wstrCmd.empty())
		return;

	std::vector<std::wstring> cmd_elems;
	split(wstrCmd, _T('|'), cmd_elems);

	if (cmd_elems.size() == 0)
		return;

	CmdChannelType nCmdType = (CmdChannelType)_ttoi(cmd_elems[0].c_str());

	IParseChannelCmdWnd* channelCmdWnd = NULL;
	if (cmd_channel_wnd_map_.count(nCmdType) > 0)
		channelCmdWnd = cmd_channel_wnd_map_[nCmdType];

	if (channelCmdWnd)
		channelCmdWnd->OnParseChannelCmd(pSender, cmd_elems);*/
}

void CMainFrame::onCommandChannelConnectResult(bool isSuccess)
{
	this->SetCommandChannelConnect(isSuccess);

	if (isSuccess && lower_thirds_setting_wnd_)
	{
		std::wstring username, description, color, isenable;
		
		if (username.empty())
			return;

	//	lower_thirds_setting_wnd_->SendLowerThirdSettingInfo(username, description, color);
	}
}

void CMainFrame::onCloudRecordingStatus(RecordingStatus status, IZoomVideoSDKRecordingConsentHandler* pHandler)
{

}

void CMainFrame::onHostAskUnmute()
{
//	CMainFrame::GetInstance().ShowMessageTipWnd(true, _T("The host would like you to unmute"));
}

void CMainFrame::onInviteByPhoneStatus(PhoneStatus status, PhoneFailedReason reason)
{

}

void CMainFrame::onMultiCameraStreamStatusChanged(ZoomVideoSDKMultiCameraStreamStatus status, IZoomVideoSDKUser* pUser, IZoomVideoSDKRawDataPipe* pVideoPipe)
{
}

void CMainFrame::onMicSpeakerVolumeChanged(unsigned int micVolume, unsigned int speakerVolume)
{

}

void CMainFrame::onAudioDeviceStatusChanged(ZoomVideoSDKAudioDeviceType type, ZoomVideoSDKAudioDeviceStatus status)
{

}

void CMainFrame::onTestMicStatusChanged(ZoomVideoSDK_TESTMIC_STATUS status)
{

}

void CMainFrame::onSelectedAudioDeviceChanged()
{

}

void CMainFrame::onLiveTranscriptionStatus(ZoomVideoSDKLiveTranscriptionStatus status)
{

}

void CMainFrame::onLiveTranscriptionMsgReceived(const zchar_t* ltMsg, IZoomVideoSDKUser* pUser, ZoomVideoSDKLiveTranscriptionOperationType type)
{

}

void CMainFrame::onLiveTranscriptionMsgError(ILiveTranscriptionLanguage* spokenLanguage, ILiveTranscriptionLanguage* transcriptLanguage)
{

}