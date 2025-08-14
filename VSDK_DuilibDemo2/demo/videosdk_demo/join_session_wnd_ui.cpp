#include "stdafx.h"
#include "join_session_wnd_ui.h"
#include "main_frame.h"
#include "session_settings_wnd_ui.h"
#include "zoom_videosdk_renderer_api.h"
#include "videosdk_demo_mgr.h"
#include <time.h>

#define HIDE_INVALID_PARAMTER_LABEL 1001
#define HIDE_WRONG_PASSWORD_LABEL   1002
#define HIDE_YOUR_NAME_ERROR_TIP    1003
#define UPDATE_DURATION_TIMER       1004
#define CONNECTION_TIMEOUT_TIMER    1005

CJoinSessionWndUI::CJoinSessionWndUI()
    : m_bConnected(false)
    , m_bVideoOn(false)
    , m_bAudioOn(false)
    , m_dwSessionStartTime(0)
    , edit_jwt_token_(nullptr)
    , btn_join_session_(nullptr)
    , btn_leave_session_(nullptr)
    , btn_toggle_video_(nullptr)
    , btn_toggle_audio_(nullptr)
    , btn_send_chat_(nullptr)
    , label_status_(nullptr)
    , label_session_info_(nullptr)
    , label_duration_(nullptr)
    , label_connection_status_(nullptr)
    , list_participants_(nullptr)
    , richedit_chat_display_(nullptr)
    , edit_chat_input_(nullptr)
    , combo_camera_(nullptr)
    , video_preview_(nullptr)
    , slider_mic_volume_(nullptr)
    , slider_speaker_volume_(nullptr)
{

}

CJoinSessionWndUI::~CJoinSessionWndUI()
{

}

void CJoinSessionWndUI::InitControls()
{
    // Original controls
    tab_layout_ = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("join_session_tablayout")));
    edit_session_name_ = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("edit_session_name")));
    edit_user_name_ = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("edit_your_name")));
    edit_session_psw_ = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("edit_password")));
    btn_setting_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_settings")));
    btn_join_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_join")));
    btn_create_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_create")));
    label_join_create_title_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_join_create_a_session")));
    check_box_turn_off_video_ = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chk_turn_off_video")));
    check_box_mute_audio_ = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chk_mute_audio")));

    join_create_session_layout_ = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("join_create_a_session")));
    init_layout_ = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("init_layout")));
    error_layout_ = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("error_layout")));
    label_invalid_paramter_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_session_error")));
    label_error_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("error_text")));
    btn_error_ok = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_error_ok")));
    label_your_name_error_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_your_name_error")));
    labe_password_error_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_password_error")));

    // New enhanced UI controls
    edit_jwt_token_ = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("edit_jwt_token")));
    btn_join_session_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_join_session")));
    btn_leave_session_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_leave_session")));
    btn_toggle_video_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_toggle_video")));
    btn_toggle_audio_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_toggle_audio")));
    btn_send_chat_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_send_chat")));
    label_status_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_status")));
    label_session_info_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_session_info")));
    label_duration_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_duration")));
    label_connection_status_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_connection_status")));
    list_participants_ = static_cast<CListUI*>(m_PaintManager.FindControl(_T("list_participants")));
    richedit_chat_display_ = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("richedit_chat_display")));
    edit_chat_input_ = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("edit_chat_input")));
    combo_camera_ = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("combo_camera")));
    video_preview_ = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("video_preview")));
    slider_mic_volume_ = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("slider_mic_volume")));
    slider_speaker_volume_ = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("slider_speaker_volume")));

    // Initialize slider values
    if (slider_mic_volume_)
        slider_mic_volume_->SetValue(80);
    if (slider_speaker_volume_)
        slider_speaker_volume_->SetValue(80);

    // Initialize main frame reference
    CMainFrame::GetInstance().InitVideoSDK();
    
    // Populate camera list
    PopulateCameraList();
}

void CJoinSessionWndUI::InitWindow()
{
    InitControls();
    SetIcon(IDI_ICON_MAIN);
}

CDuiString CJoinSessionWndUI::GetSkinFolder()
{
    return _T("");
}

CDuiString CJoinSessionWndUI::GetSkinFile()
{
    return to_wstring(IDXML_JOIN_SESSION_WND_UI).c_str();
}

UILIB_RESOURCETYPE CJoinSessionWndUI::GetResourceType() const
{
    return UILIB_RESOURCE;
}

