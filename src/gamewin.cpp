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
		MCIWndGetMode(m_eLight, _buf, 64); \
	} while (strcmp("stopped", _buf))

// FUNCTION: JMAN10 0x10081bf8
GameWindow::GameWindow(CFrameWnd *cf, int left, int top) {
	m_msgWnd = NULL;
	m_viewScreen = NULL;
	m_btnBar = NULL;
	m_compass = NULL;
	m_drawLevel = 2;

	HDC_FUN_1008_41a0(this, GetPathToSupportFile("SUPPORT\\FULLSCRN.BMP"));

	m_bitmapFullscreen = 0;
	HDIB fullscrn = OpenDIB(GetPathToSupportFile("SUPPORT\\FULLSCRN.BMP"));
	m_bitmapFullscreen = BitmapFromDib(fullscrn, GetCurrentPal(), 0);
	GlobalUnlock(fullscrn);
	GlobalFree(fullscrn);

	RECT rect = {left, top, left + 640, top + 480};
	Create(NULL, NULL, 0x40000000, rect, cf, 0, NULL);

	m_eLight = MCIWndCreate(m_hWnd, afxCurrentInstanceHandle,
							MAKELONG(MCIWNDF_NOPLAYBAR, MCIWNDF_NOERRORDLG | MCIWNDF_NOTIFYMEDIA),
							GetPathToSupportFile("SUPPORT\\ELIGHT.AVI"));

	if (m_eLight != 0) {
		::SetWindowPos(m_eLight, NULL, 575, 56, 0, 0,
					   SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_NOSIZE);
		MCIWndRealize(m_eLight, 0);
		MCIWndSeek(m_eLight, 0);

		TCHAR szMode[64];
		WaitForMCIStopped(szMode);
	}

	m_font.CreateFont(11, 6, 0, 0, 700, 0, 0, 0, 0, 4, 1, 2, 1, "Arial");

	m_energyLevel = 0;
}

// FUNCTION: JMAN10 0x10081e62
GameWindow::~GameWindow() {
}

// FUNCTION: JMAN10 0x1008211e
BOOL GameWindow::Setup() {
	m_navMap.Setup(m_navData.GetScenesHandle(), m_navData.GetScenesCount());
	m_inventory.FUN_1028_40c0();

	m_drawLevel = 2;
	m_energyLevel = 1000000;
	m_unk0x7a = 0;
	m_unk0x7e = 0;
	m_unk0x82 = 0;
	m_unk0x84 = 0;
	m_energyAlertLevel = 0;

	ShowWindow(SW_SHOW);
	if (m_eLight)
		::ShowWindow(m_eLight, SW_SHOW);

	m_msgWnd = new MsgWnd(this, 0, 96);
	m_msgWnd->EnableWindow(FALSE);
	m_msgWnd->ShowWindow(SW_SHOW);

	m_compass = new Compass(this, 287, 35);
	m_compass->ShowWindow(SW_SHOW);

	m_viewScreen = new Viewscreen(this, 128, 90, &m_navData, &m_navMap, &m_inventory, 1, 19, 3);
	m_viewScreen->EnableWindow(FALSE);
	m_viewScreen->ShowWindow(SW_SHOW);

	m_btnBar = new BtnBar(this, 439, 448);
	m_btnBar->ShowWindow(SW_SHOW);

	m_invWnd = new InvWnd(this, 140, 319, &m_inventory);
	m_invWnd->ShowWindow(SW_SHOW);

	m_bioChip = new BioChip(this, 408, 337);
	m_bioChip->ShowWindow(SW_SHOW);

	m_chipBank = new ChipBank(this, 127, 432, &m_inventory);
	m_chipBank->ShowWindow(SW_SHOW);

	return TRUE;
}

// FUNCTION: JMAN10 0x1008323e
BOOL GameWindow::AddToEnergy(long amt) {
	m_energyLevel += amt;
	return TRUE;
}

// FUNCTION: JMAN10 0x1008326a
DWORD GameWindow::GetEnergyLevel() {
	return m_energyLevel;
}

// STUB: JMAN10 0x1008327c
BOOL GameWindow::CheckEnergyLevel() {
	// TODO: Calls WaitForMciStopped.

	if (m_energyAlertLevel <= 2 && m_energyLevel < 100000) {
		// energy level critical!
	} else if (m_energyAlertLevel <= 1 && m_energyLevel < 250000) {
		// energy level 25%
	} else if (m_energyAlertLevel <= 0 && m_energyLevel < 500000) {
		// energy level 50%
	} else if (m_energyLevel <= 0) {
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

	if (m_inventory.PlayerHasItem(INV_BIOCHIP_PEG)) {
		_biochip_score += 2500;
	}
	if (m_inventory.PlayerHasItem(INV_BIOCHIP_MAP)) {
		_biochip_score += 2500;
	}
	if (m_inventory.PlayerHasItem(INV_BIOCHIP_TRA)) {
		_biochip_score += 2500;
	}
	if (m_inventory.PlayerHasItem(INV_BIOCHIP_OPT)) {
		_biochip_score += 2500;
	}
	if (m_inventory.PlayerHasItem(INV_BIOCHIP_RET)) {
		_biochip_score += 2500;
	}
	if (m_inventory.PlayerHasItem(INV_BIOCHIP_SHD)) {
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

	if (m_drawLevel >= 1) {
		HDC_FUN_1008_44b0(cdc->m_hDC, m_bitmapFullscreen, 0, 0);

		CFont *t_font = cdc->SelectObject(&m_font);
		COLORREF color = cdc->SetTextColor(COLOR_ENERGY_LEVEL);
		int bk_mode = cdc->SetBkMode(TRANSPARENT);

		RECT energy_rect;
		SetRect(&energy_rect, 195, 64, 262, 139);

		wsprintf(energy_buf, "%ld", m_energyLevel);
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
	if (m_drawLevel != 2) {
		return;
	}

	CWnd *childWindow = ChildWindowFromPoint(point);
	if (childWindow != this && childWindow != NULL) {
		CPoint cPt = point;
		MapWindowPoints(childWindow, &cPt, 1);
		childWindow->SendMessage(WM_LBUTTONDOWN, nFlags, MAKELONG(cPt.x, cPt.y));
	} else {
		RECT rect;
		// If the player clicked the pegasus recall button:
		SetRect(&rect, 376, 330, 413, 366);
		if (PtInRect(&rect, point)) {
			// If not in Caldoria or TSA:
			// (i.e. if we are back in time and can recall)
			if (m_viewScreen && m_viewScreen->GetCurrentSceneArea() > 4) {
				// TODO: probably enum
				// Open pegasus biochip window?
				m_bioChip->FUN_1020_1864(2);
			}
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
	if (m_drawLevel == 2 && m_viewScreen) {
		m_viewScreen->SendMessage(WM_KEYDOWN, nChar, MAKELONG(nRepCnt, nFlags));
	}
}

// FUNCTION: JMAN10 0x10083d22
void GameWindow::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (m_drawLevel == 2 && m_viewScreen) {
		m_viewScreen->SendMessage(WM_KEYUP, nChar, MAKELONG(nRepCnt, nFlags));
	}
}

#undef WaitForMCIStopped
