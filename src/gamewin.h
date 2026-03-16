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

	MsgWnd *m_msgWnd;         // 0x1c
	Viewscreen *m_viewScreen; // 0x20
	Compass *m_compass;       // 0x24
	BtnBar *m_btnBar;         // 0x28
	// InvWnd // 0x2c
	NavData m_navData;          // 0x38
	NavMap m_navMap;            // 0x42
	Inventory m_inventory;      // 0x4e
	DWORD m_energyLevel;        // 0x76
	CFont m_font;               // 0x88
	HWND m_eLight;              // 0x8e
	int m_energyAlertLevel;     // 0x90
	HBITMAP m_bitmapFullscreen; // 0x92
	int m_drawLevel;            // 0x94

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
