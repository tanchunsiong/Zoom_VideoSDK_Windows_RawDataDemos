#ifndef _TURN_PAGE_WND_UI_H_
#define _TURN_PAGE_WND_UI_H_

class TurnPageWndUI : public WindowImplBase
{
public:
	TurnPageWndUI();
	~TurnPageWndUI();

	void HideUpPageBtn();
	void HideDownPageBtn();
protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetWindowClassName() const { return _T("TurnPageWndUI"); }
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void Notify(TNotifyUI& msg);

private:
	void InitControls();
	void DoTurnUpPageBtnClicked();
	void DoTurnDownPageBtnClicked();
private:
	CButtonUI* btn_up_page_;
	CButtonUI* btn_down_page_;
};

#endif  //_TURN_PAGE_WND_UI_H_