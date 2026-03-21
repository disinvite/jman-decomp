#ifndef INVWND_H
#define INVWND_H

#include <afxwin.h>

#include "invdata.h"

// VTABLE: JMAN10 0x1020136c
// SIZE 0x44
class InvWnd : public CWnd {
public:
	InvWnd(CWnd *, int, int, Inventory *);
	~InvWnd();

	HBITMAP m_bitmap;       // 0x1c
	Inventory *m_inventory; // 0x1e
	int *m_unk0x22;         // 0x22
	CFont m_font;           // 0x26
	int m_unk0x2c;          // 0x2c
	int m_unk0x2e;          // 0x2e
	int m_unk0x30;          // 0x30
	int m_unk0x32;          // 0x32
	int m_unk0x34;          // 0x34
	int m_unk0x36;          // 0x36
	int m_unk0x38;          // 0x38
	int *m_unk0x3a;         // 0x3a
	int m_unk0x3e;          // 0x3e
	int m_unk0x40;          // 0x40
	UINT m_timer;           // 0x42

protected:
	DECLARE_MESSAGE_MAP()
};

#endif
