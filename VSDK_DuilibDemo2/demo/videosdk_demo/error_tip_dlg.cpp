#include "stdafx.h"
#include "error_tip_dlg.h"

ErrorTipDlg::ErrorTipDlg():label_error_tip_(nullptr)
{

}

ErrorTipDlg::~ErrorTipDlg()
{

}

void ErrorTipDlg::SetErrorTipText(wstring text)
{
	error_tip_text_ = text;
}

void ErrorTipDlg::InitControls()
{
	label_error_tip_ = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_error_tip")));

	if (label_error_tip_)
	{
		label_error_tip_->SetText(error_tip_text_.c_str());
	}
}

void ErrorTipDlg::InitWindow()
{
	InitControls();
}

CDuiString ErrorTipDlg::GetSkinFolder()
{
	return _T("");
}

CDuiString ErrorTipDlg::GetSkinFile()
{
	return to_wstring(IDXML_ERROR_TIP_DLG_UI).c_str();
}

UILIB_RESOURCETYPE ErrorTipDlg::GetResourceType() const
{
	return UILIB_RESOURCE;
}

LRESULT ErrorTipDlg::ResponseDefaultKeyEvent(WPARAM wParam)
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

void ErrorTipDlg::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btn_close"))
		{
			DoCloseBtnClicked();
		}
		else if (msg.pSender->GetName() == _T("btn_ok"))
		{
			DoOkBtnClicked();
		}
	}

	__super::Notify(msg);
}

void ErrorTipDlg::DoCloseBtnClicked()
{
	Close();
}

void ErrorTipDlg::DoOkBtnClicked()
{
	Close();
}