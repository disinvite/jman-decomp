#ifndef MENU_H
#define MENU_H

#include <afxwin.h>

class Menu : public CWnd {
protected:
	DECLARE_DYNAMIC(Menu)

public:
	Menu(CFrameWnd *);
	~Menu();

	HBITMAP _menu0;
	HBITMAP _menu1;
	HBITMAP _menu2;
	RECT _rect0;
	RECT _rect1;
	RECT _rect2;
	RECT _rect3;
	RECT _rect4;
	RECT _rect5;
	RECT _rect6;

	int _selected_opt;      // 0x5a
	char _rollover_buf[64]; // 0x5c size?

protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT, CPoint);
	afx_msg void OnLButtonUp(UINT, CPoint);
	afx_msg void OnMouseMove(UINT, CPoint);
	afx_msg BOOL OnEraseBkgnd(CDC *);
	DECLARE_MESSAGE_MAP()
};

#endif
