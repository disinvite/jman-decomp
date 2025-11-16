#include "btnbar.h"

#include <afxwin.h>

#include "gamewin.h"
#include "util.h"

BEGIN_MESSAGE_MAP(BtnBar, CWnd)
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// FUNCTION: JMAN10 0x10089d8e
BtnBar::BtnBar(CFrameWnd *cf, int left, int top) {
	bmp_ = 0;
	active_btn_ = 0;
	prop_20_ = FALSE;

	HDIB btn = OpenDIB(GetPathToSupportFile("SUPPORT\\BTNBAR.BMP"));
	bmp_ = BitmapFromDib(btn, GetCurrentPal(), 0);
	GlobalUnlock(btn);
	GlobalFree(btn);

	RECT rect = {left, top, left + 161, top + 32};
	Create(NULL, NULL, 0x40000000, rect, cf, 0, NULL);
}

// FUNCTION: JMAN10 0x10089e4a
BtnBar::~BtnBar() {
	if (bmp_ != 0) {
		DeleteObject(bmp_);
	}
}

// FUNCTION: JMAN10 0x10089e80
void BtnBar::OnPaint() {
	PAINTSTRUCT paint;

	CDC *cdc = BeginPaint(&paint);
	HDC_FUN_1008_453e(cdc->m_hDC, 0, 0, 161, 32, bmp_, active_btn_ * 32, 0);

	EndPaint(&paint);
}

// FUNCTION: JMAN10 0x10089eea
void BtnBar::OnLButtonDown(UINT nFlags, CPoint point) {
	active_btn_ = (point.x / 40) + 1;
	DWORD t_time = GetCurrentTime();

	Invalidate(FALSE);
	UpdateWindow();

	switch (active_btn_) {
	case 1:
		((GameWindow *)GetParent())->viewscreen_->MovePlayer(1);
		break;
	case 2:
		((GameWindow *)GetParent())->viewscreen_->MovePlayer(0);
		break;
	case 3:
		((GameWindow *)GetParent())->viewscreen_->MovePlayer(2);
		break;
	case 4:
		((GameWindow *)GetParent())->viewscreen_->MovePlayer(3);
		break;
	}

	while (t_time + 100 > GetCurrentTime()) {
		Yield();
	}

	active_btn_ = 0;

	Invalidate(FALSE);
	UpdateWindow();
}
