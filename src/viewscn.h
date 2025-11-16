#ifndef VIEWSCN_H
#define VIEWSCN_H

#include "jscene.h"

class Viewscreen : public CWnd {
protected:
	DECLARE_DYNAMIC(Viewscreen)

public:
	Viewscreen();
	BOOL SetTimers();
	BOOL KillTimers();
	JScene *_current_scene; // 0x1c

	UINT _timer0; // 0x28
	UINT _timer1; // 0x2a
	UINT _timer2; // 0x2c
	UINT _timer3; // 0x2e

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
