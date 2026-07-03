#include "compass.h"

#include <afxwin.h>
#include <vfw.h>

#include "util.h"

BEGIN_MESSAGE_MAP(Compass, CWnd)
ON_WM_PAINT()
END_MESSAGE_MAP()

#define WaitForMCIStopped(_buf)             \
	do {                                    \
		Yield();                            \
		MCIWndGetMode(hCompass_, _buf, 64); \
	} while (strcmp("stopped", _buf))

// FUNCTION: JMAN10 0x1008985a
Compass::Compass(CWnd *parent, int left, int top) {
	TCHAR szMode[256];

	hCompass_ = 0;
	current_dir_ = 0;

	RECT rect = {left, top, left + 160, top + 36};
	Create(NULL, NULL, 0x40000000, rect, parent, 0, NULL);
	ShowWindow(SW_SHOW);

	hCompass_ = MCIWndCreate(m_hWnd, afxCurrentInstanceHandle,
							 MAKELONG(MCIWNDF_NOPLAYBAR, MCIWNDF_NOERRORDLG),
							 GetPathToSupportFile("SUPPORT\\COMPASS.AVI"));

	if (hCompass_ != 0) {
		MCIWndSeek(hCompass_, 0);
		WaitForMCIStopped(szMode);
	}
}

// FUNCTION: JMAN10 0x1008996a
Compass::~Compass() {
	if (hCompass_ != 0)
		MCIWndDestroy(hCompass_);

	hCompass_ = 0;
}

// new frame for compass
// FUNCTION: JMAN10 0x100899b0
BOOL Compass::UpdateHeading(int dir) {
	TCHAR szMode[128];

	if (hCompass_ == 0) {
		return FALSE;
	}

	current_dir_ = dir;
	MCIWndSeek(hCompass_, dir * 10);
	WaitForMCIStopped(szMode);

	Invalidate(FALSE);
	UpdateWindow();

	return TRUE;
}

// oof (walk into wall)
// FUNCTION: JMAN10 0x10089a5c
BOOL Compass::Oof() {
	TCHAR szMode[128];

	if (hCompass_ == 0) {
		return FALSE;
	}

	ShowWindow(SW_SHOW);
	MCIWndPlayTo(hCompass_, current_dir_ * 10 + 9);
	WaitForMCIStopped(szMode);

	ShowWindow(SW_HIDE);
	MCIWndSeek(hCompass_, current_dir_ * 10);
	WaitForMCIStopped(szMode);

	Invalidate(FALSE);
	UpdateWindow();

	return TRUE;
}

// compass realign animation
// 1: N <- E
// 2: S -> W
// 3: E <- S
// 4: N <- W
// 5: N -> E
// 6: E -> S
// FUNCTION: JMAN10 0x10089b90
BOOL Compass::Spin(int which) {
	TCHAR szMode[128];

	if (hCompass_ == 0) {
		return FALSE;
	}

	ShowWindow(SW_SHOW);
	MCIWndSeek(hCompass_, which * 10 + 40);
	MCIWndPlayTo(hCompass_, which * 10 + 49);
	WaitForMCIStopped(szMode);

	ShowWindow(SW_HIDE);
	Invalidate(FALSE);
	UpdateWindow();

	return TRUE;
}

// FUNCTION: JMAN10 0x10089c7a
void Compass::OnPaint() {
	PAINTSTRUCT paint;

	CDC *cdc = BeginPaint(&paint);
	if (hCompass_ != 0) {
		hdc_UpdateHDC(hCompass_, cdc->m_hDC, 0, 0, 0, 0);
	}

	EndPaint(&paint);
}

#undef WaitForMCIStopped
