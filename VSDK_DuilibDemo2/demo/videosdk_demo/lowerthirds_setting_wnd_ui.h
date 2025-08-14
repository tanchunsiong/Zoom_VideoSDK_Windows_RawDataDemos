#ifndef _LOWER_THIRD_SETTING_WND_UI_H_
#define _LOWER_THIRD_SETTING_WND_UI_H_

#include "zoom_videosdk_renderer_def.h"

class CLowerThirdsSettingWndUI : public WindowImplBase
{
public:
	CLowerThirdsSettingWndUI();
	~CLowerThirdsSettingWndUI();

	void UpdateControl();

	wstring GetLowerThirdConfPath();
	void GetLowerThirdInfoFromIniFile(std::wstring& username, std::wstring& description, std::wstring& color, std::wstring& enable);
	void SendLowerThirdSettingInfo(const std::wstring& username, const std::wstring& description, const std::wstring& color);
protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetWindowClassName() const { return _T("LowerThirdsSettingUI"); }
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void Notify(TNotifyUI& msg);

protected:
	virtual LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void InitControls();
	void DoCloseBtnClick();
	
	void DoSaveLowerThirdsClick();
	
	void HandleLowerThirdColorChange(CControlUI* pControl);
	bool CheckLowerThirdsInput();
	void UpdateSaveBtnEnable();
	void UpdatePreviewLowerThirdInfo();

	void ShowLowerThirdInfoSetting(bool show);
	void SwitchOptionColorByColorKey(const std::wstring& color);
	void SaveLowerThirdSettingInfo(const std::wstring & username, const std::wstring& description, const std::wstring& color);	
	
	void CheckIsNeedUpdateLowerThirdWnd();
	void ShowFullLayout(bool is_show_full);
	void OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:

	CDuiString  m_lowerThirdsNameStr, m_lowerThirdsTitleStr, m_lowerThirdColorStr;
	
	CVerticalLayoutUI* session_setting_layout_ = nullptr;
	CVerticalLayoutUI* lower_third_layout_ = nullptr;
	
	CHorizontalLayoutUI* lowerthird_info_setting_ = nullptr;
	CHorizontalLayoutUI* option_color_layout_ = nullptr;
	CControlUI* control_left_line_setting_ = nullptr;
	CLabelUI* label_thirds_name_setting_ = nullptr;
	CLabelUI* label_thirds_title_setting_ = nullptr;
	CControlUI* control_right_corner_setting_ = nullptr;

	CButtonUI* btn_save_lower_third_ = nullptr;
	CButtonUI* btn_cancel_lower_third_ = nullptr;
	CRichEditUI* edit_lower_third_name_ = nullptr;
	CRichEditUI* edit_lower_third_title_ = nullptr;

	COptionUI* option_color_none_ = nullptr;
	COptionUI* option_color_blue_ = nullptr;
	COptionUI* option_color_violet_ = nullptr;
	COptionUI* option_color_green_ = nullptr;
	COptionUI* option_color_orange_ = nullptr;
	COptionUI* option_color_red_ = nullptr;
	COptionUI* option_color_darkblue_ = nullptr;
	COptionUI* option_color_yellow_ = nullptr;

};

#endif