LRESULT CJoinSessionWndUI::ResponseDefaultKeyEvent(WPARAM wParam)
{
    if (wParam == VK_RETURN)
    {
        // Handle Enter key in chat input
        if (edit_chat_input_ && edit_chat_input_->IsFocused())
        {
            SendChatMessage();
            return TRUE;
        }
        return FALSE;
    }
    else if (wParam == VK_ESCAPE)
    {
        return TRUE;
    }
    return FALSE;
}

void CJoinSessionWndUI::Notify(TNotifyUI& msg)
{
    if (msg.sType == _T("click"))
    {
        if (msg.pSender->GetName() == _T("close"))
        {
            DoCloseBtnClick();
        }
        else if (msg.pSender->GetName() == _T("btn_join_session"))
        {
            DoJoinSessionClick();
        }
        else if (msg.pSender->GetName() == _T("btn_leave_session"))
        {
            DoLeaveSessionClick();
        }
        else if (msg.pSender->GetName() == _T("btn_toggle_video"))
        {
            DoToggleVideoClick();
        }
        else if (msg.pSender->GetName() == _T("btn_toggle_audio"))
        {
            DoToggleAudioClick();
        }
        else if (msg.pSender->GetName() == _T("btn_send_chat"))
        {
            SendChatMessage();
        }
        // Original handlers
        else if (msg.pSender->GetName() == _T("btn_join_session_original"))
        {
            DoJoinSessionBtnClick();
        }
        else if (msg.pSender->GetName() == _T("btn_create_session"))
        {
            DoCreateSessionBtnClick();
        }
        else if (msg.pSender->GetName() == _T("btn_back"))
        {
            DoBackBtnClick();
        }
        else if (msg.pSender->GetName() == _T("btn_join"))
        {
            DoJoinBtnClick();
        }
        else if (msg.pSender->GetName() == _T("btn_create"))
        {
            DoJoinBtnClick();
        }
        else if (msg.pSender->GetName() == _T("btn_settings"))
        {
            DoSettingBtnClick();
        }
        else if (msg.pSender->GetName() == _T("btn_error_ok"))
        {
            DoErrorOkBtnClick();
        }
    }
    else if (msg.sType == _T("selectchanged"))
    {
        if (msg.pSender->GetName() == _T("combo_camera"))
        {
            OnCameraSelectionChanged();
        }
    }
    else if (msg.sType == _T("valuechanged"))
    {
        if (msg.pSender->GetName() == _T("slider_mic_volume"))
        {
            OnMicVolumeChanged();
        }
        else if (msg.pSender->GetName() == _T("slider_speaker_volume"))
        {
            OnSpeakerVolumeChanged();
        }
    }
    __super::Notify(msg);
}

LRESULT CJoinSessionWndUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_TIMER:
        {
            OnTimer(uMsg, wParam, lParam);
            break;
        }
    }
    return __super::HandleMessage(uMsg, wParam, lParam);
}

void CJoinSessionWndUI::DoJoinSessionClick()
{
    // Debug: Check if controls are found
    if (!edit_session_name_)
    {
        UpdateConnectionStatus(_T("Session name control not found"), false);
        return;
    }
    if (!edit_jwt_token_)
    {
        UpdateConnectionStatus(_T("JWT token control not found"), false);
        return;
    }
    if (!edit_user_name_)
    {
        UpdateConnectionStatus(_T("User name control not found"), false);
        return;
    }

    CDuiString sessionName = edit_session_name_->GetText();
    CDuiString jwtToken = edit_jwt_token_->GetText();
    CDuiString userName = edit_user_name_->GetText();
    CDuiString password = edit_session_psw_ ? edit_session_psw_->GetText() : _T("");

    if (sessionName.IsEmpty() || jwtToken.IsEmpty() || userName.IsEmpty())
    {
        UpdateConnectionStatus(_T("Please fill in all required fields"), false);
        return;
    }

    // Initialize renderer first (like the original code does)
    CSessionSettingsWndUI* setting_wnd = CMainFrame::GetInstance().GetSessionSettingsWndUI();
    VideoSDKRendererType renderer_type = setting_wnd ? setting_wnd->GetRendererType() : VideoSDKRendererType(0);
    VideoSDKRenderInitParam init_param;
    init_param.frame_rate = 30;
    init_param.renderer_type = renderer_type;
    VideoSDKRenderInitErrorType error = InitZoomVideoSDKRendererModule(init_param);

    // Join session using the enhanced method
    UpdateConnectionStatus(_T("Connecting..."), false);
    
    // Set connection timeout
    SetTimer(m_hWnd, CONNECTION_TIMEOUT_TIMER, 15000, NULL); // 15 second timeout
    
    ZoomVideoSDKSessionContext session_context;
    session_context.sessionName = sessionName;
    session_context.sessionPassword = password.IsEmpty() ? nullptr : password.GetData();
    session_context.userName = userName;
    session_context.token = jwtToken;
    session_context.videoOption.localVideoOn = false;  // Start with video off
    session_context.audioOption.connect = true;       // Connect audio
    session_context.audioOption.mute = true;          // Start muted

    // Debug output
    OutputDebugStringA("DoJoinSessionClick: About to call JoinSession\n");
    
    CMainFrame::GetInstance().JoinSession(session_context);
    
    OutputDebugStringA("DoJoinSessionClick: JoinSession call completed\n");
}

