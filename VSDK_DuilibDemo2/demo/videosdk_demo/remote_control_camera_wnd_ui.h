#pragma once
//#include "zoom_video_sdk_include_interface.h"
class CRemoteControlCameraWndUI : public WindowImplBase
{
public:
	CRemoteControlCameraWndUI();
	~CRemoteControlCameraWndUI();

	void DoClickCloseBtn();
	void DoClickZoomInBtn();
	void DoClickZoomOutBtn();
	void DoClickTurnUpBtn();
	void DoClickTurnDownBtn();
	void DoClickTurnLeftBtn();
	void DoClickTurnRightBtn();

protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetWindowClassName() const { return _T("VideoSDKDemoRemoteControlCameraUI"); }
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void Notify(TNotifyUI& msg);

private:

	void InitControls();
private:

	CButtonUI* btn_close_ = nullptr;
	CButtonUI* btn_zoom_in_ = nullptr;
	CButtonUI* btn_zoom_out_ = nullptr;
	CButtonUI* btn_turn_up_ = nullptr;
	CButtonUI* btn_turn_down_ = nullptr;
	CButtonUI* btn_turn_left_ = nullptr;
	CButtonUI* btn_turn_right_ = nullptr;
};