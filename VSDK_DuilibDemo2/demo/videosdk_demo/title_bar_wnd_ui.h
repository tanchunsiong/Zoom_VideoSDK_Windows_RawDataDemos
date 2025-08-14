#pragma once
//#include "zoom_video_sdk_include_interface.h"
class CTitleBarWndUI : public WindowImplBase
{
public:
	CTitleBarWndUI();
	~CTitleBarWndUI();

	void UpdateSessionName(const zchar_t* szSessionName);

protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetWindowClassName() const { return _T("VideoSDKDemoTitleBarUI"); }
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void Notify(TNotifyUI& msg);

private:

	void InitControls();
	void DoShowSessionInfoBtnClick();
	void DoCloseBtnClick();
	void DoMinimizeBtnClick();
	void DoFullScreenBtnClick();
private:

	CButtonUI* btn_session_name_ = nullptr;
};