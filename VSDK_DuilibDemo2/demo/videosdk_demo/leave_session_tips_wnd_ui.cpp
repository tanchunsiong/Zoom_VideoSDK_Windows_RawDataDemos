#include "stdafx.h"
#include "leave_session_tips_wnd_ui.h"
#include "main_frame.h"
#include "videosdk_demo_mgr.h"

CLeaveSessionTipsWndUI::CLeaveSessionTipsWndUI()
{

}

CLeaveSessionTipsWndUI::~CLeaveSessionTipsWndUI()
{

}

void CLeaveSessionTipsWndUI::InitControls()
{
	btn_end_session_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_end_session")));
	btn_leave_session_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_leave_session")));
}

void CLeaveSessionTipsWndUI::InitWindow()
{
	InitControls();
}

CDuiString CLeaveSessionTipsWndUI::GetSkinFolder()
{
	return _T("");
}

CDuiString CLeaveSessionTipsWndUI::GetSkinFile()
{
	return to_wstring(IDXML_LEAVE_SESSION_TIPS_WND_UI).c_str();
}

UILIB_RESOURCETYPE CLeaveSessionTipsWndUI::GetResourceType() const
{
	return UILIB_RESOURCE;
}

LRESULT CLeaveSessionTipsWndUI::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CLeaveSessionTipsWndUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btn_leave_session_close"))
		{
			ShowWindow(false);
		}
		else if (msg.pSender->GetName() == _T("btn_leave_session"))
		{
			DoLeaveSessionBtnClick(false);
		}
		else if (msg.pSender->GetName() == _T("btn_end_session"))
		{
			DoLeaveSessionBtnClick(true);
		}
	}

	__super::Notify(msg);
}

void CLeaveSessionTipsWndUI::CheckIsHost()
{
	if (!btn_end_session_)
		return;

	if (ZoomVideoSDKMgr::GetInst().IsHost())
	{
		btn_end_session_->SetVisible(true);
	}
	else
	{
		btn_end_session_->SetVisible(false);
	}

}

void CLeaveSessionTipsWndUI::DoLeaveSessionBtnClick(bool bEndSession)
{
	ShowWindow(false);
	CMainFrame::GetInstance().LeaveSession(bEndSession);

}
