#include "stdafx.h"
#include "top_tip_wnd_ui.h"

TopTipWndUI::TopTipWndUI():label_top_tip_(nullptr)
{

}

TopTipWndUI::~TopTipWndUI()
{

}

void TopTipWndUI::InitControls()
{
	label_top_tip_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_top_tip")));
}

void TopTipWndUI::InitWindow()
{
	InitControls();
}

CDuiString TopTipWndUI::GetSkinFolder()
{
	return _T("");
}

CDuiString TopTipWndUI::GetSkinFile()
{
	return to_wstring(IDXML_TOP_TIP_WND_UI).c_str();
}

UILIB_RESOURCETYPE TopTipWndUI::GetResourceType() const
{
	return UILIB_RESOURCE;
}

LRESULT TopTipWndUI::ResponseDefaultKeyEvent(WPARAM wParam)
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

void TopTipWndUI::SetTopTipText(wstring text)
{
	if (label_top_tip_)
	{
		label_top_tip_->SetText(text.c_str());
	}
}

int TopTipWndUI::GetWndWidth()
{
	if (!label_top_tip_)
		return 0;

	int width = 0;

	int ifont = label_top_tip_->GetFont();
	int font_style = label_top_tip_->GetTextStyle();

	HDC hdc = GetDC(m_hWnd);
	SIZE text_size = CRenderEngine::GetTextSize(hdc, &m_PaintManager, label_top_tip_->GetText(), ifont,font_style);
	ReleaseDC(m_hWnd, hdc);

	RECT label_padding = label_top_tip_->GetPadding();
	width = text_size.cx + label_padding.left + label_padding.right;
	return width;
}