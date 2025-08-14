#ifndef _ERROR_TIPS_WND_UI_H_
#define _ERROR_TIPS_WND_UI_H_

class MessageTipWndUI : public WindowImplBase
{
public:
	MessageTipWndUI();
	~MessageTipWndUI();

	void SetErrorTipText(wstring text);
	int GetWndWidth();

protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetWindowClassName() const { return _T("ErrorTipWndUI"); }
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:
	void InitControls();

private:
	CLabelUI* label_error_tip_;
};

#endif  //_ERROR_TIPS_WND_UI_H_