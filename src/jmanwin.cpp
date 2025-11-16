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
	_game_should_exit = FALSE;
	_curwindow = NULL;

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
	if (_curwindow != NULL) {
		_curwindow->DestroyWindow();
		delete _curwindow;
	}

	_curwindow = NULL;
	sndPlaySound(NULL, SND_NODEFAULT);
}

// FUNCTION: JMAN10 0x10080124
BOOL JmanWindow::ShowMenu() {
	if (_curwindow != NULL) {
		_curwindow->DestroyWindow();
		delete _curwindow;
	}

	_curwindow = NULL;

	_curwindow = new Menu(this);
	_curwindow->ShowWindow(SW_SHOW);
	_curwindow->InvalidateRect(NULL, TRUE);
	_curwindow->UpdateWindow();

	SetFocus();

	return TRUE;
}

// FUNCTION: JMAN10 0x100801fe
BOOL JmanWindow::ShowTitles() {
	RECT rect;
	GetClientRect(&rect);

	_curwindow = new Titles(this);
	_curwindow->ShowWindow(SW_SHOW);
	_curwindow->InvalidateRect(NULL, TRUE);
	_curwindow->UpdateWindow();
	((Titles *)_curwindow)->Logos();

	_curwindow->DestroyWindow();
	delete _curwindow;
	_curwindow = NULL;

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
	if (_curwindow != NULL) {
		_curwindow->DestroyWindow();
		delete _curwindow;
	}
	_curwindow = NULL;

	RECT rect;
	GetClientRect(&rect);

	_curwindow = new GameWindow(this, (rect.right - 640) / 2, (rect.bottom - 480) / 2);

	_curwindow->ShowWindow(SW_SHOW);
	_curwindow->Invalidate(TRUE);
	_curwindow->UpdateWindow();

	((GameWindow *)_curwindow)->Setup(this);

	return TRUE;
}

// FUNCTION: JMAN10 0x10080ae0
void JmanWindow::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	switch (nChar) {
	case VK_ESCAPE:
		_game_should_exit = TRUE;
		break;
	default:
		if (_curwindow == NULL) {
			break;
		}

		if (!_curwindow->IsWindowEnabled()) {
			break;
		}

		_curwindow->SendMessage(WM_KEYDOWN, nChar, MAKELONG(nRepCnt, nFlags));
		break;
	}
}

// FUNCTION: JMAN10 0x10080b3e
void JmanWindow::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
	switch (nChar) {
	case VK_ESCAPE:
		if (_game_should_exit) {
			_game_should_exit = FALSE;

			if (_curwindow != NULL) {
				_curwindow->DestroyWindow();
				delete _curwindow;
			}
			_curwindow = NULL;

			HDC_FUN_1008_424c(this);

			SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, 1, NULL, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
			SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
		}
		break;
	default:
		if (_curwindow == NULL) {
			break;
		}

		if (!_curwindow->IsWindowEnabled()) {
			break;
		}

		_curwindow->SendMessage(WM_KEYUP, nChar, MAKELONG(nRepCnt, nFlags));
		break;
	}
}
