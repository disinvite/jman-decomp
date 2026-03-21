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
	BOOL SetTimers();
	BOOL KillTimers();
	JScene *_current_scene; // 0x1c
	NavData *m_navData;     // 0x20
	Compass *m_compass;     // 0x24
	UINT m_timer0;          // 0x28
	UINT m_timer1;          // 0x2a
	UINT m_timer2;          // 0x2c
	UINT m_timer3;          // 0x2e

	void MovePlayer(int);
	BOOL NewArea(int scene, int area, int dir);

protected:
	afx_msg void OnLButtonUp(UINT, CPoint);
	afx_msg void OnLButtonDown(UINT, CPoint);
	afx_msg void OnMouseMove(UINT, CPoint);
	afx_msg void OnTimer(UINT);
	afx_msg LRESULT OnMCIError(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};

#endif
