#include "stdafx.h"
#include "remote_control_camera_wnd_ui.h"
#include "main_frame.h"
#include "videosdk_demo_mgr.h"

CRemoteControlCameraWndUI::CRemoteControlCameraWndUI()
{

}

CRemoteControlCameraWndUI::~CRemoteControlCameraWndUI()
{

}

void CRemoteControlCameraWndUI::InitWindow()
{
	InitControls();
}

CDuiString CRemoteControlCameraWndUI::GetSkinFolder()
{
	return _T("");
}

CDuiString CRemoteControlCameraWndUI::GetSkinFile()
{
	return to_wstring(IDXML_REMOTE_CTRL_CAMERA_WND_UI).c_str();
}

UILIB_RESOURCETYPE CRemoteControlCameraWndUI::GetResourceType() const
{
	return UILIB_RESOURCE;
}

LRESULT CRemoteControlCameraWndUI::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CRemoteControlCameraWndUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btn_close"))
		{
			DoClickCloseBtn();
		}
		else if (msg.pSender->GetName() == _T("btn_zoom_in"))
		{
			DoClickZoomInBtn();
		}
		else if (msg.pSender->GetName() == _T("btn_zoom_out"))
		{
			DoClickZoomOutBtn();
		}
		else if (msg.pSender->GetName() == _T("btn_turn_up"))
		{
			DoClickTurnUpBtn();
		}
		else if (msg.pSender->GetName() == _T("btn_turn_left"))
		{
			DoClickTurnLeftBtn();
		}
		else if (msg.pSender->GetName() == _T("btn_turn_right"))
		{
			DoClickTurnRightBtn();
		}
		else if (msg.pSender->GetName() == _T("btn_turn_down"))
		{
			DoClickTurnDownBtn();
		}
	}

	__super::Notify(msg);
}

void CRemoteControlCameraWndUI::InitControls()
{
	btn_close_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_close")));
	btn_zoom_in_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_zoom_in")));
	btn_zoom_out_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_zoom_out")));
	btn_turn_up_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_turn_up")));
	btn_turn_down_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_turn_down")));
	btn_turn_left_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_turn_left")));
	btn_turn_right_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_turn_right")));
}

void CRemoteControlCameraWndUI::DoClickCloseBtn()
{
	CMainFrame::GetInstance().ShowStopCameraControlWnd(true);
}
void CRemoteControlCameraWndUI::DoClickZoomInBtn()
{
	ZoomVideoSDKMgr::GetInst().ZoomIn(50);
}
void CRemoteControlCameraWndUI::DoClickZoomOutBtn()
{
	ZoomVideoSDKMgr::GetInst().ZoomOut(50);
}
void CRemoteControlCameraWndUI::DoClickTurnUpBtn()
{
	ZoomVideoSDKMgr::GetInst().TurnUp(50);
}
void CRemoteControlCameraWndUI::DoClickTurnDownBtn()
{
	ZoomVideoSDKMgr::GetInst().TurnDown(50);
}
void CRemoteControlCameraWndUI::DoClickTurnLeftBtn()
{
	ZoomVideoSDKMgr::GetInst().TurnLeft(50);
}
void CRemoteControlCameraWndUI::DoClickTurnRightBtn()
{
	ZoomVideoSDKMgr::GetInst().TurnRight(50);
}
