#include "jmanwin.h"

#include <afxwin.h>
#include <mmsystem.h>

#include "gamewin.h"
#include "menu.h"
#include "norad.h"
#include "titles.h"
#include "util.h"

BEGIN_MESSAGE_MAP(JmanWindow, CFrameWnd)
ON_WM_KEYDOWN()
ON_WM_KEYUP()
END_MESSAGE_MAP()

// FUNCTION: JMAN10 0x10080000
JmanWindow::JmanWindow() {
	m_gameShouldExit = FALSE;
	m_curWindow = NULL;

	// full screen
	RECT rect;
	SetRect(&rect, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

	Create(NULL, "The Journeyman Project Turbo", 0x80000000,
		   rect, NULL, 0, NULL);

	SetFocus();
	SetupPalette(this);
	SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, 0, NULL, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
}

// FUNCTION: JMAN10 0x100800aa
JmanWindow::~JmanWindow() {
	if (m_curWindow != NULL) {
		m_curWindow->DestroyWindow();
		delete m_curWindow;
	}

	m_curWindow = NULL;
	sndPlaySound(NULL, SND_NODEFAULT);
}

// FUNCTION: JMAN10 0x10080124
BOOL JmanWindow::ShowMenu() {
	if (m_curWindow != NULL) {
		m_curWindow->DestroyWindow();
		delete m_curWindow;
	}

	m_curWindow = NULL;

	m_curWindow = new Menu(this);
	m_curWindow->ShowWindow(SW_SHOW);
	m_curWindow->InvalidateRect(NULL, TRUE);
	m_curWindow->UpdateWindow();

	SetFocus();

	return TRUE;
}

// FUNCTION: JMAN10 0x100801fe
BOOL JmanWindow::ShowTitles() {
	RECT rect;
	GetClientRect(&rect);

	m_curWindow = new Titles(this);
	m_curWindow->ShowWindow(SW_SHOW);
	m_curWindow->InvalidateRect(NULL, TRUE);
	m_curWindow->UpdateWindow();
	((Titles *)m_curWindow)->Logos();

	m_curWindow->DestroyWindow();
	delete m_curWindow;
	m_curWindow = NULL;

	SetFocus();
	ShowMenu();

	return TRUE;
}

// STUB: JMAN10 0x100802f8
BOOL JmanWindow::ShowSepia() {
	return TRUE;
}

// STUB: JMAN10 0x10080414
BOOL JmanWindow::ShowDream() {
	return TRUE;
}

// FUNCTION: JMAN10 0x100805ba
BOOL JmanWindow::NewGame() {
	if (m_curWindow != NULL) {
		m_curWindow->DestroyWindow();
		delete m_curWindow;
	}
	m_curWindow = NULL;

	RECT rect;
	GetClientRect(&rect);

	m_curWindow = new GameWindow(this, (rect.right - 640) / 2, (rect.bottom - 480) / 2);

	m_curWindow->ShowWindow(SW_SHOW);
	m_curWindow->Invalidate(TRUE);
	m_curWindow->UpdateWindow();

	((GameWindow *)m_curWindow)->Setup();
	// TODO: macros or enum.
	// Area 1: (Caldoria 4th floor), Position 19, EAST.
	((GameWindow *)m_curWindow)->m_viewScreen->NewArea(1, 19, 3);

	return TRUE;
}

// FUNCTION: JMAN10 0x10080ae0
void JmanWindow::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	switch (nChar) {
	case VK_ESCAPE:
		m_gameShouldExit = TRUE;
		break;
	default:
		if (m_curWindow == NULL) {
			break;
		}

		if (!m_curWindow->IsWindowEnabled()) {
			break;
		}

		m_curWindow->SendMessage(WM_KEYDOWN, nChar, MAKELONG(nRepCnt, nFlags));
		break;
	}
}

// FUNCTION: JMAN10 0x10080b3e
void JmanWindow::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
	switch (nChar) {
	case VK_ESCAPE:
		if (m_gameShouldExit) {
			m_gameShouldExit = FALSE;

			if (m_curWindow != NULL) {
				m_curWindow->DestroyWindow();
				delete m_curWindow;
			}
			m_curWindow = NULL;

			HDC_FUN_1008_424c(this);

			SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, 1, NULL, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
			SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
		}
		break;
	default:
		if (m_curWindow == NULL) {
			break;
		}

		if (!m_curWindow->IsWindowEnabled()) {
			break;
		}

		m_curWindow->SendMessage(WM_KEYUP, nChar, MAKELONG(nRepCnt, nFlags));
		break;
	}
}
