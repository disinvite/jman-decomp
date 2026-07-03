#ifndef VIEWSCN_H
#define VIEWSCN_H

#include "compass.h"
#include "invdata.h"
#include "jscene.h"
#include "navdata.h"

// SIZE 0x550
class Viewscreen : public CWnd {
protected:
	DECLARE_DYNAMIC(Viewscreen)

public:
	Viewscreen(CWnd *, int, int, NavData *, NavMap *, Inventory *, int, int, DWORD);

	void MovePlayer(int);
	BOOL NewArea(int scene, int area, int dir);
	int GetCurrentSceneArea();
	BOOL SetTimers();
	BOOL KillTimers();

private:
	JScene *m_curScene;             // 0x01c
	NavData *m_navData;             // 0x020
	Compass *m_compass;             // 0x024
	UINT m_timer0;                  // 0x028
	UINT m_timer1;                  // 0x02a
	UINT m_timer2;                  // 0x02c
	UINT m_timer3;                  // 0x02e
	BYTE m_unk0x30[0x448 - 0x30];   // 0x030
	BYTE m_unk0x448;                // 0x448
	BYTE m_unk0x449[0x548 - 0x449]; // 0x449
	UINT m_unk0x548;                // 0x548
	BYTE m_unk0x54a[0x550 - 0x54a]; // 0x54a

protected:
	afx_msg void OnLButtonUp(UINT, CPoint);
	afx_msg void OnLButtonDown(UINT, CPoint);
	afx_msg void OnMouseMove(UINT, CPoint);
	afx_msg void OnTimer(UINT);
	afx_msg LRESULT OnMCIError(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};

#endif
