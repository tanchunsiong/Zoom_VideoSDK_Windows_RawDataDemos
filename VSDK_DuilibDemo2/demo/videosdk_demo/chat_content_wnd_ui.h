#ifndef _SHOW_CHAT_CONTENT_WND_UI_H_
#define _SHOW_CHAT_CONTENT_WND_UI_H_

class ChatContentWndUI : public WindowImplBase
{
public:
	ChatContentWndUI();
	~ChatContentWndUI();

	void InsertChatContent(const zchar_t* user_name,const zchar_t* content);
	void ClearChatContent();
	bool HasChatContent() { return has_chat_content_; }
protected:
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetWindowClassName() const { return _T("ChatContentWndUI"); }
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
private:
	void InitControls();
	SIZE CalContentItemSize(int text_width);
	wstring GetCurrentFormatTime();
private:
	CRichEditUI* chat_content_edit_ui_;
	CButtonUI* send_content_btn_ui_;
	bool has_chat_content_;
};

#endif //_SHOW_CHAT_CONTENT_WND_UI_H_