#pragma once

class CControlCameraTipWndUI : public WindowImplBase
{
public:
	CControlCameraTipWndUI();
	~CControlCameraTipWndUI();

protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetWindowClassName() const { return _T("VideoSDKDemoControlCameraTipUI"); }
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void Notify(TNotifyUI& msg);


private:

	void InitControls();
private:
	CLabelUI* label_control_camera_tip_ = nullptr;
};