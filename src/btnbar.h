#ifndef BTNBAR_H
#define BTNBAR_H

#include <afxwin.h>

// VTABLE: JMAN10 0x1008a014
class BtnBar : public CWnd {
public:
	BtnBar(CFrameWnd *, int, int);
	~BtnBar();

	HBITMAP bmp_;     // 0x1c
	UINT active_btn_; // 0x1e
	BOOL prop_20_;    // 0x20

protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT, CPoint);
	DECLARE_MESSAGE_MAP()
};

#endif
