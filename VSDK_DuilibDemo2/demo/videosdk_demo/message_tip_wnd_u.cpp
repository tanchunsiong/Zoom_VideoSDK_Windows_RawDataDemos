#include "stdafx.h"
#include "message_tip_wnd_ui.h"

MessageTipWndUI::MessageTipWndUI():label_error_tip_(nullptr)
{

}

MessageTipWndUI::~MessageTipWndUI()
{

}

void MessageTipWndUI::InitControls()
{
	label_error_tip_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_error_tip")));
}

void MessageTipWndUI::InitWindow()
{
	InitControls();
}

CDuiString MessageTipWndUI::GetSkinFolder()
{
	return _T("");
}

CDuiString MessageTipWndUI::GetSkinFile()
{
	return to_wstring(IDXML_MESSAGE_TIP_WND_UI).c_str();
}

UILIB_RESOURCETYPE MessageTipWndUI::GetResourceType() const
{
	return UILIB_RESOURCE;
}

LRESULT MessageTipWndUI::ResponseDefaultKeyEvent(WPARAM wParam)
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

void MessageTipWndUI::SetErrorTipText(wstring text)
{
	if (label_error_tip_)
	{
		label_error_tip_->SetText(text.c_str());
	}
}

int MessageTipWndUI::GetWndWidth()
{
	if (!label_error_tip_)
		return 0;

	int width = 0;

	int ifont = label_error_tip_->GetFont();
	int font_style = label_error_tip_->GetTextStyle();

	HDC hdc = GetDC(m_hWnd);
	SIZE text_size = CRenderEngine::GetTextSize(hdc, &m_PaintManager, label_error_tip_->GetText(), ifont,font_style);
	ReleaseDC(m_hWnd, hdc);

	RECT label_padding = label_error_tip_->GetPadding();
	width = text_size.cx + label_padding.left + label_padding.right;
	return width;
}