void CJoinSessionWndUI::DoLeaveSessionClick()
{
    if (m_bConnected)
    {
        CMainFrame::GetInstance().LeaveSession(false);
        UpdateConnectionStatus(_T("Disconnecting..."), false);
    }
}

void CJoinSessionWndUI::DoToggleVideoClick()
{
    if (!m_bConnected)
        return;

    m_bVideoOn = !m_bVideoOn;
    
    // Toggle video through SDK manager
    if (m_bVideoOn)
        ZoomVideoSDKMgr::GetInst().UnmuteVideo();
    else
        ZoomVideoSDKMgr::GetInst().MuteVideo();
    
    UpdateVideoButtonState(m_bVideoOn);
}

void CJoinSessionWndUI::DoToggleAudioClick()
{
    if (!m_bConnected)
        return;

    m_bAudioOn = !m_bAudioOn;
    
    // Toggle audio through SDK manager
    if (m_bAudioOn)
        ZoomVideoSDKMgr::GetInst().UnmuteAudio();
    else
        ZoomVideoSDKMgr::GetInst().MuteAudio();
    
    UpdateAudioButtonState(m_bAudioOn);
}

void CJoinSessionWndUI::SendChatMessage()
{
    if (!edit_chat_input_ || !m_bConnected)
        return;

    CDuiString message = edit_chat_input_->GetText();
    if (!message.IsEmpty())
    {
        CMainFrame::GetInstance().SendChatToAll(message);
        edit_chat_input_->SetText(_T(""));
    }
}

void CJoinSessionWndUI::OnCameraSelectionChanged()
{
    if (!combo_camera_ || !m_bConnected)
        return;

    int selectedIndex = combo_camera_->GetCurSel();
    // Handle camera selection through main frame
    // This would need to be implemented in the main frame
}

void CJoinSessionWndUI::OnMicVolumeChanged()
{
    if (!slider_mic_volume_)
        return;

    int volume = slider_mic_volume_->GetValue();
    // Set microphone volume through SDK manager
    // Note: Volume control implementation would depend on the specific SDK API
    // This is a placeholder for the actual volume control
}

void CJoinSessionWndUI::OnSpeakerVolumeChanged()
{
    if (!slider_speaker_volume_)
        return;

    int volume = slider_speaker_volume_->GetValue();
    // Set speaker volume through SDK manager
    // Note: Volume control implementation would depend on the specific SDK API
    // This is a placeholder for the actual volume control
}

void CJoinSessionWndUI::UpdateConnectionStatus(const CDuiString& status, bool connected)
{
    m_bConnected = connected;
    
    // Clear connection timeout timer when status changes
    KillTimer(m_hWnd, CONNECTION_TIMEOUT_TIMER);
    
    if (label_status_)
    {
        CDuiString statusText = _T("Status: ");
        statusText += status;
        label_status_->SetText(statusText);
        label_status_->SetTextColor(connected ? 0xFF00AA00 : 0xFFFF0000);
    }
    
    if (label_connection_status_)
    {
        label_connection_status_->SetText(status);
    }
    
    if (btn_join_session_)
        btn_join_session_->SetEnabled(!connected);
    if (btn_leave_session_)
        btn_leave_session_->SetEnabled(connected);
    
    if (connected)
    {
        m_dwSessionStartTime = GetTickCount();
        SetTimer(m_hWnd, UPDATE_DURATION_TIMER, 1000, NULL); // Start duration timer
    }
    else
    {
        KillTimer(m_hWnd, UPDATE_DURATION_TIMER); // Stop duration timer
        if (label_duration_)
            label_duration_->SetText(_T("00:00:00"));
    }
}

