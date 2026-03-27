#include "tutor.h"

#include <afxwin.h>
#include <vfw.h>

#include "sprite.h"
#include "util.h"

// FUNCTION: JMAN10 0x10300902
InterfaceOverview::InterfaceOverview(CWnd *parent) : m_spriteMgr(NULL) {
	TCHAR szMode[256];

	// Message window
	SetRect(&m_hotspots[0], 8, 95, 126, 263);

	// Inventory
	SetRect(&m_hotspots[1], 140, 315, 366, 389);

	// Biochip
	SetRect(&m_hotspots[2], 424, 336, 628, 440);

	// Chipbank
	SetRect(&m_hotspots[3], 125, 443, 387, 480);

	// Compass
	SetRect(&m_hotspots[4], 292, 31, 444, 76);

	// Date
	SetRect(&m_hotspots[5], 471, 61, 574, 77);

	// Energy
	SetRect(&m_hotspots[6], 145, 61, 264, 76);

	// E-Light
	SetRect(&m_hotspots[7], 578, 56, 619, 74);

	// Btnbar
	SetRect(&m_hotspots[8], 425, 446, 616, 480);

	// Viewscreen (take item)
	SetRect(&m_hotspots[9], 323, 160, 417, 218);

	// Pegasus button
	SetRect(&m_hotspots[10], 374, 328, 414, 368);

	// Exit button
	SetRect(&m_hotspots[11], 70, 20, 259, 34);

	RECT rect;
	GetClientRect(&rect);
	SetRect(&rect, (rect.right - 640) / 2, (rect.bottom - 480) / 2, rect.right + 640, rect.bottom + 480);
	Create(NULL, NULL, WS_CHILD, rect, parent, 0, NULL);

	m_spriteMgr.FUN_1028_b324(GetPathToSupportFile("SUPPORT\\TUTORIAL\\TUTOR.AVI"));

	MSG msg;

	while (PeekMessage(&msg, afxCurrentWinApp->m_pMainWnd->m_hWnd,
					   WM_LBUTTONDOWN, WM_LBUTTONUP, PM_REMOVE))
		;

	while (PeekMessage(&msg, afxCurrentWinApp->m_pMainWnd->m_hWnd,
					   WM_KEYFIRST, WM_KEYLAST, PM_REMOVE))
		;

	ShowWindow(SW_SHOW);
	m_unk0x1c = -1;
	Invalidate(FALSE);
	UpdateWindow();

	// "This is the Journeyman online help file. Roll over each part of the interface to learn more about it."
	sndPlaySound(GetPathToSupportFile("SUPPORT\\TUTORIAL\\TUTOR1.WAV"), SND_NODEFAULT | SND_ASYNC);
}
