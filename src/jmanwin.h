#ifndef JMANWIN_H
#define JMANWIN_H

#include <afxwin.h>

// VTABLE: JMAN10 0x10081082
class JmanWindow : public CFrameWnd {
public:
	JmanWindow();
	~JmanWindow();
	BOOL ShowMenu();
	BOOL ShowTitles();
	BOOL ShowSepia();
	BOOL ShowDream();
	BOOL NewGame();

	BOOL m_gameShouldExit;
	CWnd *m_curWindow;

protected:
	afx_msg void OnKeyDown(UINT, UINT, UINT);
	afx_msg void OnKeyUp(UINT, UINT, UINT);
	DECLARE_MESSAGE_MAP()
};

// GLOBAL: JMAN10 0x10000000
// JmanWindow::_messageEntries

#endif
