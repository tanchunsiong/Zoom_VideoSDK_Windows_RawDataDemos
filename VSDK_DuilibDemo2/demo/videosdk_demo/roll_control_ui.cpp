#include "stdafx.h"
#include "roll_control_ui.h"
#include "main_frame.h"

CRollControlUI::CRollControlUI(void)
{
	m_nStep = 2;
	m_bUseRoll = FALSE;
}

CRollControlUI::~CRollControlUI(void)
{
	m_pManager->KillTimer(this, ROLL_CONTROL_TIMERID);
}

LPCTSTR CRollControlUI::GetClass() const
{
	return _T("RollControlUI");
}

LPVOID CRollControlUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, _T("RollControl")) == 0) return static_cast<CRollControlUI*>(this);
	return CControlUI::GetInterface(pstrName);
}

void CRollControlUI::BeginRoll(LONG lTimeSpan, LONG lMaxTimeLimited)
{
	if (m_bUseRoll)
	{
		EndRoll();
	}

	if (!m_pManager)
	{
		return;
	}
	m_pManager->KillTimer(this, ROLL_CONTROL_TIMERID);
	m_pManager->SetTimer(this, ROLL_CONTROL_TIMERID, lTimeSpan);

	m_bUseRoll = TRUE;
}

void CRollControlUI::EndRoll()
{
	if (!m_bUseRoll) return;

	m_pManager->KillTimer(this, ROLL_CONTROL_TIMERID);

	m_bUseRoll = FALSE;
}

void CRollControlUI::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_TIMER && event.wParam == ROLL_CONTROL_TIMERID)
	{
		UpdateControlPos();
		Invalidate();
		return;
	}
	CControlUI::DoEvent(event);
}

void CRollControlUI::UpdateControlPos()
{
	CRect rc = this->GetPos();
	rc.left -= m_nStep;
	rc.right -= m_nStep;
	this->SetPos(rc);

	if (rc.right <= 0)
		EndRoll();
}