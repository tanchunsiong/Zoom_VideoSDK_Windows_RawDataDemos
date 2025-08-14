#include "stdafx.h"
#include "lowerthirds_setting_wnd_ui.h"
#include "main_frame.h"
#include "videosdk_demo_mgr.h"
#include "lower_thirds_info_wnd.h"

#define LOWERTHIRDS_EDIT_TEXT_CHANGE_TIMER 1002
#define STR_PATH_LOEWETHIRD_SETTING	_T("\\")
#define LOEWETHIRD_SETTING_INI_NAME	_T("lowerthird.ini")

std::wstring GetExePath()
{
	wchar_t szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameW(NULL, szFilePath, MAX_PATH);

	(_tcsrchr(szFilePath, '\\'))[0] = 0;
	wstring path = szFilePath;
	return path;
}

CLowerThirdsSettingWndUI::CLowerThirdsSettingWndUI()
{
	m_lowerThirdColorStr = _T("#444B53");
}

CLowerThirdsSettingWndUI::~CLowerThirdsSettingWndUI()
{

}

void CLowerThirdsSettingWndUI::InitControls()
{
	session_setting_layout_ = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("session_setting_layout")));
	lower_third_layout_ = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("lower_third_layout")));
	
	lowerthird_info_setting_ = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("lowerthird_info_setting")));
	option_color_layout_ = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("option_color_layout")));
	control_left_line_setting_ = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("control_left_line_setting")));
	label_thirds_name_setting_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_thirds_name_setting")));
	label_thirds_title_setting_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_thirds_title_setting")));
	control_right_corner_setting_ = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("control_right_corner_setting")));

	btn_save_lower_third_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_save_lower_third")));
	btn_cancel_lower_third_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_cancel_lower_third")));
	edit_lower_third_name_ = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("edit_lower_third_name")));
	edit_lower_third_title_ = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("edit_lower_third_title")));
	
	option_color_none_ = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("option_color_none")));
	option_color_blue_ = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("option_color_blue")));
	option_color_violet_ = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("option_color_violet")));
	option_color_green_ = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("option_color_green")));
	option_color_orange_ = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("option_color_orange")));
	option_color_red_ = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("option_color_red")));
	option_color_darkblue_ = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("option_color_darkblue")));
	option_color_yellow_ = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("option_color_yellow")));
}

void CLowerThirdsSettingWndUI::InitWindow()
{
	InitControls();
}

CDuiString CLowerThirdsSettingWndUI::GetSkinFolder()
{
	return _T("");
}

CDuiString CLowerThirdsSettingWndUI::GetSkinFile()
{
	return to_wstring(IDXML_LOWERTHIRDS_SETTINGS_WND_UI).c_str();
}

UILIB_RESOURCETYPE CLowerThirdsSettingWndUI::GetResourceType() const
{
	return UILIB_RESOURCE;
}

LRESULT CLowerThirdsSettingWndUI::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CLowerThirdsSettingWndUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == _T("btn_save_lower_third"))
		{
			DoSaveLowerThirdsClick();
		}
		else if (msg.pSender->GetName() == _T("btn_cancel_lower_third"))
		{
			this->ShowWindow(false);
		}		
	}	
	else if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		if (msg.pSender == option_color_none_ || msg.pSender == option_color_blue_ || msg.pSender == option_color_violet_ ||
			msg.pSender == option_color_green_ || msg.pSender == option_color_orange_ || msg.pSender == option_color_red_ ||
			msg.pSender == option_color_darkblue_ || msg.pSender == option_color_yellow_)
		{
			HandleLowerThirdColorChange(msg.pSender);
		}
	}

	__super::Notify(msg);
}


void CLowerThirdsSettingWndUI::UpdateControl()
{
	std::wstring file_name = GetLowerThirdConfPath();

	TCHAR tempStr[MAX_PATH] = { 0 };
	::GetPrivateProfileString(_T("LOWERTHIRD"), _T("UserName"), NULL, tempStr, sizeof(tempStr), file_name.data());
	std::wstring user_name = tempStr;

	if (!user_name.empty())
		ShowLowerThirdInfoSetting(true);
	else
	{
		if (lowerthird_info_setting_)
			lowerthird_info_setting_->SetVisible(false);
	}
	
}

wstring CLowerThirdsSettingWndUI::GetLowerThirdConfPath()
{
	std::wstring file_name = GetExePath();
	file_name += STR_PATH_LOEWETHIRD_SETTING;
	file_name += LOEWETHIRD_SETTING_INI_NAME;

	return file_name;
}

LRESULT CLowerThirdsSettingWndUI::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::OnLButtonUp(uMsg, wParam, lParam, bHandled);
}

LRESULT CLowerThirdsSettingWndUI::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//ShowWindow(false);
	return __super::OnKillFocus(uMsg, wParam, lParam, bHandled);
}

LRESULT CLowerThirdsSettingWndUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SHOWWINDOW:
	{
		if (wParam)
			SetTimer(m_hWnd, LOWERTHIRDS_EDIT_TEXT_CHANGE_TIMER, 1000, NULL);
		else
			KillTimer(m_hWnd, LOWERTHIRDS_EDIT_TEXT_CHANGE_TIMER);
		break;
	}
	case WM_TIMER:
	{
		OnTimer(uMsg, wParam, lParam);
		break;
	}
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