void CJoinSessionWndUI::UpdateSessionInfo(const CDuiString& sessionName)
{
    if (label_session_info_)
    {
        label_session_info_->SetText(sessionName);
    }
}

void CJoinSessionWndUI::UpdateVideoButtonState(bool videoOn)
{
    m_bVideoOn = videoOn;
    if (btn_toggle_video_)
    {
        btn_toggle_video_->SetText(videoOn ? _T("Stop Video") : _T("Start Video"));
    }
}

void CJoinSessionWndUI::UpdateAudioButtonState(bool audioOn)
{
    m_bAudioOn = audioOn;
    if (btn_toggle_audio_)
    {
        btn_toggle_audio_->SetText(audioOn ? _T("Mute Audio") : _T("Unmute Audio"));
    }
}

void CJoinSessionWndUI::AddChatMessage(const CDuiString& sender, const CDuiString& message)
{
    if (!richedit_chat_display_)
        return;

    // Get current time
    SYSTEMTIME st;
    GetLocalTime(&st);
    
    CDuiString timeStr;
    timeStr.Format(_T("[%02d:%02d:%02d] "), st.wHour, st.wMinute, st.wSecond);
    
    CDuiString fullMessage;
    fullMessage = timeStr;
    fullMessage += sender;
    fullMessage += _T(": ");
    fullMessage += message;
    fullMessage += _T("\r\n");
    
    // Append to chat display
    CDuiString currentText = richedit_chat_display_->GetText();
    currentText += fullMessage;
    richedit_chat_display_->SetText(currentText);
    
    // Scroll to bottom
    richedit_chat_display_->EndDown();
}

void CJoinSessionWndUI::PopulateCameraList()
{
    if (!combo_camera_)
        return;

    combo_camera_->RemoveAll();
    
    // Get camera list from SDK manager
    IVideoSDKVector<IZoomVideoSDKCameraDevice*>* cameraList = ZoomVideoSDKMgr::GetInst().GetCameraList();
    if (cameraList)
    {
        for (int i = 0; i < cameraList->GetCount(); i++)
        {
            IZoomVideoSDKCameraDevice* camera = cameraList->GetItem(i);
            if (camera)
            {
                const zchar_t* cameraName = camera->getDeviceName();
                CListLabelElementUI* pListElement = new CListLabelElementUI;
                pListElement->SetText(cameraName ? cameraName : _T("Unknown Camera"));
                combo_camera_->Add(pListElement);
            }
        }
        
        if (cameraList->GetCount() > 0)
        {
            combo_camera_->SelectItem(0);
        }
    }
}

void CJoinSessionWndUI::UpdateDurationDisplay()
{
    if (!label_duration_ || !m_bConnected)
        return;

    DWORD currentTime = GetTickCount();
    DWORD elapsed = (currentTime - m_dwSessionStartTime) / 1000; // Convert to seconds
    
    int hours = elapsed / 3600;
    int minutes = (elapsed % 3600) / 60;
    int seconds = elapsed % 60;
    
    CDuiString durationStr;
    durationStr.Format(_T("%02d:%02d:%02d"), hours, minutes, seconds);
    label_duration_->SetText(durationStr);
}

// Original methods for compatibility
void CJoinSessionWndUI::DoJoinSessionBtnClick()
{
    CSessionSettingsWndUI* setting_wnd = CMainFrame::GetInstance().GetSessionSettingsWndUI();

    if (!setting_wnd)
        return;

    token_ = setting_wnd->GetToken();
    if (token_.empty())
    {
        DoSettingBtnClick();
        return;
    }

    setting_wnd->ShowWindow(false);

    if (btn_setting_)
    {
        btn_setting_->SetVisible(false);
    }

    VideoSDKRendererType renderer_type = setting_wnd->GetRendererType();
    VideoSDKRenderInitParam init_param;
    init_param.frame_rate = 30;
    init_param.renderer_type = renderer_type;

    VideoSDKRenderInitErrorType error = InitZoomVideoSDKRendererModule(init_param);

    ShowJoinOrCreateSessionPage(true);
}

