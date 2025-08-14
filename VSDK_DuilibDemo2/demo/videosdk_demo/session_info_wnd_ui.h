#pragma once

class CSessionInfoWndUI : public WindowImplBase
{
public:
	CSessionInfoWndUI();
	~CSessionInfoWndUI();

	void UpdateSessionInfoUI();

protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetWindowClassName() const { return _T("VideoSDKDemoSessionInfoUI"); }
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
public:

	virtual LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

private:
	void InitControls();

private:
	CLabelUI* label_session_name_ = nullptr;
	CLabelUI* label_participants_num_ = nullptr;
};