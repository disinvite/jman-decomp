#include "btnbar.h"

#include <afxwin.h>

#include "gamewin.h"
#include "util.h"

BEGIN_MESSAGE_MAP(BtnBar, CWnd)
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// FUNCTION: JMAN10 0x10089d8e
BtnBar::BtnBar(CWnd *parent, int left, int top) {
	m_bitmap = 0;
	m_activeButton = 0;
	m_unk0x20 = FALSE;

	HDIB btn = OpenDIB(GetPathToSupportFile("SUPPORT\\BTNBAR.BMP"));
	m_bitmap = BitmapFromDib(btn, GetCurrentPal(), 0);
	GlobalUnlock(btn);
	GlobalFree(btn);

	RECT rect = {left, top, left + 161, top + 32};
	Create(NULL, NULL, WS_CHILD, rect, parent, 0, NULL);
}

// FUNCTION: JMAN10 0x10089e4a
BtnBar::~BtnBar() {
	if (m_bitmap != 0) {
		DeleteObject(m_bitmap);
	}
}

// FUNCTION: JMAN10 0x10089e80
void BtnBar::OnPaint() {
	PAINTSTRUCT paint;

	CDC *cdc = BeginPaint(&paint);
	HDC_FUN_1008_453e(cdc->m_hDC, 0, 0, 161, 32, m_bitmap, 0, m_activeButton * 32);

	EndPaint(&paint);
}

// FUNCTION: JMAN10 0x10089eea
void BtnBar::OnLButtonDown(UINT nFlags, CPoint point) {
	m_activeButton = (point.x / 40) + 1;
	DWORD t_time = GetTickCount();

	Invalidate(FALSE);
	UpdateWindow();

	switch (m_activeButton) {
	case 1:
		((GameWindow *)GetParent())->m_viewScreen->MovePlayer(1);
		break;
	case 2:
		((GameWindow *)GetParent())->m_viewScreen->MovePlayer(0);
		break;
	case 3:
		((GameWindow *)GetParent())->m_viewScreen->MovePlayer(2);
		break;
	case 4:
		((GameWindow *)GetParent())->m_viewScreen->MovePlayer(3);
		break;
	}

	while (t_time + 100 > GetTickCount()) {
		Yield();
	}

	m_activeButton = 0;

	Invalidate(FALSE);
	UpdateWindow();
}