void CLowerThirdsSettingWndUI::DoCloseBtnClick()
{
	CMainFrame::GetInstance().ShowSessionSettingWnd(false);
}

void CLowerThirdsSettingWndUI::HandleLowerThirdColorChange(CControlUI* pControl)
{
	if (!pControl)	return;
	m_lowerThirdColorStr = pControl->GetUserData();

	LowerThirdColorInfo colorInfo = CMainFrame::GetInstance().GetLowerThirdsColorInfo(m_lowerThirdColorStr.GetData());

	if (!control_left_line_setting_ || !label_thirds_title_setting_ || !control_right_corner_setting_)
		return;
	
	control_left_line_setting_->SetBkImage(colorInfo.color_normal_image.data());
	label_thirds_title_setting_->SetBkImage(colorInfo.color_normal_image.data());
	label_thirds_title_setting_->SetTextColor(colorInfo.textcolor);
	control_right_corner_setting_->SetBkImage(colorInfo.color_triangle_image.data());
}

void CLowerThirdsSettingWndUI::GetLowerThirdInfoFromIniFile(std::wstring& username, std::wstring& description, std::wstring& color, std::wstring& enable)
{
	std::wstring file_name = GetLowerThirdConfPath();
	
	TCHAR tempStr[MAX_PATH] = { 0 };
	::GetPrivateProfileString(_T("LOWERTHIRD"), _T("UserName"), NULL, tempStr, sizeof(tempStr), file_name.data());
	username = tempStr;
	::GetPrivateProfileString(_T("LOWERTHIRD"), _T("Description"), NULL, tempStr, sizeof(tempStr), file_name.data());
	description = tempStr;
	::GetPrivateProfileString(_T("LOWERTHIRD"), _T("Color"), NULL, tempStr, sizeof(tempStr), file_name.data());
	color = tempStr;
	::GetPrivateProfileString(_T("LOWERTHIRD"), _T("Enable"), NULL, tempStr, sizeof(tempStr), file_name.data());
	enable = tempStr;
}

void CLowerThirdsSettingWndUI::ShowFullLayout(bool is_show_full)
{
	if (!lowerthird_info_setting_ || !control_left_line_setting_ || !label_thirds_title_setting_ || !control_right_corner_setting_)
		return;

	if (!is_show_full)
	{
		lowerthird_info_setting_->SetFixedHeight(40);
		control_left_line_setting_->SetMaxHeight(19);
		label_thirds_title_setting_->SetVisible(false);
		control_right_corner_setting_->SetVisible(false);
	}
	else
	{
		lowerthird_info_setting_->SetFixedHeight(62);
		control_left_line_setting_->SetMaxHeight(38);
		label_thirds_title_setting_->SetVisible(true);
		control_right_corner_setting_->SetVisible(true);
	}

}

void CLowerThirdsSettingWndUI::ShowLowerThirdInfoSetting(bool show)
{
	if (!lowerthird_info_setting_ || !label_thirds_name_setting_ || !control_left_line_setting_ || !label_thirds_title_setting_
		 || !control_right_corner_setting_ || !edit_lower_third_name_ || !edit_lower_third_title_)
		return;

	if (show)
	{
		std::wstring username, description, color, isenable;
		GetLowerThirdInfoFromIniFile(username, description, color, isenable);

		if (username.empty())
			return;

		LowerThirdColorInfo colorInfo = CMainFrame::GetInstance().GetLowerThirdsColorInfo(color);

		label_thirds_name_setting_->SetText(username.data());
		control_left_line_setting_->SetBkImage(colorInfo.color_normal_image.data());

		if (description.empty())
		{
			ShowFullLayout(false);
		}
		else
		{
			ShowFullLayout(true);		
			label_thirds_title_setting_->SetBkImage(colorInfo.color_normal_image.data());
			label_thirds_title_setting_->SetText(description.data());
			label_thirds_title_setting_->SetTextColor(colorInfo.textcolor);
			control_right_corner_setting_->SetBkImage(colorInfo.color_triangle_image.data());
		}

		edit_lower_third_name_->SetText(username.data());
		edit_lower_third_title_->SetText(description.data());

		SwitchOptionColorByColorKey(color);		
	}
}

void CLowerThirdsSettingWndUI::SwitchOptionColorByColorKey(const std::wstring& color)
{
	if (!option_color_layout_ || color.empty())
		return;

	for (int i = 0; i < option_color_layout_->GetCount(); i++)
	{
		COptionUI* item_option = (COptionUI*)option_color_layout_->GetItemAt(i);
		if (!item_option)
			continue;

		if (item_option->GetUserData().GetData() == color)
		{
			item_option->Selected(true);
			break;
		}		
	}
}

