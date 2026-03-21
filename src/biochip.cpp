#include "biochip.h"

#include <afxwin.h>
#include <vfw.h>

#include "util.h"

BEGIN_MESSAGE_MAP(BioChip, CWnd)
END_MESSAGE_MAP()

#define WaitForMCIStopped(_handle, _buf)  \
	do {                                  \
		Yield();                          \
		MCIWndGetMode(_handle, _buf, 64); \
	} while (strcmp("stopped", _buf))

// FUNCTION: JMAN10 0x102013d8
BioChip::BioChip(CWnd *parent, int left, int top) {
	TCHAR szMode[256];

	m_popup = 0;
	m_unk0x1e = 0;
	m_unk0x20 = 0;
	m_unk0x22 = 0;
	m_unk0x26 = 0;
	m_unk0x28 = 0;
	m_unk0x34 = 0;
	m_unk0x36 = 0;
	m_unk0x38 = -1;
	m_unk0x40 = 0;
	m_unk0x42 = 0;

	RECT rect = {left, top, left + 224, top + 110};
	Create(NULL, NULL, WS_CHILD, rect, parent, 0, NULL);
	ShowWindow(SW_SHOW);

	m_popup = MCIWndCreate(m_hWnd, afxCurrentInstanceHandle,
						   MAKELONG(MCIWNDF_NOPLAYBAR, MCIWNDF_NOERRORDLG),
						   GetPathToSupportFile("SUPPORT\\POPUP.AVI"));

	if (m_popup != 0) {
		MCIWndRealize(m_popup, 0);
		MCIWndSeek(m_popup, 0);
		WaitForMCIStopped(m_popup, szMode);
		FUN_1020_16c0(NULL);
	}

	m_unk0x22 = 0;
	m_unk0x2a = NULL;

	m_font.CreateFont(11, 0, 0, 0, 700, 0, 0, 0, 0, 4, 1, 2, 1, "Arial");
}

// STUB: JMAN10 0x1020159e
BioChip::~BioChip() {
}

// STUB: JMAN10 0x102016c0
void BioChip::FUN_1020_16c0(void *) {
}
