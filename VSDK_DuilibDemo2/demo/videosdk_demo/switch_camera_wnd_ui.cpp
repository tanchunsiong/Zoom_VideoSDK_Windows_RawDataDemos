#include "stdafx.h"
#include "switch_camera_wnd_ui.h"
#include "main_frame.h"
#include "videosdk_demo_mgr.h"

CSwitchCameraWndUI::CSwitchCameraWndUI()
{

}

CSwitchCameraWndUI::~CSwitchCameraWndUI()
{

}

void CSwitchCameraWndUI::InitWindow()
{
	InitControls();
}

CDuiString CSwitchCameraWndUI::GetSkinFolder()
{
	return _T("");
}

CDuiString CSwitchCameraWndUI::GetSkinFile()
{
	return to_wstring(IDXML_SWITCH_CAMERA_WND_UI).c_str();
}

UILIB_RESOURCETYPE CSwitchCameraWndUI::GetResourceType() const
{
	return UILIB_RESOURCE;
}

LRESULT CSwitchCameraWndUI::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CSwitchCameraWndUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btn_switch_camera"))
		{
			if (ZoomVideoSDKMgr::GetInst().GetNumberOfCameras() > 3)
			{
				RECT rcWindow = { 0 };
				::GetWindowRect(this->GetHWND(), &rcWindow);
				POINT pt_temp = { rcWindow.left, rcWindow.top };
				CMainFrame::GetInstance().ShowSwitchCameraPopWnd(true, pt_temp);
			}
			else
			{
				CMainFrame::GetInstance().SwitchShareCamera();
			}
			
		}
	}

	__super::Notify(msg);
}

void CSwitchCameraWndUI::InitControls()
{
	btn_switch_camera_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_switch_camera")));
}