void CLowerThirdsSettingWndUI::DoSaveLowerThirdsClick()
{
	if (!edit_lower_third_name_ || !edit_lower_third_title_)
		return;

	m_lowerThirdsNameStr = edit_lower_third_name_->GetText();
	m_lowerThirdsTitleStr = edit_lower_third_title_->GetText();
	
	SaveLowerThirdSettingInfo(m_lowerThirdsNameStr.GetData(), m_lowerThirdsTitleStr.GetData(), m_lowerThirdColorStr.GetData());

	SendLowerThirdSettingInfo(m_lowerThirdsNameStr.GetData(), m_lowerThirdsTitleStr.GetData(), m_lowerThirdColorStr.GetData());

	this->ShowWindow(false);
}

void CLowerThirdsSettingWndUI::UpdateSaveBtnEnable()
{
	if (!btn_save_lower_third_)
		return;

	bool btnSaveEnable = CheckLowerThirdsInput();
	btn_save_lower_third_->SetEnabled(btnSaveEnable);
}

void CLowerThirdsSettingWndUI::UpdatePreviewLowerThirdInfo()
{
	if (!edit_lower_third_name_ || !edit_lower_third_title_)
		return;

	CDuiString temp_name = edit_lower_third_name_->GetText(); 
	CDuiString temp_description = edit_lower_third_title_->GetText();

	if (temp_name.GetLength() > 20)
	{
		edit_lower_third_name_->SetText(m_lowerThirdsNameStr);
		return;
	}
	if (temp_description.GetLength() > 24)
	{
		edit_lower_third_title_->SetText(m_lowerThirdsTitleStr);
		return;
	}

	if (temp_description.IsEmpty())	
		ShowFullLayout(false);	
	else	
		ShowFullLayout(true);
	
	if (m_lowerThirdsNameStr != temp_name)
	{
		m_lowerThirdsNameStr = temp_name;
		label_thirds_name_setting_->SetText(m_lowerThirdsNameStr);
	}
	if (m_lowerThirdsTitleStr != temp_description)
	{
		m_lowerThirdsTitleStr = temp_description;
		label_thirds_title_setting_->SetText(m_lowerThirdsTitleStr);
	}
	

}

void CLowerThirdsSettingWndUI::CheckIsNeedUpdateLowerThirdWnd()
{
	std::wstring username, description, color, isenable;
	GetLowerThirdInfoFromIniFile(username, description, color, isenable);

	if (username.empty())
		return;

	IZoomVideoSDKUser* pCurUser = CMainFrame::GetInstance().GetGalleryContainerSelectUser();

	if (pCurUser)
	{
		std::wstring curSelectUserId = pCurUser->getUserID();
		std::wstring mySelfId = ZoomVideoSDKMgr::GetInst().GetMySelf()->getUserID();
		if (curSelectUserId == mySelfId)
		{
			CMainFrame::GetInstance().SetLowerThirdsInfo(username, description, color);
		}
	}
	else
	{
		CMainFrame::GetInstance().SetLowerThirdsInfo(username, description, color);
	}
}

void CLowerThirdsSettingWndUI::SaveLowerThirdSettingInfo(const std::wstring& username, const std::wstring& description, const std::wstring& color)
{
	std::wstring file_name = GetLowerThirdConfPath();
	
	::WritePrivateProfileString(_T("LOWERTHIRD"), _T("UserName"), username.data(), file_name.data());
	::WritePrivateProfileString(_T("LOWERTHIRD"), _T("Description"), description.data(), file_name.data());
	::WritePrivateProfileString(_T("LOWERTHIRD"), _T("Color"), color.data(), file_name.data());

	CheckIsNeedUpdateLowerThirdWnd();
}

void CLowerThirdsSettingWndUI::SendLowerThirdSettingInfo(const std::wstring& username, const std::wstring& description, const std::wstring& color)
{
	CDuiString cmdStr;
	cmdStr.Format(_T("%d|%s|%s|%s"), CmdChannelType::CMD_LOWER_THIRD, username.data(), description.data(), color.data());

	//update myself viedobase info		
	CMainFrame::GetInstance().onCommandReceived(ZoomVideoSDKMgr::GetInst().GetMySelf(), cmdStr.GetData());

	if (ZoomVideoSDKMgr::GetInst().IsInSession())
	{
		if (!CMainFrame::GetInstance().IsCommandChannelConnect())
			return;

		ZoomVideoSDKErrors err = ZoomVideoSDKMgr::GetInst().SendCommand(NULL, cmdStr.GetData());

		if (ZoomVideoSDKErrors_Success != err)
		{
			wstring errStr = ZoomVideoSDKMgr::GetInst().GetErrorStringByErrorCode(err);
			MessageBoxW(NULL, errStr.data(), _T("Error"), NULL);
		}
	}
}

bool CLowerThirdsSettingWndUI::CheckLowerThirdsInput()
{
	if (!edit_lower_third_name_)
		return false;

	CDuiString temp_name = edit_lower_third_name_->GetText();

	return !temp_name.IsEmpty();
}

void CLowerThirdsSettingWndUI::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (wParam == LOWERTHIRDS_EDIT_TEXT_CHANGE_TIMER)
	{
		UpdateSaveBtnEnable();
		UpdatePreviewLowerThirdInfo();
	}
}