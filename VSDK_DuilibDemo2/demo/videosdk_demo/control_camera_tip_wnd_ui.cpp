#include "stdafx.h"
#include "control_camera_tip_wnd_ui.h"
#include "main_frame.h"

CControlCameraTipWndUI::CControlCameraTipWndUI()
{

}

CControlCameraTipWndUI::~CControlCameraTipWndUI()
{

}

void CControlCameraTipWndUI::InitWindow()
{
	InitControls();
}

CDuiString CControlCameraTipWndUI::GetSkinFolder()
{
	return _T("");
}

CDuiString CControlCameraTipWndUI::GetSkinFile()
{
	return to_wstring(IDXML_CONTROL_CAMERA_TIP_WND_UI).c_str();
}

UILIB_RESOURCETYPE CControlCameraTipWndUI::GetResourceType() const
{
	return UILIB_RESOURCE;
}

LRESULT CControlCameraTipWndUI::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CControlCameraTipWndUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		
	}
	__super::Notify(msg);
}

void CControlCameraTipWndUI::InitControls()
{
	label_control_camera_tip_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("label_control_camera_tip")));
}


