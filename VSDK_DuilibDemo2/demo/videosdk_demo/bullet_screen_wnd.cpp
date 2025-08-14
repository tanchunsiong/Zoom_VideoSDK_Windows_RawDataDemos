#include "stdafx.h"
#include "bullet_screen_wnd.h"
#include "main_frame.h"
#include "roll_control_ui.h"

#define ROLL_CONTROL_WIDTH		32
#define TITLE_WND_HEIGHT		50

CBulletScreenWnd::CBulletScreenWnd()
{
}

CBulletScreenWnd::~CBulletScreenWnd()
{
}

CControlUI* CBulletScreenWnd::CreateControl(LPCTSTR pstrClass)
{
	if (_tcscmp(pstrClass, _T("RollControl")) == 0)
	{
		return new CRollControlUI();		
	}

	return NULL;
}

void CBulletScreenWnd::InitWindow()
{
	InitControl();
	InitEmojiImageMap();
}

void CBulletScreenWnd::InitControl()
{
	m_pBulletScreenLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("bullet_screen_layout")));
}

void CBulletScreenWnd::InitEmojiImageMap()
{
	m_emojiImageMap.clear();
	
	m_emojiImageMap.insert(make_pair(_T("clap"), _T("BUTTON_EMOJI_CLAP")));
	m_emojiImageMap.insert(make_pair(_T("thumbsup"), _T("BUTTON_EMOJI_THUMBSUP")));
	m_emojiImageMap.insert(make_pair(_T("joy"), _T("BUTTON_EMOJI_JOY")));
	m_emojiImageMap.insert(make_pair(_T("hushed"), _T("BUTTON_EMOJI_HUSHED")));
	m_emojiImageMap.insert(make_pair(_T("heart"), _T("BUTTON_EMOJI_HEART")));
	m_emojiImageMap.insert(make_pair(_T("tada"), _T("BUTTON_EMOJI_TADA")));
	m_emojiImageMap.insert(make_pair(_T("raisehand"), _T("BUTTON_EMOJI_RAISEDHAND")));
	m_emojiImageMap.insert(make_pair(_T("lowerhand"), _T("BUTTON_EMOJI_RAISEDHAND")));
}

CDuiString CBulletScreenWnd::GetRandomPos()
{
	CDuiRect rcClient;
	GetClientRect(CMainFrame::GetInstance().GetHWND(), &rcClient);

	POINT ptLeftTop = { rcClient.left,rcClient.top };
	POINT ptRightBottom = { rcClient.right,rcClient.bottom };
	ClientToScreen(CMainFrame::GetInstance().GetHWND(), &ptLeftTop);
	ClientToScreen(CMainFrame::GetInstance().GetHWND(), &ptRightBottom);
	int client_width = ptRightBottom.x - ptLeftTop.x;
	int client_height = ptRightBottom.y - ptLeftTop.y;

	CDuiString strPos;
	CRect rc = { 0 };
	int nRowConut = (client_height - TITLE_WND_HEIGHT) / ROLL_CONTROL_WIDTH - 1;

	if (nRowConut <= 0)	
		nRowConut = 1;
	
	int lineNum = rand() % nRowConut;

	rc.left = client_width;
	rc.right = client_width + ROLL_CONTROL_WIDTH;
	rc.top = lineNum * ROLL_CONTROL_WIDTH - ROLL_CONTROL_WIDTH;
	rc.bottom = lineNum * ROLL_CONTROL_WIDTH;

	strPos.Format(_T("%d,%d,%d,%d"), rc.left,rc.top, rc.right, rc.bottom);//
	return strPos;
}

void CBulletScreenWnd::OnParseChannelCmd(IZoomVideoSDKUser* pSender, const std::vector<std::wstring >& cmdVector)
{
	if (cmdVector.size() < 2)
		return;

	std::wstring emojiStr = cmdVector[1];
	if (emojiStr.empty())
		return;

	if (0 == m_emojiImageMap.count(emojiStr))
		return;

	CMainFrame::GetInstance().ShowBulletScreenWnd(true);
}

void CBulletScreenWnd::RemoveAllRoolControl()
{
	if (m_pBulletScreenLayout)
		m_pBulletScreenLayout->RemoveAll();	
}

void CBulletScreenWnd::AddRoolControl(const std::wstring& buf)
{	
	if (buf.empty())
		return;

	CRollControlUI* pRollControlUI = new CRollControlUI();
	if (!pRollControlUI) return;

	pRollControlUI->SetAttribute(_T("float"), _T("true"));
	pRollControlUI->SetAttribute(_T("width"), _T("32"));
	pRollControlUI->SetAttribute(_T("height"), _T("32"));
	
	CDuiString emojiImgStr;
	emojiImgStr.Format(_T("res='%s' restype='ZPIMGRES'"), buf.data());
	pRollControlUI->SetAttribute(_T("bkimage"), emojiImgStr.GetData());
	
	CDuiString randomPos = this->GetRandomPos();
	pRollControlUI->SetAttribute(_T("pos"), randomPos.GetData());
	
	if (m_pBulletScreenLayout)
		m_pBulletScreenLayout->Add(pRollControlUI);

	pRollControlUI->SetManager(&m_PaintManager, m_pBulletScreenLayout);

	pRollControlUI->BeginRoll();
}

LRESULT CBulletScreenWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_ADD_ROLLCONTROL:
	{
		std::wstring* buf = (std::wstring*)lParam;
		this->AddRoolControl(*buf);
		delete buf;
		break;
	}
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

void CBulletScreenWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		;
	}

	__super::Notify(msg);
}
