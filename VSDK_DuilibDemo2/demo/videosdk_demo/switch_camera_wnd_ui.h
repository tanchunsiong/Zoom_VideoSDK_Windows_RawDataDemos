#pragma once

class CSwitchCameraWndUI : public WindowImplBase
{
public:
	CSwitchCameraWndUI();
	~CSwitchCameraWndUI();

protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetWindowClassName() const { return _T("VideoSDKDemoSwitchCameraUI"); }
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void Notify(TNotifyUI& msg);

private:

	void InitControls();
private:

	CButtonUI* btn_switch_camera_ = nullptr;
};