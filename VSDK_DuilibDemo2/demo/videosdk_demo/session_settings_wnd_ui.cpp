#include "stdafx.h"
#include "session_settings_wnd_ui.h"
#include "main_frame.h"
#include "videosdk_demo_mgr.h"
#include "lower_thirds_info_wnd.h"

#define TOKEN_EDIT_TEXT_CHANGE_TIMER 1001
#define REFRESH_CURSOR_TIMER	1002

CSessionSettingsWndUI::CSessionSettingsWndUI()
{
}

CSessionSettingsWndUI::~CSessionSettingsWndUI()
{

}

void CSessionSettingsWndUI::InitControls()
{
	lowerthird_info_ = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("lowerthird_info")));
	control_left_line_ = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("control_left_line")));
	label_thirds_name_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_thirds_name")));
	label_thirds_title_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_thirds_title")));
	control_right_corner_ = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("control_right_corner")));

	combo_camera_list_ = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("combo_camera_list")));	
	combo_speaker_list_ = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("combo_speaker_list")));
	combo_mic_list_ = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("combo_microphone_list")));
	combo_renderer_type_ = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("combo_render_type")));
	edit_token_ = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("edit_token")));
	label_empty_tip_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("token_empty_tip")));
	token_layout_ = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("token_layout")));
	statistics_layout_ = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("show_statistics_layout")));
	chk_fill_frame_ = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chk_fill_video_to_frame")));	
	chk_enable_lower_third_ = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chk_enable_lower_third")));
	btn_custom_lower_third_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_custom_lower_third")));
	client_layout_ = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("client_layout")));
}

void CSessionSettingsWndUI::InitWindow()
{
	InitControls();
}

CDuiString CSessionSettingsWndUI::GetSkinFolder()
{
	return _T("");
}

CDuiString CSessionSettingsWndUI::GetSkinFile()
{
	return to_wstring(IDXML_SESSION_SETTINGS_WND_UI).c_str();
}

UILIB_RESOURCETYPE CSessionSettingsWndUI::GetResourceType() const
{
	return UILIB_RESOURCE;
}

LRESULT CSessionSettingsWndUI::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CSessionSettingsWndUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == _T("btn_session_settings_close"))
		{
			DoCloseBtnClick();
		}
		else if (msg.pSender->GetName() == _T("chk_fill_video_to_frame"))
		{
			DoRenderModeSelect();
		}
		else if (msg.pSender->GetName() == _T("btn_show_statistics"))
		{
		
		}	
		else if (msg.pSender->GetName() == _T("btn_custom_lower_third"))
		{
			DoCustomLowerThirdClick();
		}
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
		if (msg.pSender == combo_camera_list_)
		{
			HandleCameraSelected();
		}
		else if (msg.pSender == combo_mic_list_)
		{
			HandleMicSelected();
		}
		else if (msg.pSender == combo_speaker_list_)
		{
			HandleSpeakerSelected();
		}
	}
	else if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		if (msg.pSender == chk_enable_lower_third_)
			HandleEnableLowerThirdChange();
	}

	__super::Notify(msg);
}

bool CSessionSettingsWndUI::SelectCamera(const zchar_t* cameraDeviceID)
{
	if (!ZoomVideoSDKMgr::GetInst().IsMyselfVideoOn())
		return false;
	else
		return ZoomVideoSDKMgr::GetInst().SelectCamera(cameraDeviceID);
}

void CSessionSettingsWndUI::UpdateSettingsInfoUI()
{
	//update camera list
	UpdateCameraListUI();
	//update speaker list
	UpdateSpeakerListUI();
	//update microphone list
	UdateMicrophoneListUI();
}

void CSessionSettingsWndUI::UpdateControl()
{
	if (!token_layout_ || !edit_token_ || !combo_renderer_type_ || !statistics_layout_)
		return;

	bool is_in_session = ZoomVideoSDKMgr::GetInst().IsInSession();

	if (!is_in_session)
	{
		combo_renderer_type_->SetEnabled(true);
		token_layout_->SetVisible(true);
		statistics_layout_->SetVisible(false);
		SetTimer(m_hWnd, TOKEN_EDIT_TEXT_CHANGE_TIMER, 100, 0);
	}
	else
	{
		combo_renderer_type_->SetEnabled(false);
		token_layout_->SetVisible(false);
		statistics_layout_->SetVisible(true);
		KillTimer(m_hWnd, TOKEN_EDIT_TEXT_CHANGE_TIMER);
	}

	if (!chk_enable_lower_third_)
		return;

	if (IsShowLowerThirdsInfoWnd())
	{
		chk_enable_lower_third_->SetCheck(true);
		ShowLowerThirdInfo(true);
	}
}

