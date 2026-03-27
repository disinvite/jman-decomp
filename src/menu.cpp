#include "menu.h"

#include <afxdlgs.h>
#include <afxwin.h>

#include "jmanwin.h"
#include "tutor.h"
#include "util.h"

IMPLEMENT_DYNAMIC(Menu, CWnd)

BEGIN_MESSAGE_MAP(Menu, CWnd)
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

static char _far kMenuInterface[] = "Show me how to use Journeyman's interface.";
static char _far kMenuNewGame[] = "Start the game from the very beginning.";
static char _far kMenuSkipBkgd[] = "Start the game at the introductory animation.";
static char _far kMenuSkipIntro[] = "Start from after the background and introductory animations.";
static char _far kMenuCredits[] = "Run the credits.";
static char _far kMenuRestore[] = "Restart a previously saved game.";
static char _far kMenuQuit[] = "Quit, so I can optimize my system for best performance.";

// FUNCTION: JMAN10 0x1028c044
Menu::Menu(CFrameWnd *cf) {
	m_selectedOpt = -1;
	m_rolloverBuf[0] = '\0';

	HDC_FUN_1008_41a0(this, GetPathToSupportFile("SUPPORT\\FULLSCRN.BMP"));
	m_menu2 = 0;
	HDIB hBkgd = OpenDIB(GetPathToSupportFile("SUPPORT\\MAINMNU1.BMP"));
	m_menu2 = BitmapFromDib(hBkgd, GetCurrentPal(), 0);

	GlobalUnlock(hBkgd);
	GlobalFree(hBkgd);

	m_menu0 = 0;
	hBkgd = OpenDIB(GetPathToSupportFile("SUPPORT\\MAINMNU1.BMP"));
	m_menu0 = BitmapFromDib(hBkgd, GetCurrentPal(), 0);

	GlobalUnlock(hBkgd);
	GlobalFree(hBkgd);

	m_menu1 = 0;
	hBkgd = OpenDIB(GetPathToSupportFile("SUPPORT\\MAINMNU2.BMP"));
	m_menu1 = BitmapFromDib(hBkgd, GetCurrentPal(), 0);

	GlobalUnlock(hBkgd);
	GlobalFree(hBkgd);

	// interface overview
	SetRect(&m_rect0, 406, 65, 614, 111);

	// new game
	SetRect(&m_rect1, 406, 128, 614, 174);

	// skip background, skip intro
	SetRect(&m_rect2, 406, 182, 506, 228);
	SetRect(&m_rect3, 514, 182, 614, 228);

	// credits
	SetRect(&m_rect4, 406, 245, 614, 291);

	// restore previous, quit so i can prepare
	SetRect(&m_rect5, 406, 299, 506, 345);
	SetRect(&m_rect6, 514, 299, 614, 345);

	RECT rect;
	RECT center;
	::GetClientRect(cf->m_hWnd, &rect);

	int left = (rect.right - 638) / 2;
	int top = (rect.bottom - 478) / 2;
	SetRect(&center, left, top, left + 638, top + 478);

	Create(NULL, NULL, 0x40000000, center, cf, 0, NULL);

	// wait for keypress or click to display menu

	MSG msg;

	while (PeekMessage(&msg, afxCurrentWinApp->m_pMainWnd->m_hWnd,
					   WM_LBUTTONDOWN, WM_LBUTTONUP, PM_REMOVE))
		;

	while (PeekMessage(&msg, afxCurrentWinApp->m_pMainWnd->m_hWnd,
					   WM_KEYFIRST, WM_KEYLAST, PM_REMOVE))
		;
}

// FUNCTION: JMAN10 0x1028c2ce
Menu::~Menu() {
	if (m_menu0 != 0) {
		DeleteObject(m_menu0);
	}

	if (m_menu1 != 0) {
		DeleteObject(m_menu1);
	}

	if (m_menu2 != 0) {
		DeleteObject(m_menu2);
	}
}

