#include "stdafx.h"
#include "chat_send_edit_ui.h"
#include "main_frame.h"

ChatSendEditUI::ChatSendEditUI():btn_send_(nullptr)
{

}

ChatSendEditUI::~ChatSendEditUI()
{

}

void ChatSendEditUI::DoInit()
{
	__super::DoInit();
}

void ChatSendEditUI::SendChatToAll()
{
	CDuiString text = GetText();
	SetText(_T(""));

	CMainFrame::GetInstance().SendChatToAll(text.GetData());
	SetFocus();
}

LRESULT ChatSendEditUI::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if (uMsg == WM_SETFOCUS)
	{
		CMainFrame::GetInstance().KillBottomBarHideTimer();
	}
	else if (uMsg == WM_KILLFOCUS)
	{
		CMainFrame::GetInstance().SetBottomBarHideTimer();
	}
	else if (uMsg == WM_KEYUP)
	{
		if (wParam == VK_RETURN)
		{
			SendChatToAll();
		}
	}

	return __super::MessageHandler(uMsg, wParam, lParam, bHandled);
}