wstring CSessionSettingsWndUI::GetToken()
{
	if (!edit_token_)
		return _T("");

	return edit_token_->GetText().GetData();
}

bool CSessionSettingsWndUI::IsShowLowerThirdsInfoWnd()
{
	std::wstring file_name = CMainFrame::GetInstance().GetLowerThirdConfPath();

	TCHAR tempStr[MAX_PATH] = { 0 };
	GetPrivateProfileString(_T("LOWERTHIRD"), _T("Enable"), NULL, tempStr, sizeof(tempStr), file_name.data());
	std::wstring enableStr = tempStr;
	return enableStr == _T("1");	
}

VideoSDKRendererType CSessionSettingsWndUI::GetRendererType()
{
	if (!combo_renderer_type_)
		return kVideoSDKRenderTypeNone;
	wstring text = combo_renderer_type_->GetText().GetData();
	if (text == _T("SDL"))
	{
		return kVideoSDKRenderTypeSDL;
	}
	return kVideoSDKRenderTypeNone;
}

void CSessionSettingsWndUI::UpdateCameraListUI()
{
	if (!combo_camera_list_)
		return;
	//wimle todo: need make sure the RemoveAll call will DELETE all the items we add before
	combo_camera_list_->RemoveAll();
	IVideoSDKVector<IZoomVideoSDKCameraDevice*>* pCameraList = ZoomVideoSDKMgr::GetInst().GetCameraList();
	if (pCameraList)
	{
		for (int i = 0; i < pCameraList->GetCount(); i++)
		{
			IZoomVideoSDKCameraDevice* pCameraDevice = pCameraList->GetItem(i);

			const zchar_t* pDeviceName = nullptr;
			const zchar_t* pDeviceID = nullptr;

			bool bSelected = false;
			if (pCameraDevice)
			{
				pDeviceName = pCameraDevice->getDeviceName();
				pDeviceID = pCameraDevice->getDeviceId();
				bSelected = pCameraDevice->isSelectedDevice();
			}

			CListLabelElementUI* pCameraListElement = new CListLabelElementUI;
			if (!pCameraListElement)
				continue;
			pCameraListElement->SetText(pDeviceName);
			pCameraListElement->SetTag((UINT_PTR)pDeviceID);

			combo_camera_list_->Add(pCameraListElement);

			if (bSelected)
			{
				combo_camera_list_->SelectItem(i);
			}
		}
	}
}

void CSessionSettingsWndUI::UpdateSpeakerListUI()
{
	if (!combo_speaker_list_)
		return;
	combo_speaker_list_->RemoveAll();
	IVideoSDKVector<IZoomVideoSDKSpeakerDevice*>* pSpeakerList = ZoomVideoSDKMgr::GetInst().GetSpeakerList();
	if (pSpeakerList)
	{
		for (int i = 0; i < pSpeakerList->GetCount(); i++)
		{
			IZoomVideoSDKSpeakerDevice* pSpeakerDevice = pSpeakerList->GetItem(i);

			const zchar_t* pDeviceName = nullptr;
			const zchar_t* pDeviceID = nullptr;

			bool bSelected = false;
			if (pSpeakerDevice)
			{
				pDeviceName = pSpeakerDevice->getDeviceName();
				pDeviceID = pSpeakerDevice->getDeviceId();
				bSelected = pSpeakerDevice->isSelectedDevice();
			}

			CListLabelElementUI* pSpeakerListElement = new CListLabelElementUI;
			if (!pSpeakerListElement)
				continue;
			pSpeakerListElement->SetText(pDeviceName);
			pSpeakerListElement->SetTag((UINT_PTR)pDeviceID);

			combo_speaker_list_->Add(pSpeakerListElement);

			if (bSelected)
			{
				combo_speaker_list_->SelectItem(i);
			}
		}
	}
}

