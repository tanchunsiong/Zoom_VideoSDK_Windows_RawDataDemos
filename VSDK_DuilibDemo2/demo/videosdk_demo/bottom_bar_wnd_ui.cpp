#include "stdafx.h"
#include "bottom_bar_wnd_ui.h"
#include "main_frame.h"
#include "videosdk_demo_mgr.h"
#include "chat_send_edit_ui.h"
#include "error_tip_dlg.h"

#define NUMBER_IMAGE_RED	_T("res = 'NUMBER_BKIMAGE_RED' restype = 'ZPIMGRES'")
#define NUMBER_IMAGE_BLACK	_T("res = 'NUMBER_BKIMAGE_BLACK' restype = 'ZPIMGRES'")

CBottomBarWndUI::CBottomBarWndUI()
{
}

CBottomBarWndUI::~CBottomBarWndUI()
{

}

CControlUI* CBottomBarWndUI::CreateControl(LPCTSTR pstrClass)
{
	if (_tcscmp(pstrClass, _T("ChatRichEdit")) == 0)
	{
		return new ChatSendEditUI();
	}

	return nullptr;
}

void CBottomBarWndUI::InitWindow()
{
	InitControls();
}

CDuiString CBottomBarWndUI::GetSkinFolder()
{
	return _T("");
}

CDuiString CBottomBarWndUI::GetSkinFile()
{
	return to_wstring(IDXML_BOTTOM_BAR_WND_UI).c_str();
}

UILIB_RESOURCETYPE CBottomBarWndUI::GetResourceType() const
{
	return UILIB_RESOURCE;
}

void CBottomBarWndUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btn_muted_audio"))
		{
			DoMutedAudioBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_unmuted_audio"))
		{
			DoUnmutedAudioBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_muted_video"))
		{
			DoMutedVideoBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_unmuted_video"))
		{
			DoUnmutedVideoBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_end"))
		{
			DoEndSessionBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_participant"))
		{
			DoParticipantBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_share"))
		{
			
		}
		else if (msg.pSender->GetName() == _T("btn_emoji"))
		{
			
		}
		else if (msg.pSender->GetName() == _T("btn_more"))
		{
		
		}
		else if (msg.pSender->GetName() == _T("btn_settings"))
		{
			DoSettingBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_chat_send"))
		{
			DoSendChatBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_feedback"))
		{
		
		}
	}

	__super::Notify(msg);
}

LRESULT CBottomBarWndUI::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::OnSetFocus(uMsg, wParam, lParam, bHandled);
}

LRESULT CBottomBarWndUI::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::OnKillFocus(uMsg, wParam, lParam, bHandled);
}

LRESULT CBottomBarWndUI::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

LRESULT CBottomBarWndUI::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//CMainFrame::GetInstance().ShowBottomBarWnd(true);
	return __super::OnLButtonUp(uMsg, wParam, lParam, bHandled);
}

void CBottomBarWndUI::InitControls()
{
	btn_muted_audio_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_muted_audio")));
	btn_unmuted_audio_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_unmuted_audio")));
	btn_muted_video_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_muted_video")));
	btn_unmuted_video_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_unmuted_video")));
	btn_participant_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_participant")));
	btn_setting_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_settings")));
	btn_share_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_share")));
	btn_emoji_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_emoji")));
	btn_more_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_more")));
	btn_feedback_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_feedback")));
	edit_chat_ = static_cast<ChatSendEditUI*>(m_PaintManager.FindControl(_T("chat_rich_edit")));
	label_participants_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_participant_num")));
	label_feedback_num_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_feedback_num")));

	feedback_layout_ = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("feedback_layout")));
}

void CBottomBarWndUI::OnSessionJoin()
{
	if (!btn_more_ || !feedback_layout_)
		return;

	bool isHost = ZoomVideoSDKMgr::GetInst().IsHost();
	
	btn_more_->SetVisible(!isHost);
	feedback_layout_->SetVisible(isHost);
}

void CBottomBarWndUI::UpdateParticipantNum()
{
	if (nullptr == label_participants_)
		return;

	int user_count = ZoomVideoSDKMgr::GetInst().GetUserCountInSession();

	if (user_count >= 0)
		label_participants_->SetVisible(true);
	else
		label_participants_->SetVisible(false);

	TCHAR szNum[64] = { 0 };
	wsprintf(szNum, _T("%d"), user_count);   
	label_participants_->SetText(szNum);
}

void CBottomBarWndUI::UpdateFeedBackNum(int feedback_num)
{
	if (!label_feedback_num_)
		return;

	CDuiString strFeedBackNum;
	strFeedBackNum.Format(_T("%d"), feedback_num);
	label_feedback_num_->SetText(strFeedBackNum);

	label_feedback_num_->SetBkImage(feedback_num == 0 ? NUMBER_IMAGE_BLACK : NUMBER_IMAGE_RED);
}

RECT CBottomBarWndUI::GetButtonPos(CButtonUI* btn) const
{
	RECT rc = { 0 };
	if (!btn)	return rc;

	rc = btn->GetPos();
	POINT ptLeftTop = { rc.left,rc.top };
	POINT ptRightBottom = { rc.right,rc.bottom };
	ClientToScreen(m_hWnd, &ptLeftTop);
	ClientToScreen(m_hWnd, &ptRightBottom);
	return { ptLeftTop.x,ptLeftTop.y,ptRightBottom.x,ptRightBottom.y };
}

void CBottomBarWndUI::DoMutedAudioBtnClick()
{
	ZoomVideoSDKErrors err = ZoomVideoSDKMgr::GetInst().UnmuteAudio();
	if (err == ZoomVideoSDKErrors_Session_Audio_No_Microphone)
	{
		CMainFrame::GetInstance().ShowMessageTipWnd(true, _T("You have no microphone."));
	}
}

void CBottomBarWndUI::DoUnmutedAudioBtnClick()
{
	ZoomVideoSDKMgr::GetInst().MuteAudio();
}

void CBottomBarWndUI::UpdateUIForAudioStatus(bool is_audio_on)
{
	if (!btn_muted_audio_ || !btn_unmuted_audio_)
		return;

	btn_muted_audio_->SetVisible(!is_audio_on);
	btn_unmuted_audio_->SetVisible(is_audio_on);
}
void CBottomBarWndUI::DoMutedVideoBtnClick()
{
	ZoomVideoSDKErrors err = ZoomVideoSDKMgr::GetInst().UnmuteVideo();
	if (err == ZoomVideoSDKErrors_Session_Video_Error)
	{
		CMainFrame::GetInstance().ShowMessageTipWnd(true, _T("Please select another video camera in settings."));
	}
	else if (err == ZoomVideoSDKErrors_Session_Video_Device_Error)
	{
		CMainFrame::GetInstance().ShowMessageTipWnd(true, _T("You have no camera."));
	}
	else if (err == ZoomVideoSDKErrors_Session_No_Rights)
	{
		CMainFrame::GetInstance().ShowMessageTipWnd(true, _T("You have no rights."));
	}
}

void CBottomBarWndUI::DoUnmutedVideoBtnClick()
{
	ZoomVideoSDKMgr::GetInst().MuteVideo();
}

void CBottomBarWndUI::UpdateUIForVideoStatus(bool is_video_on)
{
	if (!btn_muted_video_ || !btn_unmuted_video_)
		return;

	btn_muted_video_->SetVisible(!is_video_on);
	btn_unmuted_video_->SetVisible(is_video_on);
}

void CBottomBarWndUI::DoParticipantBtnClick()
{
	if (CMainFrame::GetInstance().IsParticipantGalleryShown())
		CMainFrame::GetInstance().ShowParticipantGallery(false);
	else
		CMainFrame::GetInstance().ShowParticipantGallery(true);
}



void CBottomBarWndUI::ShowErrorTipDlg(IZoomVideoSDKUser* user)
{
	if (!user) return;

	if (!error_tip_dlg_showing_)
	{
		error_tip_dlg_showing_ = true;
		ErrorTipDlg error_tip_dlg;

		wstring user_name = user->getUserName();
		TCHAR text[512] = { 0 };
		wsprintf(text, _T("%s is sharing. Yon can not share now."), user_name.c_str());   
		error_tip_dlg.SetErrorTipText(text);
		HWND hwnd = error_tip_dlg.Create(CMainFrame::GetInstance().GetHWND(), nullptr, UI_WNDSTYLE_DIALOG & ~WS_MAXIMIZEBOX, 0);
		RECT rc = CMainFrame::GetInstance().GetMainFrameCenterRect(hwnd);
		::SetWindowPos(hwnd, NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE);
		error_tip_dlg.ShowModal();
		error_tip_dlg_showing_ = false;
	}
}





void CBottomBarWndUI::DoSettingBtnClick()
{
	RECT rcScreen = GetButtonPos(btn_setting_);

	CMainFrame::GetInstance().ShowSessionSettingWnd(true, rcScreen);
}

void CBottomBarWndUI::DoEndSessionBtnClick()
{
	CMainFrame::GetInstance().ShowLeaveSessionTipsWnd(true);
}

void CBottomBarWndUI::DoSendChatBtnClick()
{
	if (!edit_chat_)
		return;

	CDuiString text = edit_chat_->GetText();
	edit_chat_->SetText(_T(""));

	CMainFrame::GetInstance().SendChatToAll(text.GetData());
	edit_chat_->SetFocus();
}