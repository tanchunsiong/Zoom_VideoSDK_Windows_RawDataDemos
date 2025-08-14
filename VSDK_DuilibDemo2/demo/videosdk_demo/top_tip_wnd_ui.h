#ifndef _TOP_TIPS_WND_UI_H_
#define _TOP_TIPS_WND_UI_H_

class TopTipWndUI : public WindowImplBase
{
public:
	TopTipWndUI();
	~TopTipWndUI();

	void SetTopTipText(wstring text);
	int GetWndWidth();

protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetWindowClassName() const { return _T("TopTipWndUI"); }
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

private:
	void InitControls();

private:
	CLabelUI* label_top_tip_;
};

#endif  //_ERROR_TIPS_WND_UI_H_