#include "stdafx.h"
#include "title_bar_wnd_ui.h"
#include "main_frame.h"

CTitleBarWndUI::CTitleBarWndUI()
{

}

CTitleBarWndUI::~CTitleBarWndUI()
{

}

void CTitleBarWndUI::InitWindow()
{
	InitControls();
}

CDuiString CTitleBarWndUI::GetSkinFolder()
{
	return _T("");
}

CDuiString CTitleBarWndUI::GetSkinFile()
{
	return to_wstring(IDXML_TITLE_BAR_WND_UI).c_str();
}

UILIB_RESOURCETYPE CTitleBarWndUI::GetResourceType() const
{
	return UILIB_RESOURCE;
}

LRESULT CTitleBarWndUI::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CTitleBarWndUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btn_close"))
		{
			DoCloseBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_minimize"))
		{
			DoMinimizeBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_fullscreen"))
		{
			DoFullScreenBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_session_name"))
		{
			DoShowSessionInfoBtnClick();
		}
	}

	__super::Notify(msg);
}

void CTitleBarWndUI::InitControls()
{
	btn_session_name_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_session_name")));
}

void CTitleBarWndUI::UpdateSessionName(const zchar_t* szSessionName)
{
	if (btn_session_name_ && szSessionName)
	{
		btn_session_name_->SetText(szSessionName);
	}
}

void CTitleBarWndUI::DoShowSessionInfoBtnClick()
{
	if (!btn_session_name_)
		return;
	RECT rc = btn_session_name_->GetPos();
	POINT ptLeftTop = { rc.left,rc.top };
	POINT ptRightBottom = { rc.right,rc.bottom };
	ClientToScreen(m_hWnd, &ptLeftTop);
	ClientToScreen(m_hWnd, &ptRightBottom);
	RECT rcScreen = { ptLeftTop.x,ptLeftTop.y,ptRightBottom.x,ptRightBottom.y };
	CMainFrame::GetInstance().ShowSessionInfoWnd(rcScreen);
}

void CTitleBarWndUI::DoCloseBtnClick()
{
	CMainFrame::GetInstance().ShowLeaveSessionTipsWnd(true);
}
void CTitleBarWndUI::DoMinimizeBtnClick()
{
	CMainFrame::GetInstance().OnMinimizeBtnClick();	
}
void CTitleBarWndUI::DoFullScreenBtnClick()
{
	CMainFrame::GetInstance().OnFullScreenBtnClick();
}