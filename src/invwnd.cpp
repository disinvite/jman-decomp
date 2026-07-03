#include "invwnd.h"

#include <afxwin.h>

#include "util.h"

BEGIN_MESSAGE_MAP(InvWnd, CWnd)
END_MESSAGE_MAP()

// FUNCTION: JMAN10 0x1018ed5e
InvWnd::InvWnd(CWnd *parent, int left, int top, Inventory *p_inventory) {
	m_bitmap = 0;
	m_unk0x2c = 0;
	m_unk0x22 = NULL;
	m_unk0x3a = NULL;
	m_unk0x2e = 0;
	m_unk0x30 = 0;
	m_unk0x32 = -1;
	m_unk0x38 = -1;
	m_unk0x3e = 0;
	m_unk0x40 = 0;

	RECT rect = {left, top, left + 227, top + 70};
	Create(NULL, NULL, WS_CHILD, rect, parent, 0, NULL);

	m_timer = 0;
	m_timer = SetTimer(1, 500, NULL);

	HDIB dib = OpenDIB(GetPathToSupportFile("SUPPORT\\INVWND.BMP"));
	m_bitmap = BitmapFromDib(dib, GetCurrentPal(), 0);
	GlobalUnlock(dib);
	GlobalFree(dib);

	m_unk0x2c = 0;
	m_inventory = p_inventory;

	m_font.CreateFont(14, 0, 0, 0, 400, 0, 0, 0, 0, 4, 1, 2, 1, "Arial");
}

InvWnd::~InvWnd() {
}

// STUB: JMAN10 0x1020063a
void InvWnd::FUN_1020_063a() {
	// DECOMP: Is this module split across multiple sections?
}
