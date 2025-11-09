#include "gamewin.h"

#include <afxwin.h>
#include <vfw.h>

#include "define.h"
#include "invdata.h"
#include "jmanwin.h"
#include "util.h"

BEGIN_MESSAGE_MAP(GameWindow, CWnd)
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_KEYDOWN()
ON_WM_KEYUP()
END_MESSAGE_MAP()

#define WaitForMCIStopped(_buf)            \
	do {                                   \
		Yield();                           \
		MCIWndGetMode(hElight_, _buf, 64); \
	} while (strcmp("stopped", _buf))

// FUNCTION: JMAN10 0x10081bf8
GameWindow::GameWindow(CFrameWnd *cf, int left, int top) {
	msgwnd_ = NULL;
	viewscreen_ = NULL;
	btnbar_ = NULL;
	compass_ = NULL;
	draw_level_ = 2;

	HDC_FUN_1008_41a0(this, GetPathToSupportFile("SUPPORT\\FULLSCRN.BMP"));

	_bmp_fullscrn = 0;
	HDIB fullscrn = OpenDIB(GetPathToSupportFile("SUPPORT\\FULLSCRN.BMP"));
	_bmp_fullscrn = BitmapFromDib(fullscrn, GetCurrentPal(), 0);
	GlobalUnlock(fullscrn);
	GlobalFree(fullscrn);

	RECT rect = {left, top, left + 640, top + 480};
	Create(NULL, NULL, 0x40000000, rect, cf, 0, NULL);

	hElight_ = MCIWndCreate(m_hWnd, afxCurrentInstanceHandle,
							MAKELONG(MCIWNDF_NOPLAYBAR, MCIWNDF_NOERRORDLG | MCIWNDF_NOTIFYMEDIA),
							GetPathToSupportFile("SUPPORT\\ELIGHT.AVI"));

	if (hElight_ != 0) {
		::SetWindowPos(hElight_, NULL, 575, 56, 0, 0,
					   SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_NOSIZE);
		MCIWndRealize(hElight_, 0);
		MCIWndSeek(hElight_, 0);

		TCHAR szMode[64];
		WaitForMCIStopped(szMode);
	}

	_font.CreateFont(11, 6, 0, 0, 700, 0, 0, 0, 0, 4, 1, 2, 1, "Arial");

	_energy_level = 0;
}

// FUNCTION: JMAN10 0x10081e62
GameWindow::~GameWindow() {
}

BOOL GameWindow::Setup(CFrameWnd *cf) {
	navmap_.Setup(navdata_.GetScenesHandle(), navdata_.GetScenesCount());

	_energy_level = 1000000;

	_energy_alert_level = 0;

	ShowWindow(SW_SHOW);
	if (hElight_)
		::ShowWindow(hElight_, SW_SHOW);

	msgwnd_ = new MsgWnd(this, 0, 96);
	msgwnd_->ShowWindow(SW_SHOW);

	compass_ = new Compass(this, 287, 35);
	compass_->ShowWindow(SW_SHOW);

	// Viewscreen
	// Btnbar
	// InvWnd
	// Biochip
	// Chipbank

	return TRUE;
}

// FUNCTION: JMAN10 0x1008323e
BOOL GameWindow::AddToEnergy(long amt) {
	_energy_level += amt;
	return TRUE;
}

// FUNCTION: JMAN10 0x1008326a
DWORD GameWindow::GetEnergyLevel() {
	return _energy_level;
}

// STUB: JMAN10 0x1008327c
BOOL GameWindow::CheckEnergyLevel() {
	// TODO: Calls WaitForMciStopped.

	if (_energy_alert_level <= 2 && _energy_level < 100000) {
		// energy level critical!
	} else if (_energy_alert_level <= 1 && _energy_level < 250000) {
		// energy level 25%
	} else if (_energy_alert_level <= 0 && _energy_level < 500000) {
		// energy level 50%
	} else if (_energy_level <= 0) {
		// dead
		JmanWindow *jw = (JmanWindow *)GetParent();
		return FALSE;
	}

	return TRUE;
}

