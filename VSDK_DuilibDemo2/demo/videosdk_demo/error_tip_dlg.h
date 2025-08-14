#ifndef _ERROR_TIP_DLG_UI_H_
#define _ERROR_TIP_DLG_UI_H_

class ErrorTipDlg : public WindowImplBase
{
public:
	ErrorTipDlg();
	~ErrorTipDlg();

	void SetErrorTipText(wstring text);

protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetWindowClassName() const { return _T("ErrorTipDlgUI"); }
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void Notify(TNotifyUI& msg);

private:
	void InitControls();
	void DoCloseBtnClicked();
	void DoOkBtnClicked();

private:
	CLabelUI* label_error_tip_;
	wstring error_tip_text_;
};

#endif //_ERROR_TIP_DLG_UI_H_