bool CSessionSettingsWndUI::SelectSpeaker(const zchar_t* devidceID, const zchar_t* deviceName)
{
	if (!devidceID || !deviceName) return false;

	ZoomVideoSDKErrors err = ZoomVideoSDKMgr::GetInst().SelectSpeaker(devidceID, deviceName);

	if (err == ZoomVideoSDKErrors_Success)
	{
		return true;
	}

	return false;
}

void CSessionSettingsWndUI::UdateMicrophoneListUI()
{
	if (!combo_mic_list_)
		return;
	combo_mic_list_->RemoveAll();
	IVideoSDKVector<IZoomVideoSDKMicDevice*>* pMicList = ZoomVideoSDKMgr::GetInst().GetMicList();
	if (pMicList)
	{
		for (int i = 0; i < pMicList->GetCount(); i++)
		{
			IZoomVideoSDKMicDevice* pMicDevice = pMicList->GetItem(i);

			const zchar_t* pDeviceName = nullptr;
			const zchar_t* pDeviceID = nullptr;

			bool bSelected = false;
			if (pMicDevice)
			{
				pDeviceName = pMicDevice->getDeviceName();
				pDeviceID = pMicDevice->getDeviceId();
				bSelected = pMicDevice->isSelectedDevice();
			}

			CListLabelElementUI* pMicListElement = new CListLabelElementUI;
			if (!pMicListElement)
				continue;
			pMicListElement->SetText(pDeviceName);
			pMicListElement->SetTag((UINT_PTR)pDeviceID);

			combo_mic_list_->Add(pMicListElement);

			if (bSelected)
			{
				combo_mic_list_->SelectItem(i);
			}
		}
	}
}

bool CSessionSettingsWndUI::SelectMicrophone(const zchar_t* devidceID, const zchar_t* deviceName)
{
	if (!devidceID || !deviceName) return false;

	ZoomVideoSDKErrors err = ZoomVideoSDKMgr::GetInst().SelectMic(devidceID, deviceName);

	if (err == ZoomVideoSDKErrors_Success)
	{
		return true;
	}

	return false;
}



LRESULT CSessionSettingsWndUI::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::OnLButtonUp(uMsg, wParam, lParam, bHandled);
}

LRESULT CSessionSettingsWndUI::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//ShowWindow(false);
	return __super::OnKillFocus(uMsg, wParam, lParam, bHandled);
}

LRESULT CSessionSettingsWndUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_TIMER:
	{
		OnTimer(uMsg, wParam, lParam);
		break;
	}
	case WM_SHOWWINDOW:
	{
		CheckCursorRefresh(wParam);
		break;
	}
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

void CSessionSettingsWndUI::CheckCursorRefresh(bool is_window_show)
{
	bool is_in_session = ZoomVideoSDKMgr::GetInst().IsInSession();
	if (is_in_session)
	{
		KillTimer(m_hWnd, REFRESH_CURSOR_TIMER);
		return;
	}

	if (is_window_show)
		SetTimer(m_hWnd, REFRESH_CURSOR_TIMER, 100, 0);
	else
		KillTimer(m_hWnd, REFRESH_CURSOR_TIMER);
}

void CSessionSettingsWndUI::DoCloseBtnClick()
{
	CMainFrame::GetInstance().ShowSessionSettingWnd(false);
}

void CSessionSettingsWndUI::DoRenderModeSelect()
{
	if (!chk_fill_frame_)
		return;
	RendererMode mode = !chk_fill_frame_->GetCheck() ? kVideoSDKRenderMode_FullRect : kVideoSDKRenderMode_AspectRatio;
	CMainFrame::GetInstance().HanldeRenderModeChanged(mode);
}


void CSessionSettingsWndUI::DoCustomLowerThirdClick()
{
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	POINT ptLeftTop = { rc.left,rc.top };
	POINT ptRightBottom = { rc.right,rc.bottom };
	ClientToScreen(m_hWnd, &ptLeftTop);
	ClientToScreen(m_hWnd, &ptRightBottom);
	RECT rcScreen = { ptLeftTop.x,ptLeftTop.y,ptRightBottom.x,ptRightBottom.y };

	CMainFrame::GetInstance().ShowLowerThirdsSettingWndUI(true, rcScreen);
	this->ShowWindow(false);
}

