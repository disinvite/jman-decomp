#include "msgwnd.h"

#include <afxwin.h>
#include <vfw.h>

#include "util.h"

BEGIN_MESSAGE_MAP(MsgWnd, CWnd)
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

#define WaitForMCIStopped(_buf)             \
	do {                                    \
		Yield();                            \
		MCIWndGetMode(hMessage_, _buf, 64); \
	} while (strcmp("stopped", _buf))

// FUNCTION: JMAN10 0x1008513c
MsgWnd::MsgWnd(CWnd *parent, int left, int top) {
	TCHAR szMode[64];

	hMessage_ = 0;
	windowOpen_ = FALSE;
	canvas_ = 0;
	isPrompt_ = FALSE;

	str1_.Empty();
	str2_.Empty();
	str3_.Empty();

	font_.CreateFont(14, 0, 0, 0, 700, 0, 0, 0, 0, 4, 1, 2, 1, "Arial");

	RECT rect = {left, top, left + 128, top + 168};
	Create(NULL, NULL, 0x40000000, rect, parent, 0, NULL);

	ShowWindow(SW_SHOW);

	hMessage_ = MCIWndCreate(m_hWnd, afxCurrentInstanceHandle,
							 MAKELONG(MCIWNDF_NOPLAYBAR, MCIWNDF_NOERRORDLG),
							 GetPathToSupportFile("SUPPORT\\MESSAGE.AVI"));

	if (hMessage_ != 0) {
		MCIWndSeek(hMessage_, 0);
		WaitForMCIStopped(szMode);
		CDC *cdc = GetDC();
		canvas_ = ::CreateCompatibleBitmap(cdc->m_hDC, 128, 168);
		HDC_FUN_1008_47a8(cdc->m_hDC, m_hWnd, 0, 0, 0, 0, 0);
		ReleaseDC(cdc);
	}
}

// FUNCTION: JMAN10 0x100853a4
MsgWnd::~MsgWnd() {
}

// STUB: JMAN10 0x10085442
void MsgWnd::NewMessage(char *str) {
	str1_ = str;
	// TypeText()
}

BOOL MsgWnd::TypeText() {
	TCHAR szMode[64];

	if (!hMessage_)
		return FALSE;

	// prop 58 = 0;

	// If the window is not open yet, open it.
	if (!windowOpen_) {
		MCIWndSeek(hMessage_, 1);
		WaitForMCIStopped(szMode);
		::ShowWindow(hMessage_, SW_SHOW);

		MCIWndPlayTo(hMessage_, 13);
		WaitForMCIStopped(szMode);
		windowOpen_ = TRUE;
		::ShowWindow(hMessage_, SW_HIDE);
	}

	// message string empty?
	if (str1_.GetLength() == 0 && canvas_ != 0) {
		CDC *cdc = GetDC();
		HDC_FUN_1008_47a8(cdc->m_hDC, hMessage_, canvas_, 0, 0, 0, 0);
		ReleaseDC(cdc);

		Invalidate(FALSE);
		UpdateWindow();
	}

	// message string not empty?
	if (str1_.GetLength() != 0 && canvas_ != 0) {
		CDC tCDC;    // 2e
		CFont tFont; // 1a/1c

		CDC *cdc = GetDC();
		HDC_FUN_1008_47a8(cdc->m_hDC, hMessage_, canvas_, 0, 0, 0, 0);
		// cdc->CreateCompatibleDC(&tCDC);
		tCDC.CreateCompatibleDC(cdc);

		CBitmap *bmp = CBitmap::FromHandle(canvas_);
		CBitmap *old_bmp = cdc->SelectObject(bmp);
		CFont *old_font = tCDC.SelectObject(&font_);

		COLORREF old_color = tCDC.SetTextColor(RGB(255, 255, 255));
		int old_bk = tCDC.SetBkMode(TRANSPARENT);

		// Main message
		RECT rect;
		SetRect(&rect, 20, 10, 118, 160);
		tCDC.DrawText(str1_, -1, &rect, DT_NOCLIP | DT_WORDBREAK);

		// Option 1 (e.g. "Use")
		if (isPrompt_) {
			SetRect(&rect, 20, 100, 118, 120);
			tCDC.DrawText(str2_, -1, &rect, DT_NOCLIP | DT_WORDBREAK);

			// Option 2 (e.g. "Remove")
			SetRect(&rect, 20, 130, 118, 150);
			tCDC.DrawText(str3_, -1, &rect, DT_NOCLIP | DT_WORDBREAK);
		}

		tCDC.SetBkMode(old_bk);
		tCDC.SetTextColor(old_color);
		tCDC.SelectObject(old_font);
		tCDC.SelectObject(old_bmp);

		HDC_FUN_1008_4876(m_hWnd, cdc->m_hDC, 0, 0, 128, 168,
						  canvas_, 0, 0, 160, 500);

		ReleaseDC(cdc);
	}

	return TRUE;
}

BOOL MsgWnd::CloseWindow() {
	TCHAR szMode[64];

	if (!hMessage_)
		return FALSE;

	MCIWndSeek(hMessage_, 14);
	WaitForMCIStopped(szMode);

	::ShowWindow(hMessage_, SW_SHOW);

	MCIWndPlayTo(hMessage_, 25);
	WaitForMCIStopped(szMode);

	windowOpen_ = FALSE;
	isPrompt_ = FALSE;
	// dword prop 32 = 0;

	::ShowWindow(hMessage_, SW_HIDE);

	Invalidate(FALSE);
	UpdateWindow();

	return TRUE;
}

void MsgWnd::OnPaint() {
	PAINTSTRUCT paint;

	CDC *cdc = BeginPaint(&paint);
	if (canvas_ != 0 && windowOpen_) {
		HDC_FUN_1008_44b0(cdc->m_hDC, canvas_, 0, 0);
	} else {
		hdc_UpdateHDC(hMessage_, cdc->m_hDC, 0, 0, 0, 0);
	}

	EndPaint(&paint);
}

void MsgWnd::OnLButtonDown(UINT nFlags, CPoint point) {
	if (!windowOpen_) {
		TypeText();
	} else {
		if (isPrompt_) {
			RECT opt1;
			::SetRect(&opt1, 20, 100, 118, 120);

			RECT opt2;
			::SetRect(&opt2, 20, 130, 118, 150);

			if (PtInRect(&opt1, point)) {
			}
			// TODO
		} else {
			CloseWindow();
		}
	}
}

#undef WaitForMCIStopped
