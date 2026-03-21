#ifndef CHIPBANK_H
#define CHIPBANK_H

#include <afxwin.h>

#include "invdata.h"

// VTABLE: JMAN10 0x10204e0c
// SIZE 0x28
class ChipBank : public CWnd {
public:
	ChipBank(CWnd *, int, int, Inventory *);
	virtual ~ChipBank();

	HWND m_video;           // 0x1c
	int m_unk0x1e;          // 0x1e
	int m_unk0x20;          // 0x20
	Inventory *m_inventory; // 0x22
	int m_unk0x26;          // 0x26

protected:
	DECLARE_MESSAGE_MAP()
};

#endif