void CSessionSettingsWndUI::HandleEnableLowerThirdChange()
{
	if (!chk_enable_lower_third_ || !btn_custom_lower_third_)
		return;

	std::wstring file_name = CMainFrame::GetInstance().GetLowerThirdConfPath();
	TCHAR tempStr[MAX_PATH] = { 0 };
	::WritePrivateProfileString(_T("LOWERTHIRD"), _T("Enable"), chk_enable_lower_third_->GetCheck() ? _T("1") : _T("0"), file_name.data());

	ShowLowerThirdInfo(chk_enable_lower_third_->GetCheck());

	if (ZoomVideoSDKMgr::GetInst().IsInSession())
		CMainFrame::GetInstance().ShowLowerThirdsInfoWnd(chk_enable_lower_third_->GetCheck());
}

void CSessionSettingsWndUI::HandleCameraSelected()
{
	if (!combo_camera_list_)
		return;
	int iSelItem = combo_camera_list_->GetCurSel();
	CListLabelElementUI* pSelItem = (CListLabelElementUI*)combo_camera_list_->GetItemAt(iSelItem);
	if (!pSelItem)
		return;
	const zchar_t* pDeviceID = (zchar_t*)pSelItem->GetTag();
	SelectCamera(pDeviceID);
}

void CSessionSettingsWndUI::HandleMicSelected()
{
	if (!combo_mic_list_)
		return;
	int iSelItem = combo_mic_list_->GetCurSel();
	CListLabelElementUI* pSelItem = (CListLabelElementUI*)combo_mic_list_->GetItemAt(iSelItem);

	const zchar_t* pDevideName = pSelItem->GetText();
	const zchar_t* pDeviceID = (zchar_t*)pSelItem->GetTag();
	SelectMicrophone(pDeviceID, pDevideName);
}

void CSessionSettingsWndUI::HandleSpeakerSelected()
{
	if (!combo_speaker_list_)
		return;
	int iSelItem = combo_speaker_list_->GetCurSel();
	CListLabelElementUI* pSelItem = (CListLabelElementUI*)combo_speaker_list_->GetItemAt(iSelItem);
	if (!pSelItem)
		return;
	const zchar_t* pDevideName = pSelItem->GetText();
	const zchar_t* pDeviceID = (zchar_t*)pSelItem->GetTag();
	SelectSpeaker(pDeviceID, pDevideName);
}

void CSessionSettingsWndUI::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (wParam == TOKEN_EDIT_TEXT_CHANGE_TIMER)
	{
		wstring token;
		if (edit_token_)
		{
			token = edit_token_->GetText().GetData();
		}
		if (label_empty_tip_)
		{
			if (token.empty() && client_layout_)
			{
				label_empty_tip_->SetVisible(true);
				client_layout_->EndDown();
				edit_token_->SetFocus();
				KillTimer(m_hWnd, TOKEN_EDIT_TEXT_CHANGE_TIMER);
			}
			else
			{
				label_empty_tip_->SetVisible(false);
			}
		}
	}
	else if (wParam == REFRESH_CURSOR_TIMER)
	{
		m_PaintManager.Invalidate();		
	}
}

void CSessionSettingsWndUI::ShowLowerThirdInfo(bool show)
{
	if (!lowerthird_info_ || !label_thirds_name_ || !control_left_line_ || !label_thirds_title_ || !control_right_corner_)
		return;

	if (show)
	{
		std::wstring username, description, color, isenable;
		CMainFrame::GetInstance().GetLowerThirdInfoFromIniFile(username, description, color, isenable);

		if (username.empty())
			return;

		LowerThirdColorInfo colorInfo = CMainFrame::GetInstance().GetLowerThirdsColorInfo(color);

		label_thirds_name_->SetText(username.data());
		control_left_line_->SetBkImage(colorInfo.color_normal_image.data());
		
		if (description.empty())
		{
			lowerthird_info_->SetFixedHeight(40);
			control_left_line_->SetMaxHeight(19);
			label_thirds_title_->SetVisible(false);
			control_right_corner_->SetVisible(false);
		}
		else
		{
			lowerthird_info_->SetFixedHeight(62);
			control_left_line_->SetMaxHeight(38);
			label_thirds_title_->SetVisible(true);
			control_right_corner_->SetVisible(true);

			label_thirds_title_->SetBkImage(colorInfo.color_normal_image.data());
			label_thirds_title_->SetText(description.data());
			label_thirds_title_->SetTextColor(colorInfo.textcolor);
			control_right_corner_->SetBkImage(colorInfo.color_triangle_image.data());
		}
		
	}

	lowerthird_info_->SetVisible(show);
}