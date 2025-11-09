#include "viewscn.h"

#include <afxwin.h>
#include <vfw.h>

#include "gamewin.h"
#include "jscene.h"
#include "navdata.h"

IMPLEMENT_DYNAMIC(Viewscreen, CWnd)

BEGIN_MESSAGE_MAP(Viewscreen, CWnd)
ON_WM_LBUTTONUP()
ON_WM_LBUTTONDOWN()
ON_WM_MOUSEMOVE()
ON_WM_TIMER()
// ON_MESSAGE(MCIWNDM_NOTIFYERROR, OnMCIError)
ON_MESSAGE(100, OnMCIError)
END_MESSAGE_MAP()

Viewscreen::Viewscreen(void) {
	// Create(NULL, "The Journeyman Project Turbo", WS_OVERLAPPEDWINDOW, rectDefault);
}

void Viewscreen::MovePlayer(int dir) {
}

BOOL Viewscreen::NewArea(int scene, int area, int dir) {
	//((GameWindow)*GetParent())->invwnd_

	pos_t nextpos; //?
	pos_t oldPos;
	pos_t nextScene = {scene, area, dir};

	if (_current_scene) {
		if (_current_scene->OnLeave(nextScene)) {
			oldPos = _current_scene->curpos;
			_current_scene->~JScene(); // odd
			delete _current_scene;
			_current_scene = NULL;
		} else {
			((GameWindow *)GetParent())->compass_->Oof();
			return FALSE;
		}
	} else {
		// byte +458 = 0
		// word +548 = 0
		nextpos.scene = -1;
		nextpos.area = -1;
		nextpos.dir = -1;
	}

	return TRUE;
}

BOOL Viewscreen::SetTimers(void) {
	if (_timer0 != NULL) {
		::KillTimer(m_hWnd, _timer0);
	}
	_timer0 = ::SetTimer(m_hWnd, 1, 1000, NULL);

	if (_timer1 != NULL) {
		::KillTimer(m_hWnd, _timer1);
	}
	_timer1 = ::SetTimer(m_hWnd, 2, 100, NULL);

	if (_timer2 != NULL) {
		::KillTimer(m_hWnd, _timer2);
	}
	_timer2 = ::SetTimer(m_hWnd, 3, 1000, NULL);

	if (_timer3 != NULL) {
		::KillTimer(m_hWnd, _timer3);
	}
	_timer3 = ::SetTimer(m_hWnd, 4, 1000, NULL);

	return TRUE;
}

BOOL Viewscreen::KillTimers(void) {
	MSG msg;

	while (PeekMessage(&msg, m_hWnd, WM_TIMER, WM_TIMER, PM_REMOVE))
		;

	if (_timer0 != NULL) {
		::KillTimer(m_hWnd, _timer0);
	}
	_timer0 = NULL;

	if (_timer1 != NULL) {
		::KillTimer(m_hWnd, _timer1);
	}
	_timer1 = NULL;

	if (_timer2 != NULL) {
		::KillTimer(m_hWnd, _timer2);
	}
	_timer2 = NULL;

	if (_timer3 != NULL) {
		::KillTimer(m_hWnd, _timer3);
	}
	_timer3 = NULL;

	while (PeekMessage(&msg, m_hWnd, WM_TIMER, WM_TIMER, PM_REMOVE))
		;

	return TRUE;
}

// FUNCTION: JMAN10 0x10088418
void Viewscreen::OnLButtonUp(UINT nFlags, CPoint point) {
	if (_current_scene != NULL) {
		_current_scene->OnMouseUp(nFlags, point);
	}
}

// FUNCTION: JMAN10 0x100883e8
void Viewscreen::OnLButtonDown(UINT nFlags, CPoint point) {
	if (_current_scene != NULL) {
		_current_scene->OnMouseDown(nFlags, point);
	}
}

// FUNCTION: JMAN10 0x10088448
void Viewscreen::OnMouseMove(UINT nFlags, CPoint point) {
	if (_current_scene != NULL) {
		_current_scene->OnMouseMove(nFlags, point);
	}
}

// FUNCTION: JMAN10 0x10088a12
void Viewscreen::OnTimer(UINT nIDEvent) {
	if (GetWindowTask(m_hWnd) == GetCurrentTask() && _current_scene != NULL) {
		switch (nIDEvent) {
		case 1:
			_current_scene->Timer1();
			break;
		case 2:
			_current_scene->Timer2();
			break;
		case 3:
			break;
		case 4:
			// Update energy counter.
			CWnd *wnd = FromHandle(::GetParent(m_hWnd));
			RECT rect;
			::SetRect(&rect, 195, 64, 262, 75);

			::InvalidateRect(m_hWnd, &rect, FALSE);
			::UpdateWindow(m_hWnd);
			break;
		}
	}
}

// FUNCTION: JMAN10 0x10088cde
LRESULT Viewscreen::OnMCIError(WPARAM wp, LPARAM lp) {
	MessageBox("MCI ERROR PASSED OUR WAY", NULL, MB_OK);
	return TRUE;
}
