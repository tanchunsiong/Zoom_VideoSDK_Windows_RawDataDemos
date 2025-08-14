#include "stdafx.h"
#include "turn_page_wnd_ui.h"
#include "main_frame.h"

TurnPageWndUI::TurnPageWndUI():btn_up_page_(nullptr), btn_down_page_(nullptr)
{

}

TurnPageWndUI::~TurnPageWndUI()
{

}

void TurnPageWndUI::InitControls()
{
	btn_up_page_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_up_page")));
	btn_down_page_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_down_page")));
}

void TurnPageWndUI::InitWindow()
{
	InitControls();
}

CDuiString TurnPageWndUI::GetSkinFolder()
{
	return _T("");
}

CDuiString TurnPageWndUI::GetSkinFile()
{
	return to_wstring(IDXML_TURN_PAGE_WND_UI).c_str();
}

UILIB_RESOURCETYPE TurnPageWndUI::GetResourceType() const
{
	return UILIB_RESOURCE;
}

LRESULT TurnPageWndUI::ResponseDefaultKeyEvent(WPARAM wParam)
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

void TurnPageWndUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender == btn_up_page_)
		{
			DoTurnUpPageBtnClicked();
		}
		else if (msg.pSender == btn_down_page_)
		{
			DoTurnDownPageBtnClicked();
		}
	}
}

void TurnPageWndUI::DoTurnUpPageBtnClicked()
{
	CMainFrame::GetInstance().HandleTurnUpPageEvent();
}

void TurnPageWndUI::DoTurnDownPageBtnClicked()
{
	CMainFrame::GetInstance().HandleTurnDownPageEvent();
}

void TurnPageWndUI::HideUpPageBtn()
{
	if (btn_up_page_)
	{
		btn_up_page_->SetVisible(false);
	}
}

void TurnPageWndUI::HideDownPageBtn()
{
	if (btn_down_page_)
	{
		btn_down_page_->SetVisible(false);
	}
}