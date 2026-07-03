#ifndef JMANWIN_H
#define JMANWIN_H

#include <afxwin.h>

// VTABLE: JMAN10 0x10081082
// SIZE 0x62
class JmanWindow : public CFrameWnd {
public:
	JmanWindow();
	~JmanWindow();
	BOOL ShowMenu();
	BOOL ShowTitles();
	BOOL ShowSepia();
	BOOL ShowDream();
	BOOL NewGame();

	BOOL m_gameShouldExit; // 0x5a
	CWnd *m_curWindow;     // 0x5c
	char m_padding[2];     // 0x60

protected:
	afx_msg void OnKeyDown(UINT, UINT, UINT);
	afx_msg void OnKeyUp(UINT, UINT, UINT);
	DECLARE_MESSAGE_MAP()
};

// GLOBAL: JMAN10 0x10000000
// ?_messageEntries@JmanWindow@@0QEUAFX_MSGMAP_ENTRY@@E

#endif