// STUB: JMAN10 0x10083534
BOOL GameWindow::CalculateScore() {
	// biochip score is +2c of struct.
	DWORD _biochip_score = 0; // TODO: score struct.

	if (_inventory.PlayerHasItem(INV_BIOCHIP_PEG)) {
		_biochip_score += 2500;
	}
	if (_inventory.PlayerHasItem(INV_BIOCHIP_MAP)) {
		_biochip_score += 2500;
	}
	if (_inventory.PlayerHasItem(INV_BIOCHIP_TRA)) {
		_biochip_score += 2500;
	}
	if (_inventory.PlayerHasItem(INV_BIOCHIP_OPT)) {
		_biochip_score += 2500;
	}
	if (_inventory.PlayerHasItem(INV_BIOCHIP_RET)) {
		_biochip_score += 2500;
	}
	if (_inventory.PlayerHasItem(INV_BIOCHIP_SHD)) {
		_biochip_score += 2500;
	}

	return TRUE;
}

// Seems like this should be RGB(0, 102, 102), but there's a 2 in the MSB.
#define COLOR_ENERGY_LEVEL 0x02666600

// FUNCTION: JMAN10 0x100839ae
void GameWindow::OnPaint() {
	PAINTSTRUCT paint;
	char energy_buf[256];

	CDC *cdc = BeginPaint(&paint);
	HPALETTE pal = ::SelectPalette(cdc->m_hDC, GetCurrentPal(), FALSE);

	if (draw_level_ >= 1) {
		HDC_FUN_1008_44b0(cdc->m_hDC, _bmp_fullscrn, 0, 0);

		CFont *t_font = cdc->SelectObject(&_font);
		COLORREF color = cdc->SetTextColor(COLOR_ENERGY_LEVEL);
		int bk_mode = cdc->SetBkMode(TRANSPARENT);

		RECT energy_rect;
		SetRect(&energy_rect, 195, 64, 262, 139);

		wsprintf(energy_buf, "%ld", _energy_level);
		cdc->DrawText(energy_buf, -1, &energy_rect, DT_NOCLIP | DT_RIGHT);

		cdc->SetBkMode(bk_mode);
		cdc->SetTextColor(color);
		cdc->SelectObject(t_font);
	}

	cdc->RealizePalette();
	::SelectPalette(cdc->m_hDC, pal, FALSE);

	EndPaint(&paint);
}

#undef COLOR_ENERGY_LEVEL

// FUNCTION: JMAN10 0x10083b06
void GameWindow::OnLButtonDown(UINT nFlags, CPoint point) {
	if (draw_level_ != 2)
		return;

	CWnd *childWindow = ChildWindowFromPoint(point);
	if (childWindow != this && childWindow != NULL) {
		CPoint cPt = point;
		MapWindowPoints(childWindow, &cPt, 1);
		childWindow->SendMessage(WM_LBUTTONDOWN, nFlags, MAKELONG(cPt.x, cPt.y));
	} else {
		// TODO
		RECT rect;
		SetRect(&rect, 376, 330, 413, 366);
		if (PtInRect(&rect, point)) {
			// viewscreen function
			// if (fun_1008_76a6 > 4) {
			// biochip function?
			// fun_1020_1864
			// }
		}
	}
}

// STUB: JMAN10 0x10083be6
void GameWindow::OnLButtonUp(UINT nFlags, CPoint point) {
}

// STUB: JMAN10 0x10083c68
void GameWindow::OnMouseMove(UINT nFlags, CPoint point) {
}

// FUNCTION: JMAN10 0x10083cea
void GameWindow::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (draw_level_ == 2 && viewscreen_) {
		viewscreen_->SendMessage(WM_KEYDOWN, nChar, MAKELONG(nRepCnt, nFlags));
	}
}

// FUNCTION: JMAN10 0x10083d22
void GameWindow::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (draw_level_ == 2 && viewscreen_) {
		viewscreen_->SendMessage(WM_KEYUP, nChar, MAKELONG(nRepCnt, nFlags));
	}
}

#undef WaitForMCIStopped
