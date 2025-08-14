#include "stdafx.h"
#include "switch_camera_pop_wnd_ui.h"
#include "main_frame.h"
#include "videosdk_demo_mgr.h"

CSwitchCameraPopWnd::CSwitchCameraPopWnd()
{

}

CSwitchCameraPopWnd::~CSwitchCameraPopWnd()
{

}


void CSwitchCameraPopWnd::InitControl()
{	
	btn_camera_1_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_camera_1")));
	btn_camera_2_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_camera_2")));
	btn_camera_3_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_camera_3")));
	btn_camera_4_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_camera_4")));
	btn_camera_5_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_camera_5")));
	btn_camera_6_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_camera_6")));
}

void CSwitchCameraPopWnd::InitWindow()
{	
	InitControl();
}                                                                                                                                       

void CSwitchCameraPopWnd::Notify(TNotifyUI& msg)
{
	int iii = 0;
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btn_camera_1"))
		{
			CMainFrame::GetInstance().ShareSelectedCamera(map_cameras_[0]);
		}
		else if (msg.pSender->GetName() == _T("btn_camera_2"))
		{
			CMainFrame::GetInstance().ShareSelectedCamera(map_cameras_[1]);
		}
		else if (msg.pSender->GetName() == _T("btn_camera_3"))
		{
			CMainFrame::GetInstance().ShareSelectedCamera(map_cameras_[2]);
		}
		else if (msg.pSender->GetName() == _T("btn_camera_4"))
		{
			CMainFrame::GetInstance().ShareSelectedCamera(map_cameras_[3]);
		}
		else if (msg.pSender->GetName() == _T("btn_camera_5"))
		{
			CMainFrame::GetInstance().ShareSelectedCamera(map_cameras_[4]);
		}
		else if (msg.pSender->GetName() == _T("btn_camera_6"))
		{
			CMainFrame::GetInstance().ShareSelectedCamera(map_cameras_[5]);
		}
		this->ShowWindow(false);
		map_cameras_.clear();
	}

	__super::Notify(msg);
}

LRESULT CSwitchCameraPopWnd::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::OnSetFocus(uMsg, wParam, lParam, bHandled);
}

LRESULT CSwitchCameraPopWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	this->ShowWindow(false);
	return __super::OnKillFocus(uMsg, wParam, lParam, bHandled);
}

void CSwitchCameraPopWnd::SetBtnText(int index, std::wstring deviceName, std::wstring deviceID)
{
	if (index < 0 || index > 5)
	{
		return;
	}
	if (!btn_camera_1_ ||
		!btn_camera_2_ ||
		!btn_camera_3_ ||
		!btn_camera_4_ ||
		!btn_camera_5_ ||
		!btn_camera_6_
		)
	{
		return;
	}
	TCHAR szTmp[1024 * 5] = { 0 };
	wsprintf(szTmp, _T("%s"), deviceName.c_str());     
	switch (index)
	{
	case 0:
	{
		btn_camera_1_->SetText(szTmp);
		break;
	}
	case 1:
	{
		btn_camera_2_->SetText(szTmp);
		break;
	}
	case 2:
	{
		btn_camera_3_->SetText(szTmp);
		break;
	}
	case 3:
	{
		btn_camera_4_->SetText(szTmp);
		break;
	}
	case 4:
	{
		btn_camera_5_->SetText(szTmp);
		break;
	}
	case 5:
	{
		btn_camera_6_->SetText(szTmp);
		break;
	}
	default:
		break;
	}
	map_cameras_[index] = deviceID;
}
