#pragma once

class CJoinSessionWndUI : public WindowImplBase
{
public:
	CJoinSessionWndUI();
	~CJoinSessionWndUI();

public:
	void OnInvalidParamterNotification();
	void OnJoinMettingJoinFailed(int error_code);
	void OnJoinPasswordWrong();
	void BackToInitPage();
	bool IsSelectedVideoOff();

	// Enhanced UI Methods
	void DoJoinSessionClick();
	void DoLeaveSessionClick();
	void DoToggleVideoClick();
	void DoToggleAudioClick();
	void SendChatMessage();
	void OnCameraSelectionChanged();
	void OnMicVolumeChanged();
	void OnSpeakerVolumeChanged();
	void UpdateConnectionStatus(const CDuiString& status, bool connected);
	void UpdateSessionInfo(const CDuiString& sessionName);
	void UpdateVideoButtonState(bool videoOn);
	void UpdateAudioButtonState(bool audioOn);
	void AddChatMessage(const CDuiString& sender, const CDuiString& message);
	void PopulateCameraList();
	void UpdateDurationDisplay();
protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetWindowClassName() const { return _T("VideoSDKDemoJoinSessionUI"); }
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void Notify(TNotifyUI& msg);
	
private:

	void InitControls();
	void DoJoinSessionBtnClick();
	void DoCreateSessionBtnClick();
	void DoJoinBtnClick();
	void DoCloseBtnClick();
	void DoSettingBtnClick();
	void DoBackBtnClick();
	void DoErrorOkBtnClick();
	void ShowJoinOrCreateSessionPage(bool is_join);
	void ShowInitPage();
	void ShowErrorPage();
	void ShowBtnJoinOrCreate(bool show_join);

	void JudgeHideInvalidParamter();
	void JudgeHideWrongPWDTip();
	void JudgeHideNameEmptyErrorTip();
	void OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam);
	wstring FormatString(wstring str,int error_code);
private:

	CTabLayoutUI* tab_layout_ = nullptr;

	CRichEditUI* edit_session_name_ = nullptr;
	CRichEditUI* edit_user_name_ = nullptr;
	CRichEditUI* edit_session_psw_ = nullptr;
	CButtonUI* btn_setting_ = nullptr;
	CButtonUI* btn_join_ = nullptr;
	CButtonUI* btn_create_ = nullptr;
	CLabelUI* label_join_create_title_ = nullptr;
	CCheckBoxUI* check_box_turn_off_video_ = nullptr;
	CCheckBoxUI* check_box_mute_audio_ = nullptr;
	wstring token_;

	CVerticalLayoutUI* init_layout_ = nullptr;
	CHorizontalLayoutUI* join_create_session_layout_ = nullptr;
	CVerticalLayoutUI* error_layout_ = nullptr;
	CLabelUI* label_error_ = nullptr;
	CLabelUI* label_invalid_paramter_ = nullptr;
	CLabelUI* label_your_name_error_ = nullptr;
	CLabelUI* labe_password_error_ = nullptr;
	CButtonUI* btn_error_ok = nullptr;
	CDuiString session_name_;
	CDuiString session_password_;

	// Enhanced UI Controls
	CRichEditUI* edit_jwt_token_ = nullptr;
	CButtonUI* btn_join_session_ = nullptr;
	CButtonUI* btn_leave_session_ = nullptr;
	CButtonUI* btn_toggle_video_ = nullptr;
	CButtonUI* btn_toggle_audio_ = nullptr;
	CButtonUI* btn_send_chat_ = nullptr;
	CLabelUI* label_status_ = nullptr;
	CLabelUI* label_session_info_ = nullptr;
	CLabelUI* label_duration_ = nullptr;
	CLabelUI* label_connection_status_ = nullptr;
	CListUI* list_participants_ = nullptr;
	CRichEditUI* richedit_chat_display_ = nullptr;
	CRichEditUI* edit_chat_input_ = nullptr;
	CComboUI* combo_camera_ = nullptr;
	CControlUI* video_preview_ = nullptr;
	CSliderUI* slider_mic_volume_ = nullptr;
	CSliderUI* slider_speaker_volume_ = nullptr;

	// State Variables
	bool m_bConnected = false;
	bool m_bVideoOn = false;
	bool m_bAudioOn = false;
	DWORD m_dwSessionStartTime = 0;
};
