#ifndef BIOCHIP_H
#define BIOCHIP_H

#include <afxwin.h>

// VTABLE: JMAN10 0x102041e0
// SIZE 0x44
class BioChip : public CWnd {
public:
	BioChip(CWnd *, int, int);
	virtual ~BioChip();

	void FUN_1020_16c0(void *);

	HWND m_popup;   // 0x1c
	int m_unk0x1e;  // 0x1e
	int m_unk0x20;  // 0x20
	int m_unk0x22;  // 0x22
	int m_unk0x24;  // 0x24
	int m_unk0x26;  // 0x26
	int m_unk0x28;  // 0x28
	int *m_unk0x2a; // 0x2a
	CFont m_font;   // 0x2e
	int m_unk0x34;  // 0x34
	int m_unk0x36;  // 0x36
	int m_unk0x38;  // 0x38
	int m_unk0x3a;  // 0x3a
	int m_unk0x3c;  // 0x3c
	int m_unk0x3e;  // 0x3e
	int m_unk0x40;  // 0x40
	int m_unk0x42;  // 0x42

protected:
	DECLARE_MESSAGE_MAP()
};

#endif
