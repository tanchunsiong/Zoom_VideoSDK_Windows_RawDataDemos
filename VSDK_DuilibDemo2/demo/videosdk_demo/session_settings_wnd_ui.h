#pragma once

#include "zoom_videosdk_renderer_def.h"

class CSessionSettingsWndUI : public WindowImplBase
{
public:
	CSessionSettingsWndUI();
	~CSessionSettingsWndUI();

	void UpdateSettingsInfoUI();

	void UpdateControl();
	
	bool IsShowLowerThirdsInfoWnd();
	wstring GetToken();	
	VideoSDKRendererType GetRendererType();

protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetWindowClassName() const { return _T("VideoSDKDemoSettingUI"); }
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void Notify(TNotifyUI& msg);

protected:
	virtual LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	void UpdateCameraListUI();
	bool SelectCamera(const zchar_t* cameraDeviceID);

	void UpdateSpeakerListUI();
	bool SelectSpeaker(const zchar_t* devidceID, const zchar_t* deviceName);

	void UdateMicrophoneListUI();
	bool SelectMicrophone(const zchar_t* devidceID, const zchar_t* deviceName);


	void ShowLowerThirdInfo(bool show);

	void CheckCursorRefresh(bool is_window_show);
private:
	void InitControls();
	void DoCloseBtnClick();
	void DoRenderModeSelect();
	void DoCustomLowerThirdClick();
	void HandleCameraSelected();
	void HandleMicSelected();
	void HandleSpeakerSelected();
	void HandleEnableLowerThirdChange();

	void OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:

	CComboUI* combo_camera_list_ = nullptr;	
	CComboUI* combo_speaker_list_ = nullptr;
	CComboUI* combo_mic_list_ = nullptr;
	CComboUI* combo_renderer_type_ = nullptr;
	CRichEditUI* edit_token_ = nullptr;
	CLabelUI* label_empty_tip_ = nullptr;
	CVerticalLayoutUI* token_layout_ = nullptr;
	CVerticalLayoutUI* statistics_layout_ = nullptr;
	CCheckBoxUI* chk_fill_frame_ = nullptr;
	CCheckBoxUI* chk_enable_lower_third_ = nullptr;
	CButtonUI* btn_custom_lower_third_ = nullptr;

	CHorizontalLayoutUI* lowerthird_info_ = nullptr;
	CControlUI* control_left_line_ = nullptr;
	CLabelUI* label_thirds_name_ = nullptr;
	CLabelUI* label_thirds_title_ = nullptr;
	CControlUI* control_right_corner_ = nullptr;
	CVerticalLayoutUI* client_layout_ = nullptr;
};