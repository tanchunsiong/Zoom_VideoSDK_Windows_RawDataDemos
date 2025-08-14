#ifndef _ROLL_CONTROL_UI_H_
#define _ROLL_CONTROL_UI_H_


#define ROLL_CONTROL_TIMERID			20
#define ROLL_CONTROL_TIMERID_SPAN		5

class CRollControlUI : public CControlUI
{
public:
	CRollControlUI();
	~CRollControlUI();
public:
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

public:	
	virtual void DoEvent(TEventUI& event);

public:
	void BeginRoll(LONG lTimeSpan = ROLL_CONTROL_TIMERID_SPAN, LONG lMaxTimeLimited = 60);
	void EndRoll();

private:
	void UpdateControlPos();
private:
	int m_nStep;
	BOOL m_bUseRoll;
};

#endif