#pragma once

class CLeaveSessionTipsWndUI : public WindowImplBase
{
public:
	CLeaveSessionTipsWndUI();
	~CLeaveSessionTipsWndUI();

	void CheckIsHost();

protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetWindowClassName() const { return _T("VideoSDKDemoLeaveSessionUI"); }
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void Notify(TNotifyUI& msg);

private:
	void InitControls();
	void DoLeaveSessionBtnClick(bool bEndSession);

private:
	CButtonUI* btn_end_session_ = nullptr;
	CButtonUI* btn_leave_session_ = nullptr;

};