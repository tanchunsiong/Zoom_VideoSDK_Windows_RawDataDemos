#pragma once

class ChatSendEditUI;
class CBottomBarWndUI : public WindowImplBase
{
public:
	CBottomBarWndUI();
	~CBottomBarWndUI();

public:

	void OnSessionJoin();
	void UpdateParticipantNum();
	void UpdateFeedBackNum(int feedback_num);
	void UpdateUIForAudioStatus(bool is_audio_on);
	void UpdateUIForVideoStatus(bool is_video_on);
protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetWindowClassName() const { return _T("VideoSDKDemoBottomBarUI"); }
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
protected:
	virtual LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

private:
	void DoMutedAudioBtnClick();
	void DoUnmutedAudioBtnClick();
	void DoMutedVideoBtnClick();
	void DoUnmutedVideoBtnClick();
	void DoParticipantBtnClick();
	void DoSettingBtnClick();
	void DoEndSessionBtnClick();
	void DoSendChatBtnClick();
private:
	void InitControls();
	void ShowErrorTipDlg(IZoomVideoSDKUser* user);
	RECT GetButtonPos(CButtonUI* btn) const;
private:

	CButtonUI* btn_muted_audio_ = nullptr;
	CButtonUI* btn_unmuted_audio_ = nullptr;
	CButtonUI* btn_muted_video_ = nullptr;
	CButtonUI* btn_unmuted_video_ = nullptr;
	CButtonUI* btn_participant_ = nullptr;
	CButtonUI* btn_setting_ = nullptr;
	CButtonUI* btn_share_ = nullptr;
	CButtonUI* btn_emoji_ = nullptr;
	CButtonUI* btn_more_ = nullptr;
	CButtonUI* btn_feedback_ = nullptr;
	CVerticalLayoutUI* feedback_layout_ = nullptr;
	ChatSendEditUI* edit_chat_ = nullptr;
	CLabelUI* label_participants_ = nullptr;
	CLabelUI* label_feedback_num_ = nullptr;
	bool error_tip_dlg_showing_ = false;
};