#ifndef _CHAT_RICH_EDIT_UI_H_
#define _CHAT_RICH_EDIT_UI_H_

class ChatSendEditUI : public CRichEditUI
{
public:
	ChatSendEditUI();
	~ChatSendEditUI();
	void DoInit();
public:
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

private:
	void SendChatToAll();

private:
	CButtonUI* btn_send_;
};

#endif //_CHAT_RICH_EDIT_UI_H_