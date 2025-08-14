#include "stdafx.h"
#include "control_camera_pop_wnd_ui.h"
#include "main_frame.h"
#include "videosdk_demo_mgr.h"

CControlCameraPopWnd::CControlCameraPopWnd()
{
}

CControlCameraPopWnd::~CControlCameraPopWnd()
{
}


void CControlCameraPopWnd::InitControl()
{	
	label_text_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_text")));
}

void CControlCameraPopWnd::InitWindow()
{	
	InitControl();
} 
void CControlCameraPopWnd::SetText(std::wstring text)
{
	TCHAR szTmp[1024 * 2] = { 0 };
	wsprintf(szTmp, _T("%s"), text.c_str());      
	label_text_->SetText(szTmp);
}
int CControlCameraPopWnd::GetWndWidth()
{
	if (!label_text_)
		return 0;

	int width = 0;

	int ifont = label_text_->GetFont();
	int font_style = label_text_->GetTextStyle();

	HDC hdc = GetDC(m_hWnd);
	SIZE text_size = CRenderEngine::GetTextSize(hdc, &m_PaintManager, label_text_->GetText(), ifont, font_style);
	ReleaseDC(m_hWnd, hdc);

	RECT label_padding = label_text_->GetPadding();
	width = text_size.cx + label_padding.left + label_padding.right;
	return width;
}

