#include "titles.h"

#include <afxwin.h>
#include <vfw.h>

#include "util.h"

IMPLEMENT_DYNAMIC(Titles, CWnd)

BEGIN_MESSAGE_MAP(Titles, CWnd)
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// FUNCTION: JMAN10 0x1028b74c
Titles::Titles(CFrameWnd *cf) {
	RECT rect;
	MSG msg;
	RECT center;
	::GetClientRect(cf->m_hWnd, &rect);

	int left = (rect.right - 638) / 2;
	int top = (rect.bottom - 478) / 2;
	SetRect(&center, left, top, left + 638, top + 478);

	Create(NULL, NULL, 0x40000000, center, cf, 0, NULL);

	while (PeekMessage(&msg, afxCurrentWinApp->m_pMainWnd->m_hWnd,
					   WM_LBUTTONDOWN, WM_LBUTTONUP, PM_REMOVE))
		;

	while (PeekMessage(&msg, afxCurrentWinApp->m_pMainWnd->m_hWnd,
					   WM_KEYFIRST, WM_KEYLAST, PM_REMOVE))
		;
}

// FUNCTION: JMAN10 0x1028b848
BOOL Titles::Logos() {
	ShowCursor(FALSE);
	InvalidateRect(NULL, TRUE);
	UpdateWindow();

	MSG msg;

	while (PeekMessage(&msg, afxCurrentWinApp->m_pMainWnd->m_hWnd,
					   WM_LBUTTONDOWN, WM_LBUTTONUP, PM_REMOVE))
		;

	while (PeekMessage(&msg, afxCurrentWinApp->m_pMainWnd->m_hWnd,
					   WM_KEYFIRST, WM_KEYLAST, PM_REMOVE))
		;

	RECT rect;
	GetClientRect(&rect);

	HWND videoWindow;
	RECT mci_rect;

	// Sanctuary Woods logo

	videoWindow = MCIWndCreate(m_hWnd, afxCurrentInstanceHandle,
							   MAKELONG(MCIWNDF_NOPLAYBAR, MCIWNDF_NOERRORDLG),
							   GetPathToSupportFile("SUPPORT\\SLOGO.AVI"));

	::GetClientRect(videoWindow, &mci_rect);

	::SetWindowPos(videoWindow, 0,
				   (rect.right - mci_rect.right) / 2,
				   (rect.bottom - mci_rect.bottom) / 2,
				   0, 0,
				   SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_NOSIZE);

	MCIWndUseFrames(videoWindow);
	::ShowWindow(videoWindow, SW_SHOW);
	::SendMessage(videoWindow, MCI_PLAY, 0, NULL);

	TCHAR szMode[64];

	do {
		if (PeekMessage(&msg, afxCurrentWinApp->m_pMainWnd->m_hWnd,
						WM_KEYFIRST, WM_KEYLAST, PM_REMOVE)) {
			::SendMessage(videoWindow, MCI_STOP, 0, NULL);
		}

		MCIWndGetMode(videoWindow, szMode, sizeof(szMode));
	} while (strcmp("stopped", szMode));

	::ShowWindow(videoWindow, SW_HIDE);
	::SendMessage(videoWindow, WM_CLOSE, 0, NULL);
	InvalidateRect(NULL, TRUE);
	UpdateWindow();

	// Presto Studios logo

	videoWindow = MCIWndCreate(m_hWnd, afxCurrentInstanceHandle,
							   MAKELONG(MCIWNDF_NOPLAYBAR, MCIWNDF_NOERRORDLG),
							   GetPathToSupportFile("SUPPORT\\PRESTO.AVI"));

	::GetClientRect(videoWindow, &mci_rect);

	::SetWindowPos(videoWindow, 0,
				   (rect.right - mci_rect.right) / 2,
				   (rect.bottom - mci_rect.bottom) / 2,
				   0, 0,
				   SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_NOSIZE);

	MCIWndUseFrames(videoWindow);
	::ShowWindow(videoWindow, SW_SHOW);
	::SendMessage(videoWindow, MCI_PLAY, 0, NULL);

	do {
		if (PeekMessage(&msg, afxCurrentWinApp->m_pMainWnd->m_hWnd,
						WM_KEYFIRST, WM_KEYLAST, PM_REMOVE)) {
			::SendMessage(videoWindow, MCI_STOP, 0, NULL);
		}

		MCIWndGetMode(videoWindow, szMode, sizeof(szMode));
	} while (strcmp("stopped", szMode));

	::ShowWindow(videoWindow, SW_HIDE);
	::SendMessage(videoWindow, WM_CLOSE, 0, NULL);
	InvalidateRect(NULL, TRUE);
	UpdateWindow();

	// "Present..."

	HDIB hdib_present = OpenDIB(GetPathToSupportFile("SUPPORT\\PRESENT.BMP"));

	HBITMAP hbmp = BitmapFromDib(
		hdib_present,
		GetCurrentPal(),
		0);
	::GlobalUnlock(hdib_present);
	::GlobalFree(hdib_present);

	CDC *cdc = GetDC();
	HDC_FUN_1008_44b0(cdc->m_hDC, hbmp, 212, 222);
	ReleaseDC(cdc);

	while (PeekMessage(&msg, afxCurrentWinApp->m_pMainWnd->m_hWnd,
					   WM_LBUTTONDOWN, WM_LBUTTONUP, PM_REMOVE))
		;

	while (PeekMessage(&msg, afxCurrentWinApp->m_pMainWnd->m_hWnd,
					   WM_KEYFIRST, WM_KEYLAST, PM_REMOVE))
		;

	DWORD startTicks;
	for (startTicks = ::GetTickCount(); startTicks + 5000 > ::GetTickCount();) {
		if ((PeekMessage(&msg, afxCurrentWinApp->m_pMainWnd->m_hWnd,
						 WM_KEYFIRST, WM_KEYLAST, PM_REMOVE))) {
			break;
		}

		::Yield();
	}

	::DeleteObject(hbmp);
	InvalidateRect(NULL, TRUE);
	UpdateWindow();

	while (PeekMessage(&msg, afxCurrentWinApp->m_pMainWnd->m_hWnd,
					   WM_LBUTTONDOWN, WM_LBUTTONUP, PM_REMOVE))
		;

	while (PeekMessage(&msg, afxCurrentWinApp->m_pMainWnd->m_hWnd,
					   WM_KEYFIRST, WM_KEYLAST, PM_REMOVE))
		;

	// Journeyman Project Turbo logo

	videoWindow = MCIWndCreate(m_hWnd, afxCurrentInstanceHandle,
							   MAKELONG(MCIWNDF_NOPLAYBAR, MCIWNDF_NOERRORDLG),
							   GetPathToSupportFile("SUPPORT\\JMANSPIN.AVI"));

	::GetClientRect(videoWindow, &mci_rect);

	::SetWindowPos(videoWindow, 0,
				   (rect.right - mci_rect.right) / 2,
				   (rect.bottom - mci_rect.bottom) / 2,
				   0, 0,
				   SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_NOSIZE);

	MCIWndUseFrames(videoWindow);
	::ShowWindow(videoWindow, SW_SHOW);
	::SendMessage(videoWindow, MCI_PLAY, 0, NULL);

	do {
		if (PeekMessage(&msg, afxCurrentWinApp->m_pMainWnd->m_hWnd,
						WM_KEYFIRST, WM_KEYLAST, PM_REMOVE)) {
			::SendMessage(videoWindow, MCI_STOP, 0, NULL);
		}

		MCIWndGetMode(videoWindow, szMode, sizeof(szMode));
	} while (strcmp("stopped", szMode));

	// TODO: magic number
	::SendMessage(videoWindow, MCI_SEEK, 0,
				  ::SendMessage(videoWindow, 0x469, 0, NULL));

	while (PeekMessage(&msg, afxCurrentWinApp->m_pMainWnd->m_hWnd,
					   WM_LBUTTONDOWN, WM_LBUTTONUP, PM_REMOVE))
		;

	while (PeekMessage(&msg, afxCurrentWinApp->m_pMainWnd->m_hWnd,
					   WM_KEYFIRST, WM_KEYLAST, PM_REMOVE))
		;

	sndPlaySound(GetPathToSupportFile("SUPPORT\\CALTHEME.WAV"), SND_NODEFAULT | SND_ASYNC);

	startTicks = ::GetTickCount();
	do {
		if (PeekMessage(&msg, afxCurrentWinApp->m_pMainWnd->m_hWnd,
						WM_KEYFIRST, WM_KEYLAST, PM_REMOVE)) {
			startTicks -= 10000;
		}
	} while (startTicks + 10000 > ::GetTickCount());

	::ShowWindow(videoWindow, SW_HIDE);
	::SendMessage(videoWindow, WM_CLOSE, 0, NULL);
	ShowCursor(TRUE);

	return TRUE;
}

// FUNCTION: JMAN10 0x1028beb8
BOOL Titles::OnEraseBkgnd(CDC *pDC) {
	CBrush backBrush(RGB(0, 0, 0));
	CBrush *pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox(&rect);
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);
	return TRUE;
}