void CJoinSessionWndUI::DoCreateSessionBtnClick()
{
    CSessionSettingsWndUI* setting_wnd = CMainFrame::GetInstance().GetSessionSettingsWndUI();

    if (!setting_wnd)
        return;

    token_ = setting_wnd->GetToken();
    if (token_.empty())
    {
        DoSettingBtnClick();
        return;
    }

    setting_wnd->ShowWindow(false);

    if (btn_setting_)
    {
        btn_setting_->SetVisible(false);
    }
    VideoSDKRendererType renderer_type = setting_wnd->GetRendererType();
    VideoSDKRenderInitParam init_param;
    init_param.frame_rate = 30;
    init_param.renderer_type = renderer_type;

    VideoSDKRenderInitErrorType error = InitZoomVideoSDKRendererModule(init_param);
    ShowJoinOrCreateSessionPage(false);
}

void CJoinSessionWndUI::DoJoinBtnClick()
{
    if (!edit_session_name_ || !edit_session_psw_ || !edit_user_name_
        ||!check_box_turn_off_video_ || !check_box_mute_audio_)
        return;

    //session name
    session_name_ = edit_session_name_->GetText();
    //user name
    CDuiString sUserName = edit_user_name_->GetText();
    if (sUserName.IsEmpty())
    {
        if (label_your_name_error_)
        {
            label_your_name_error_->SetVisible(true);
            SetTimer(m_hWnd, HIDE_YOUR_NAME_ERROR_TIP, 100, NULL);
        }
        return;
    }
    //password
    session_password_ = edit_session_psw_->GetText();
    //is turn off video
    bool is_video_on = !check_box_turn_off_video_->GetCheck();
    //is mute audio
    bool is_mute_audio = check_box_mute_audio_->GetCheck();

    ZoomVideoSDKSessionContext session_context;
    session_context.sessionName = session_name_;
    session_context.sessionPassword = session_password_;
    session_context.userName = sUserName;
    session_context.token = token_.c_str();
    session_context.videoOption.localVideoOn = is_video_on;
    session_context.audioOption.connect = true;
    session_context.audioOption.mute = is_mute_audio;
    
    CMainFrame::GetInstance().JoinSession(session_context);
}

void CJoinSessionWndUI::DoCloseBtnClick()
{
    CMainFrame::GetInstance().OnJoinSessionCloseBtnClick();
}

void CJoinSessionWndUI::DoSettingBtnClick()
{
    RECT rc;
    GetClientRect(m_hWnd, &rc);
    POINT ptLeftTop = { rc.left,rc.top };
    POINT ptRightBottom = { rc.right,rc.bottom };
    ClientToScreen(m_hWnd, &ptLeftTop);
    ClientToScreen(m_hWnd, &ptRightBottom);
    RECT rcScreen = { ptLeftTop.x,ptLeftTop.y,ptRightBottom.x,ptRightBottom.y };

    CMainFrame::GetInstance().ShowSessionSettingWnd(true,rcScreen);
}

void CJoinSessionWndUI::DoBackBtnClick()
{
    ShowInitPage();
}

void CJoinSessionWndUI::DoErrorOkBtnClick()
{
    ShowJoinOrCreateSessionPage(true);

    KillTimer(m_hWnd, HIDE_WRONG_PASSWORD_LABEL);
}

void CJoinSessionWndUI::ShowJoinOrCreateSessionPage(bool is_join)
{
    if (tab_layout_ && join_create_session_layout_)
    {
        tab_layout_->SelectItem(join_create_session_layout_);
    }

    ShowBtnJoinOrCreate(is_join);
    
    if (edit_session_name_)
    {
        edit_session_name_->SetFocus();
    }
}

void CJoinSessionWndUI::ShowInitPage()
{
    if (btn_setting_)
    {
        btn_setting_->SetVisible(true);
    }

    if (tab_layout_ && init_layout_)
    {
        tab_layout_->SelectItem(init_layout_);
    }
}

void CJoinSessionWndUI::ShowErrorPage()
{
    if (tab_layout_ && init_layout_)
    {
        tab_layout_->SelectItem(error_layout_);
    }
}

