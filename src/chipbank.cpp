#include "chipbank.h"

#include <afxwin.h>
#include <vfw.h>

#include "util.h"

BEGIN_MESSAGE_MAP(ChipBank, CWnd)
END_MESSAGE_MAP()

#define WaitForMCIStopped(_handle, _buf)  \
	do {                                  \
		Yield();                          \
		MCIWndGetMode(_handle, _buf, 64); \
	} while (strcmp("stopped", _buf))

// FUNCTION: JMAN10 0x1020424c
ChipBank::ChipBank(CWnd *parent, int left, int top, Inventory *p_inventory) {
	TCHAR szMode[256];

	m_video = 0;
	m_unk0x1e = 0;
	m_unk0x20 = 0;
	m_inventory = p_inventory;
	m_unk0x26 = -1;

	RECT rect = {left, top, left + 256, top + 48};
	Create(NULL, NULL, WS_CHILD, rect, parent, 0, NULL);
	ShowWindow(SW_SHOW);

	m_video = MCIWndCreate(m_hWnd, afxCurrentInstanceHandle,
						   MAKELONG(MCIWNDF_NOPLAYBAR, MCIWNDF_NOERRORDLG),
						   GetPathToSupportFile("SUPPORT\\CHIPBANK.AVI"));

	if (m_video != 0) {
		MCIWndRealize(m_video, 0);
		MCIWndSeek(m_video, 0);
		WaitForMCIStopped(m_video, szMode);
	}
}

// STUB: JMAN10 0x10204384
ChipBank::~ChipBank() {
}
