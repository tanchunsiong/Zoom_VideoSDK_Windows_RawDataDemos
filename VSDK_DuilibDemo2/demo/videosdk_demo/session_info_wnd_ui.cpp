#include "stdafx.h"
#include "session_info_wnd_ui.h"
#include "main_frame.h"
#include "videosdk_demo_mgr.h"

CSessionInfoWndUI::CSessionInfoWndUI()
{

}

CSessionInfoWndUI::~CSessionInfoWndUI()
{

}

void CSessionInfoWndUI::UpdateSessionInfoUI()
{
	if (!label_participants_num_ || !label_session_name_)
		return;

	const zchar_t* pSessionName = ZoomVideoSDKMgr::GetInst().GetSessionName();
	if (pSessionName)
	{
		label_session_name_->SetText(pSessionName);
	}

	int count = ZoomVideoSDKMgr::GetInst().GetUserCountInSession();

	TCHAR szNum[32] = { 0 };
	wsprintf(szNum, _T("%d"), count);   
	label_participants_num_->SetText(szNum);
}

void CSessionInfoWndUI::InitWindow()
{
	InitControls();
}

void CSessionInfoWndUI::InitControls()
{
	label_session_name_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_session_name")));
	label_participants_num_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_participants_num")));
}

CDuiString CSessionInfoWndUI::GetSkinFolder()
{
	return _T("");
}

CDuiString CSessionInfoWndUI::GetSkinFile()
{
	return to_wstring(IDXML_SESSION_INFO_WND_UI).c_str();
}

UILIB_RESOURCETYPE CSessionInfoWndUI::GetResourceType() const
{
	return UILIB_RESOURCE;
}

LRESULT CSessionInfoWndUI::ResponseDefaultKeyEvent(WPARAM wParam)
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

LRESULT CSessionInfoWndUI::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ShowWindow(false);
	return __super::OnKillFocus(uMsg, wParam, lParam, bHandled);
}