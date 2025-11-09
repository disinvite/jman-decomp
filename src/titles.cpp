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
void Titles::Logos() {
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
	::InvalidateRect(m_hWnd, NULL, TRUE);
	::UpdateWindow(m_hWnd);

	// "Present..."

	HDIB hdib_present = OpenDIB(GetPathToSupportFile("SUPPORT\\PRESENT.BMP"));

	HBITMAP hbmp = BitmapFromDib(
		hdib_present,
		GetCurrentPal(),
		0);
	::GlobalUnlock(hdib_present);
	::GlobalFree(hdib_present);

	CDC *cdc = CDC::FromHandle(::GetDC(m_hWnd));
	HDC_FUN_1008_44b0(cdc->m_hDC, hbmp, 212, 222);
	::ReleaseDC(m_hWnd, cdc->m_hDC);

	while (PeekMessage(&msg, afxCurrentWinApp->m_pMainWnd->m_hWnd,
					   WM_LBUTTONDOWN, WM_LBUTTONUP, PM_REMOVE))
		;

	while (PeekMessage(&msg, afxCurrentWinApp->m_pMainWnd->m_hWnd,
					   WM_KEYFIRST, WM_KEYLAST, PM_REMOVE))
		;

	DWORD start_time = ::GetCurrentTime();
	while (::GetCurrentTime() >= (start_time + 5000)) {
		if ((PeekMessage(&msg, afxCurrentWinApp->m_pMainWnd->m_hWnd,
						 WM_KEYFIRST, WM_KEYLAST, PM_REMOVE))) {
			break;
		}

		::Yield();

		if (::GetCurrentTime() < (start_time + 5000)) {
			break;
		}
	}

	::DeleteObject(hbmp);
	::InvalidateRect(m_hWnd, NULL, TRUE);
	::UpdateWindow(m_hWnd);

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

	::ShowWindow(videoWindow, SW_HIDE);
	::SendMessage(videoWindow, WM_CLOSE, 0, NULL);
	::InvalidateRect(m_hWnd, NULL, TRUE);
	::UpdateWindow(m_hWnd);

	::ShowCursor(TRUE);
}

// FUNCTION: JMAN10 0x1028beb8
BOOL Titles::OnEraseBkgnd(CDC *pDC) {
	CBrush backBrush;
	CBrush *pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox(&rect);
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);
	return TRUE;
}
