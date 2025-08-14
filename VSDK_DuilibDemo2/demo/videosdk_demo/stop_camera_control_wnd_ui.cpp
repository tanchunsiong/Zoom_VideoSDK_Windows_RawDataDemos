#include "stdafx.h"
#include "stop_camera_control_wnd_ui.h"
#include "main_frame.h"
#include "videosdk_demo_mgr.h"

CStopCameraControlWndUI::CStopCameraControlWndUI()
{

}

CStopCameraControlWndUI::~CStopCameraControlWndUI()
{
	
}

void CStopCameraControlWndUI::InitControls()
{
	btn_donot_stop_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_donot_stop")));
	btn_stop_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_stop")));
}

void CStopCameraControlWndUI::InitWindow()
{
	InitControls();
}

CDuiString CStopCameraControlWndUI::GetSkinFolder()
{
	return _T("");
}

CDuiString CStopCameraControlWndUI::GetSkinFile()
{
	return to_wstring(IDXML_STOP_CAMERA_CONTROL_WND_UI).c_str();
}

UILIB_RESOURCETYPE CStopCameraControlWndUI::GetResourceType() const
{
	return UILIB_RESOURCE;
}

LRESULT CStopCameraControlWndUI::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		DoStopBtnClick();
		return TRUE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void CStopCameraControlWndUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btn_donot_stop"))
		{
			DoNotStopBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_stop"))
		{
			DoStopBtnClick();
		}
	}

	__super::Notify(msg);
}

void CStopCameraControlWndUI::DoNotStopBtnClick()
{
	ShowWindow(false);
}
void CStopCameraControlWndUI::DoStopBtnClick()
{
	std::wstring user_name;
	ZoomVideoSDKMgr::GetInst().GiveUpControlRemoteCamera(user_name);
	wstring str_message;
	str_message = _T("You can no longer control ") + user_name + _T("'s camera because permissions have been revoked");
	CMainFrame::GetInstance().ShowRemoteControlCameraWnd(false);

	ShowWindow(false);
}