void CJoinSessionWndUI::ShowBtnJoinOrCreate(bool show_join)
{
    if (!btn_join_ || !btn_create_ ||!label_join_create_title_)
        return;

    if (edit_session_name_)
    {
        edit_session_name_->SetText(_T(""));
    }

    if (edit_user_name_)
    {
        edit_user_name_->SetText(_T(""));
    }

    if (edit_session_psw_)
    {
        edit_session_psw_->SetText(_T(""));
    }

    if (label_invalid_paramter_)
    {
        label_invalid_paramter_->SetVisible(false);
    }

    if (label_your_name_error_)
    {
        label_your_name_error_->SetVisible(false);
    }

    if (labe_password_error_)
    {
        labe_password_error_->SetVisible(false);
    }

    btn_join_->SetVisible(show_join);
    btn_create_->SetVisible(!show_join);

    if (show_join)
    {
        label_join_create_title_->SetText(_T("Join a Session"));
    }
    else
    {
        label_join_create_title_->SetText(_T("Create a Session"));
    }
}

void CJoinSessionWndUI::OnInvalidParamterNotification()
{
    if (label_invalid_paramter_)
        label_invalid_paramter_->SetVisible(true);

    SetTimer(m_hWnd, HIDE_INVALID_PARAMTER_LABEL, 100, NULL);
}

void CJoinSessionWndUI::OnJoinPasswordWrong()
{
    ShowErrorPage();
    if (label_error_)	
        label_error_->SetText(_T("Wrong password.Please enter again."));

    SetTimer(m_hWnd, HIDE_WRONG_PASSWORD_LABEL, 5000, NULL);
}

void CJoinSessionWndUI::BackToInitPage()
{
    if (!error_layout_ || !tab_layout_)
        return;

    if (error_layout_ != tab_layout_->GetItemAt(tab_layout_->GetCurSel())) //current page is not error page,back to init page.
    {
        ShowInitPage();
    }
}

bool CJoinSessionWndUI::IsSelectedVideoOff()
{
    if (check_box_turn_off_video_)
    {
        return check_box_turn_off_video_->GetCheck();
    }

    return false;
}

wstring CJoinSessionWndUI::FormatString(wstring str, int error_code)
{
    TCHAR temp_str[256] = { 0 };
    wsprintf(temp_str, _T("%s(Error Code: %d)"), str.c_str(),error_code);   

    return temp_str;
}

void CJoinSessionWndUI::OnJoinMettingJoinFailed(int error_code)
{
    if (label_error_)
    {
        wstring str_error = FormatString(_T("Unable to join or create session.Please try again."), error_code);
        label_error_->SetText(str_error.c_str());
    }

    ShowErrorPage();
}

void CJoinSessionWndUI::JudgeHideInvalidParamter()
{
    if (!edit_session_name_ || !label_invalid_paramter_)
        return;

    if (session_name_ != edit_session_name_->GetText())
    {
        label_invalid_paramter_->SetVisible(false);
        KillTimer(m_hWnd, HIDE_INVALID_PARAMTER_LABEL);
    }
}

void CJoinSessionWndUI::JudgeHideWrongPWDTip()
{
    ShowJoinOrCreateSessionPage(true);
    KillTimer(m_hWnd, HIDE_WRONG_PASSWORD_LABEL);
}

void CJoinSessionWndUI::JudgeHideNameEmptyErrorTip()
{
    if (!label_your_name_error_ || !edit_user_name_)
        return;

    if (!(edit_user_name_->GetText().IsEmpty()))
    {
        label_your_name_error_->SetVisible(false);
        KillTimer(m_hWnd, HIDE_YOUR_NAME_ERROR_TIP);
    }
}

void CJoinSessionWndUI::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (wParam == HIDE_INVALID_PARAMTER_LABEL)
    {
        JudgeHideInvalidParamter();
    }
    else if (wParam == HIDE_WRONG_PASSWORD_LABEL)
    {
        JudgeHideWrongPWDTip();
    }
    else if (wParam == HIDE_YOUR_NAME_ERROR_TIP)
    {
        JudgeHideNameEmptyErrorTip();
    }
    else if (wParam == UPDATE_DURATION_TIMER)
    {
        UpdateDurationDisplay();
    }
    else if (wParam == CONNECTION_TIMEOUT_TIMER)
    {
        // Connection timeout - reset to ready state
        KillTimer(m_hWnd, CONNECTION_TIMEOUT_TIMER);
        UpdateConnectionStatus(_T("Connection timeout - please try again"), false);
    }
}
