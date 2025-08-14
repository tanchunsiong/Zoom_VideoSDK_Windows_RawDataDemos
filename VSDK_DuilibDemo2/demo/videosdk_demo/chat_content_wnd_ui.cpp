#include "stdafx.h"
#include "chat_content_wnd_ui.h"

#define CONTENT_ITEM_HEIGHT 30
#define CONTENT_USER_NAME_COLOR  RGB(186, 218, 183)
#define CONTENT_CONTENT_COLOR  RGB(255, 255, 255)

ChatContentWndUI::ChatContentWndUI():chat_content_edit_ui_(nullptr), send_content_btn_ui_(nullptr), has_chat_content_(false)
{

}

ChatContentWndUI::~ChatContentWndUI()
{

}

void ChatContentWndUI::InitControls()
{
	chat_content_edit_ui_ = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("view_richedit")));
	send_content_btn_ui_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_chat_send")));

	chat_content_edit_ui_->HideSelection(true);
}

void ChatContentWndUI::InitWindow()
{
	InitControls();
}

CDuiString ChatContentWndUI::GetSkinFolder()
{
	return _T("");
}

CDuiString ChatContentWndUI::GetSkinFile()
{
	return to_wstring(IDXML_SHOW_CHAT_CONTENT_WND_UI).c_str();
}

UILIB_RESOURCETYPE ChatContentWndUI::GetResourceType() const
{
	return UILIB_RESOURCE;
}

LRESULT ChatContentWndUI::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

SIZE ChatContentWndUI::CalContentItemSize(int text_width)
{
	SIZE item_size = { 0 };

	RECT rect_client;
	GetClientRect(m_hWnd,&rect_client);

	int client_width = rect_client.right - rect_client.left;
	if (client_width <= 0)
		return item_size;

	if (text_width < client_width)
	{
		item_size.cx = text_width;
	}
	else
	{	
		item_size.cx = client_width;	
	}

	int row = text_width / client_width;
	item_size.cy = (row + 1) * CONTENT_ITEM_HEIGHT;

	return item_size;
}

wstring ChatContentWndUI::GetCurrentFormatTime()
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	TCHAR chTimes[64] = { 0 };
	wsprintf(chTimes, _T("%04d-%02d-%02d %02d:%02d:%02d"), sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);  
	return chTimes;
}

void ChatContentWndUI::InsertChatContent(const zchar_t* user_name, const zchar_t* content)
{
	if (!chat_content_edit_ui_)
		return;

	long sel_begin = 0, sel_end = 0;
	CHARFORMAT2 cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT2));   
	cf.cbSize = sizeof(cf);
	cf.dwReserved = 0;
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = CONTENT_USER_NAME_COLOR;
	cf.dwEffects = 0;

	sel_end = sel_begin = chat_content_edit_ui_->GetTextLength();
	chat_content_edit_ui_->SetSel(sel_end, sel_end);
	chat_content_edit_ui_->ReplaceSel(user_name, false);

	chat_content_edit_ui_->SetSel(-1, -1);
	chat_content_edit_ui_->ReplaceSel(_T(":\n"), false);

	sel_end = chat_content_edit_ui_->GetTextLength();
	chat_content_edit_ui_->SetSel(sel_begin, sel_end);
	chat_content_edit_ui_->SetSelectionCharFormat(cf);

	PARAFORMAT2 pf;
	ZeroMemory(&pf, sizeof(PARAFORMAT2));    
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT | PFM_LINESPACING;
	pf.dxStartIndent = 0;
	chat_content_edit_ui_->SetParaFormat(pf);

	sel_end = sel_begin = chat_content_edit_ui_->GetTextLength();

	chat_content_edit_ui_->SetSel(-1, -1);
	chat_content_edit_ui_->ReplaceSel(content, false);

	chat_content_edit_ui_->SetSel(-1, -1);
	chat_content_edit_ui_->ReplaceSel(_T("\n"), false);

	cf.crTextColor = CONTENT_CONTENT_COLOR;
	sel_end = chat_content_edit_ui_->GetTextLength();
	chat_content_edit_ui_->SetSel(sel_begin, sel_end);
	chat_content_edit_ui_->SetSelectionCharFormat(cf);

	ZeroMemory(&pf, sizeof(PARAFORMAT2));   
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT | PFM_LINESPACING;
	pf.dxStartIndent = 0;
	chat_content_edit_ui_->SetParaFormat(pf);

	chat_content_edit_ui_->EndDown();

	chat_content_edit_ui_->HideSelection(true);
	has_chat_content_ = true;
}

void ChatContentWndUI::ClearChatContent()
{
	if (chat_content_edit_ui_)
	{
		chat_content_edit_ui_->SetText(_T(""));
		has_chat_content_ = false;
	}
}