// FUNCTION: JMAN10 0x1028c344
void Menu::OnPaint() {
	PAINTSTRUCT paint;

	CDC *cdc = BeginPaint(&paint);
	cdc->RealizePalette();

	HDC_FUN_1008_46f6(cdc->m_hDC, m_menu0, 0, 0, 638, 478,
					  m_menu2, 0, 0, FALSE, NULL);

	if (m_menu1 != 0) {
		switch (m_selectedOpt) {
		case 1:
			HDC_FUN_1008_46f6(cdc->m_hDC, m_menu1, 390, 55, 230, 64, m_menu2, 390, 55, FALSE, NULL);
			break;
		case 2:
			HDC_FUN_1008_46f6(cdc->m_hDC, m_menu1, 390, 126, 230, 53, m_menu2, 390, 126, FALSE, NULL);
			break;
		case 3:
			HDC_FUN_1008_46f6(cdc->m_hDC, m_menu1, 390, 180, 116, 54, m_menu2, 390, 180, FALSE, NULL);
			break;
		case 4:
			HDC_FUN_1008_46f6(cdc->m_hDC, m_menu1, 506, 180, 108, 56, m_menu2, 506, 180, FALSE, NULL);
			break;
		case 5:
			HDC_FUN_1008_46f6(cdc->m_hDC, m_menu1, 390, 240, 277, 55, m_menu2, 390, 240, FALSE, NULL);
			break;
		case 6:
			HDC_FUN_1008_46f6(cdc->m_hDC, m_menu1, 390, 298, 115, 54, m_menu2, 390, 298, FALSE, NULL);
			break;
		case 7:
			HDC_FUN_1008_46f6(cdc->m_hDC, m_menu1, 506, 298, 108, 56, m_menu2, 506, 298, FALSE, NULL);
			break;
		}
	}

	if (m_rolloverBuf[0] != '\0') {
		CFont font;
		CDC myCdc;

		RECT text_rect;
		SetRect(&text_rect, 411, 385, 607, 415);

		myCdc.CreateCompatibleDC(cdc);
		// HGDIOBJ old_obj = myCdc.SelectObject(m_menu2);
		HGDIOBJ old_obj = SelectObject(myCdc.m_hDC, m_menu2);
		font.CreateFont(14, 0, 0, 0, 700, 0, 0, 0, 0, 4, 1, 2, 1, "Arial");
		COLORREF old_color = myCdc.SetTextColor(0);
		int old_mode = myCdc.SetBkMode(TRANSPARENT);
		CFont *old_font = myCdc.SelectObject(&font);
		myCdc.DrawText(m_rolloverBuf, -1, &text_rect, DT_NOCLIP | DT_WORDBREAK);

		myCdc.SelectObject(old_font);
		myCdc.SetBkMode(old_mode);
		myCdc.SetTextColor(old_color);
		// myCdc.SelectObject(old_obj);
		SelectObject(myCdc.m_hDC, old_obj);
		myCdc.DeleteDC();
		font.DeleteObject();
	}

	HDC_FUN_1008_44b0(cdc->m_hDC, m_menu2, 0, 0);
	EndPaint(&paint);
}

// FUNCTION: JMAN10 0x1028c636
void Menu::OnLButtonDown(UINT nFlags, CPoint point) {
	m_selectedOpt = 0;

	// redundant `== TRUE` used intentionally.
	if (PtInRect(&m_rect0, point) == TRUE) {
		m_selectedOpt = 1;
	}

	if (PtInRect(&m_rect1, point) == TRUE) {
		m_selectedOpt = 2;
	}

	if (PtInRect(&m_rect2, point) == TRUE) {
		m_selectedOpt = 3;
	}

	if (PtInRect(&m_rect3, point) == TRUE) {
		m_selectedOpt = 4;
	}

	if (PtInRect(&m_rect4, point) == TRUE) {
		m_selectedOpt = 5;
	}

	if (PtInRect(&m_rect5, point) == TRUE) {
		m_selectedOpt = 6;
	}

	if (PtInRect(&m_rect6, point) == TRUE) {
		m_selectedOpt = 7;
	}

	Invalidate(FALSE);
	UpdateWindow();
}

