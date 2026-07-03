#include "viewscn.h"

#include <afxwin.h>
#include <vfw.h>

#include "gamewin.h"
#include "jscene.h"
#include "navdata.h"
#include "prehist.h"

IMPLEMENT_DYNAMIC(Viewscreen, CWnd)

BEGIN_MESSAGE_MAP(Viewscreen, CWnd)
ON_WM_LBUTTONUP()
ON_WM_LBUTTONDOWN()
ON_WM_MOUSEMOVE()
ON_WM_TIMER()
// ON_MESSAGE(MCIWNDM_NOTIFYERROR, OnMCIError)
ON_MESSAGE(100, OnMCIError)
END_MESSAGE_MAP()

// STUB: JMAN10 0x10086a4c
Viewscreen::Viewscreen(CWnd *parent, int left, int top, NavData *navdata, NavMap *navmap, Inventory *inventory, int, int, DWORD) {
	// Create(NULL, "The Journeyman Project Turbo", WS_OVERLAPPEDWINDOW, rectDefault);
}

// STUB: JMAN10 0x10086ccc
void Viewscreen::MovePlayer(int dir) {
}

// FUNCTION: JMAN10 0x10086d62
BOOL Viewscreen::NewArea(int area, int scene, int dir) {
	if (((GameWindow *)GetParent())->m_invWnd) {
		((GameWindow *)GetParent())->m_invWnd->FUN_1020_063a();
	}

	pos_t nextpos; //?
	pos_t oldPos;
	pos_t nextScene = {area, scene, dir};

	// LINE: JMAN10 0x10086db8
	if (m_curScene) {
		if (m_curScene->OnLeave(this, nextScene)) {
			oldPos = m_curScene->m_scene.m_data.pos;
			m_curScene->~JScene(); // odd
			delete m_curScene;
			m_curScene = NULL;
		} else {
			((GameWindow *)GetParent())->m_compass->Oof();
			return FALSE;
		}
	} else {
		m_unk0x448 = 0;
		m_unk0x548 = 0;
		nextpos.scene = -1;
		nextpos.area = -1;
		nextpos.dir = -1;
	}

	if (((GameWindow *)GetParent())->m_bioChip) {
		((GameWindow *)GetParent())->m_bioChip->FUN_1020_294e(nextScene);
	}

	scene_s x_scene = m_navData->FUN_1008_60c6(area, scene, dir);
	switch (x_scene.m_data.pos.area) {
	case 1:
	case 2:
	case 3:
		// Caldoria
		break;

	case 4:
		// TSA
		break;

	case 5:
		// Prehistoric
		PrehistRouter(x_scene.m_data, this, m_navData, m_compass);
		break;

	case 6:
	case 7:
		// Mars Colony
		break;

	case 8:
		// Norad VI
		break;

	case 9:
		// WSC
		break;
	}

	return TRUE;
}

BOOL Viewscreen::SetTimers(void) {
	if (m_timer0 != NULL) {
		::KillTimer(m_hWnd, m_timer0);
	}
	m_timer0 = ::SetTimer(m_hWnd, 1, 1000, NULL);

	if (m_timer1 != NULL) {
		::KillTimer(m_hWnd, m_timer1);
	}
	m_timer1 = ::SetTimer(m_hWnd, 2, 100, NULL);

	if (m_timer2 != NULL) {
		::KillTimer(m_hWnd, m_timer2);
	}
	m_timer2 = ::SetTimer(m_hWnd, 3, 1000, NULL);

	if (m_timer3 != NULL) {
		::KillTimer(m_hWnd, m_timer3);
	}
	m_timer3 = ::SetTimer(m_hWnd, 4, 1000, NULL);

	return TRUE;
}

BOOL Viewscreen::KillTimers(void) {
	MSG msg;

	while (PeekMessage(&msg, m_hWnd, WM_TIMER, WM_TIMER, PM_REMOVE))
		;

	if (m_timer0 != NULL) {
		::KillTimer(m_hWnd, m_timer0);
	}
	m_timer0 = NULL;

	if (m_timer1 != NULL) {
		::KillTimer(m_hWnd, m_timer1);
	}
	m_timer1 = NULL;

	if (m_timer2 != NULL) {
		::KillTimer(m_hWnd, m_timer2);
	}
	m_timer2 = NULL;

	if (m_timer3 != NULL) {
		::KillTimer(m_hWnd, m_timer3);
	}
	m_timer3 = NULL;

	while (PeekMessage(&msg, m_hWnd, WM_TIMER, WM_TIMER, PM_REMOVE))
		;

	return TRUE;
}

// FUNCTION: JMAN10 0x10088418
void Viewscreen::OnLButtonUp(UINT nFlags, CPoint point) {
	if (m_curScene != NULL) {
		m_curScene->OnMouseUp(nFlags, point);
	}
}

// FUNCTION: JMAN10 0x100883e8
void Viewscreen::OnLButtonDown(UINT nFlags, CPoint point) {
	if (m_curScene != NULL) {
		m_curScene->OnMouseDown(nFlags, point);
	}
}

// FUNCTION: JMAN10 0x10088448
void Viewscreen::OnMouseMove(UINT nFlags, CPoint point) {
	if (m_curScene != NULL) {
		m_curScene->OnMouseMove(nFlags, point);
	}
}

// FUNCTION: JMAN10 0x10088a12
void Viewscreen::OnTimer(UINT nIDEvent) {
	if (GetWindowTask(m_hWnd) == GetCurrentTask() && m_curScene != NULL) {
		switch (nIDEvent) {
		case 1:
			m_curScene->Timer1();
			break;
		case 2:
			m_curScene->Timer2();
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
