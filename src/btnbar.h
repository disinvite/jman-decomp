#ifndef BTNBAR_H
#define BTNBAR_H

#include <afxwin.h>

// VTABLE: JMAN10 0x1008a014
class BtnBar : public CWnd {
public:
	BtnBar(CWnd *, int, int);
	~BtnBar();

	HBITMAP m_bitmap;    // 0x1c
	UINT m_activeButton; // 0x1e
	BOOL m_unk0x20;      // 0x20

protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT, CPoint);
	DECLARE_MESSAGE_MAP()
};

#endif