// FUNCTION: JMAN10 0x1028c73c
void Menu::OnLButtonUp(UINT nFlags, CPoint point) {
	int t_opt = m_selectedOpt;
	m_selectedOpt = 0;

	Invalidate(FALSE);
	UpdateWindow();

	m_selectedOpt = 0;
	if (PtInRect(&m_rect0, point) == TRUE && t_opt == 1) {
		JmanWindow *wnd = (JmanWindow *)GetParent();
		if (wnd->m_curWindow != NULL) {
			delete wnd->m_curWindow;
		}

		wnd->m_curWindow = new InterfaceOverview(wnd);
	}

	if (PtInRect(&m_rect1, point) == TRUE && t_opt == 2) {
		((JmanWindow *)GetParent())->ShowSepia();
		return;
	}

	if (PtInRect(&m_rect2, point) == TRUE && t_opt == 3) {
		((JmanWindow *)GetParent())->ShowDream();
		return;
	}

	if (PtInRect(&m_rect3, point) == TRUE && t_opt == 4) {
		((JmanWindow *)GetParent())->NewGame();
		return;
	}

	if (PtInRect(&m_rect4, point) == TRUE && t_opt == 5) {
		// TODO: this is here just to get afxdlg code into the build.
		CFileDialog fileDialog(
			TRUE,
			"Saved Games (*.jsg) | *.jsg | All Files (*.*) | *.* ||",
			"agent5.jsg",
			0,
			"jsg",
			GetParent());
	}

	if (PtInRect(&m_rect5, point) == TRUE && t_opt == 6) {
	}

	if (PtInRect(&m_rect6, point) == TRUE && t_opt == 7) {
		// emulate escape keypress to exit game
		GetParent()->SendMessage(WM_KEYDOWN, VK_ESCAPE, 0);
		GetParent()->SendMessage(WM_KEYUP, VK_ESCAPE, 0);
	}
}

// FUNCTION: JMAN10 0x1028cb14
void Menu::OnMouseMove(UINT nFlags, CPoint point) {
	if (m_selectedOpt <= 0) {
		m_rolloverBuf[0] = '\0';

		// redundant `== TRUE` used intentionally.
		if (PtInRect(&m_rect0, point) == TRUE) {
			lstrcpy(m_rolloverBuf, kMenuInterface);
		}

		if (PtInRect(&m_rect1, point) == TRUE) {
			lstrcpy(m_rolloverBuf, kMenuNewGame);
		}

		if (PtInRect(&m_rect2, point) == TRUE) {
			lstrcpy(m_rolloverBuf, kMenuSkipBkgd);
		}

		if (PtInRect(&m_rect3, point) == TRUE) {
			lstrcpy(m_rolloverBuf, kMenuSkipIntro);
		}

		if (PtInRect(&m_rect4, point) == TRUE) {
			lstrcpy(m_rolloverBuf, kMenuCredits);
		}

		if (PtInRect(&m_rect5, point) == TRUE) {
			lstrcpy(m_rolloverBuf, kMenuRestore);
		}

		if (PtInRect(&m_rect6, point) == TRUE) {
			lstrcpy(m_rolloverBuf, kMenuQuit);
		}
	} else {
		int t_opt = m_selectedOpt;
		m_selectedOpt = 0;

		if (PtInRect(&m_rect0, point) == TRUE && t_opt == 1) {
			m_selectedOpt = 1;
		}

		if (PtInRect(&m_rect1, point) == TRUE && t_opt == 2) {
			m_selectedOpt = 2;
		}

		if (PtInRect(&m_rect2, point) == TRUE && t_opt == 3) {
			m_selectedOpt = 3;
		}

		if (PtInRect(&m_rect3, point) == TRUE && t_opt == 4) {
			m_selectedOpt = 4;
		}

		if (PtInRect(&m_rect4, point) == TRUE && t_opt == 5) {
			m_selectedOpt = 5;
		}

		if (PtInRect(&m_rect5, point) == TRUE && t_opt == 6) {
			m_selectedOpt = 6;
		}

		if (PtInRect(&m_rect6, point) == TRUE && t_opt == 7) {
			m_selectedOpt = 7;
		}
	}

	Invalidate(FALSE);
	UpdateWindow();
}

// FUNCTION: JMAN10 0x1028cd78
BOOL Menu::OnEraseBkgnd(CDC *pDC) {
	CBrush brush_black((COLORREF)0);
	CBrush *old_brush = pDC->SelectObject(&brush_black);

	RECT rect;
	pDC->GetClipBox(&rect);

	pDC->PatBlt(rect.left, rect.top, rect.right - rect.left,
				rect.bottom - rect.top, PATCOPY);

	pDC->SelectObject(old_brush);
	return TRUE;
}
