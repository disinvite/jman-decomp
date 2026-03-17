#ifndef MENU_H
#define MENU_H

#include <afxwin.h>

// SIZE 0x45c
class Menu : public CWnd {
protected:
	DECLARE_DYNAMIC(Menu)

public:
	Menu(CFrameWnd *);
	~Menu();

	HBITMAP m_menu0;
	HBITMAP m_menu1;
	HBITMAP m_menu2;
	RECT m_rect0;
	RECT m_rect1;
	RECT m_rect2;
	RECT m_rect3;
	RECT m_rect4;
	RECT m_rect5;
	RECT m_rect6;

	int m_selectedOpt;        // 0x5a
	char m_rolloverBuf[1024]; // 0x5c size?

protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT, CPoint);
	afx_msg void OnLButtonUp(UINT, CPoint);
	afx_msg void OnMouseMove(UINT, CPoint);
	afx_msg BOOL OnEraseBkgnd(CDC *);
	DECLARE_MESSAGE_MAP()
};

#endif
