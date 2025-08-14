#include "stdafx.h"
#include "camera_control_request_received_wnd_ui.h"
#include "main_frame.h"
#include "videosdk_demo_mgr.h"

CCameraControlRequestReceivedWndUI::CCameraControlRequestReceivedWndUI()
{
	m_pCameraControlRequestHandler = NULL;
	m_pRequester = NULL;
}

CCameraControlRequestReceivedWndUI::~CCameraControlRequestReceivedWndUI()
{
	m_pCameraControlRequestHandler = NULL;
	m_pRequester = NULL;
}

void CCameraControlRequestReceivedWndUI::InitControls()
{
	btn_decline_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_decline")));
	btn_approve_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_approve")));
	label_text_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_text")));
}

void CCameraControlRequestReceivedWndUI::InitWindow()
{
	InitControls();
}

CDuiString CCameraControlRequestReceivedWndUI::GetSkinFolder()
{
	return _T("");
}

CDuiString CCameraControlRequestReceivedWndUI::GetSkinFile()
{
	return to_wstring(IDXML_CAMERA_CONTROL_REQUEST_WND_UI).c_str();
}

UILIB_RESOURCETYPE CCameraControlRequestReceivedWndUI::GetResourceType() const
{
	return UILIB_RESOURCE;
}

LRESULT CCameraControlRequestReceivedWndUI::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CCameraControlRequestReceivedWndUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btn_camera_control_request_recieved_close"))
		{
			DoDeclineBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_decline"))
		{
			DoDeclineBtnClick();
		}
		else if (msg.pSender->GetName() == _T("btn_approve"))
		{
			DoApproveBtnClick();
		}
	}

	__super::Notify(msg);
}

void CCameraControlRequestReceivedWndUI::DoDeclineBtnClick()
{
	ZoomVideoSDKErrors err = ZoomVideoSDKErrors_Internal_Error;
	if (m_pCameraControlRequestHandler)
	{
		err = m_pCameraControlRequestHandler->decline();
		m_pCameraControlRequestHandler = NULL;
		ShowWindow(false);
		if (err == ZoomVideoSDKErrors_Success)
		{
			CMainFrame::GetInstance().ShowControlCameraTipButton(false);
			SetRequester(NULL);
		}
	}
}
void CCameraControlRequestReceivedWndUI::DoApproveBtnClick()
{
	ZoomVideoSDKErrors err = ZoomVideoSDKErrors_Internal_Error;
	if (m_pCameraControlRequestHandler)
	{
		err = m_pCameraControlRequestHandler->approve();
		m_pCameraControlRequestHandler = NULL;
		ShowWindow(false);
		if (err == ZoomVideoSDKErrors_Success)
		{
			CMainFrame::GetInstance().ShowControlCameraTipButton(true);
		}
	}
}

void CCameraControlRequestReceivedWndUI::SetCameraControlRequestHandler(IZoomVideoSDKCameraControlRequestHandler* handler)
{
	if (handler)
	{
		m_pCameraControlRequestHandler = handler;
	}
}
void CCameraControlRequestReceivedWndUI::SetRequester(IZoomVideoSDKUser* pUser)
{
	m_pRequester = pUser;
	if (pUser)
	{
		std::wstring text;
		std::wstring userName;
		userName = pUser->getUserName();
		text = userName + _T(" is requesting control of your camera");
		TCHAR szTmp[1024 * 2] = { 0 };
		wsprintf(szTmp, _T("%s"), text.c_str());     
		label_text_->SetText(szTmp);
	}
}
IZoomVideoSDKUser* CCameraControlRequestReceivedWndUI::GetRequester()
{
	return m_pRequester;
}
void CCameraControlRequestReceivedWndUI::OnUserLeave(IVideoSDKVector<IZoomVideoSDKUser*>* userList)
{
	if (!userList) return;

	std::wstring curUserId = _T("");
	if (m_pRequester && m_pRequester->getUserID())
	{
		curUserId = m_pRequester->getUserID();
	}

	for (int i = 0; i < userList->GetCount(); i++)
	{
		IZoomVideoSDKUser* leave_user = userList->GetItem(i);
		if (!leave_user) continue;

		std::wstring userId = leave_user->getUserID();
		if (curUserId == userId)
		{
			std::wstring text;
			std::wstring userName;
			userName = leave_user->getUserName();
			text = userName + _T(" stopped controlling your camera");
			CMainFrame::GetInstance().ShowControlCameraTipButton(false);
			SetRequester(NULL);
		}
		else if (ZoomVideoSDKMgr::GetInst().GetRemoteControlCameraUser() &&
				ZoomVideoSDKMgr::GetInst().GetRemoteControlCameraUser()->getUserID())
		{
			std::wstring remoteControlCameraUserID = ZoomVideoSDKMgr::GetInst().GetRemoteControlCameraUser()->getUserID();
			if (userId == remoteControlCameraUserID)
			{
				CMainFrame::GetInstance().ShowRemoteControlCameraWnd(false);
			}
		}
	}
}

