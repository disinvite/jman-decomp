#ifndef GAMEWIN_H
#define GAMEWIN_H

#include <afxwin.h>

#include "btnbar.h"
#include "compass.h"
#include "invdata.h"
#include "msgwnd.h"
#include "navdata.h"
#include "viewscn.h"

class GameWindow : public CWnd {
public:
	GameWindow(CFrameWnd *, int, int);
	~GameWindow();

	MsgWnd *msgwnd_;         // 0x1c
	Viewscreen *viewscreen_; // 0x20
	Compass *compass_;       // 0x24
	BtnBar *btnbar_;         // 0x28
	// InvWnd // 0x2c
	NavData navdata_;        // 0x38
	NavMap navmap_;          // 0x42
	Inventory _inventory;    // 0x4e
	DWORD _energy_level;     // 0x76
	CFont _font;             // 0x88
	HWND hElight_;           // 0x8e
	int _energy_alert_level; // 0x90
	HBITMAP _bmp_fullscrn;   // 0x92
	int draw_level_;         // 0x94

	BOOL Setup(CFrameWnd *cf);
	BOOL AddToEnergy(long);
	DWORD GetEnergyLevel();
	BOOL CheckEnergyLevel();
	// FUN_1008_34a6
	BOOL CalculateScore();

protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()
};

#endif
