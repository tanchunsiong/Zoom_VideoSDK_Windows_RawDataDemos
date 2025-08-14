#pragma once

class CStopCameraControlWndUI : public WindowImplBase
{
public:
	CStopCameraControlWndUI();
	~CStopCameraControlWndUI();

	//void SetLabelText(std::wstring text);
	//void SetCameraControlRequestHandler(IZoomVideoSDKCameraControlRequestHandler* handler);

protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetWindowClassName() const { return _T("VideoSDKDemoStopCameraControlUI"); }
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void Notify(TNotifyUI& msg);
	

private:
	void InitControls();
	void DoNotStopBtnClick();
	void DoStopBtnClick();
	

private:
	CButtonUI* btn_donot_stop_ = nullptr;
	CButtonUI* btn_stop_ = nullptr;
};