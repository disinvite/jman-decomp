#include <afxwin.h>
#include <windows.h>

#include "jman.h"
#include "jmanwin.h"
#include "util.h"

// STUB: JMAN10 0x10080f12
JManApp::JManApp() {
}

// FUNCTION: JMAN10 0x10080cac
BOOL JManApp::InitInstance() {
	if (m_hPrevInstance != NULL) {
		return FALSE;
	}

	if (!Startup_DialogHiColorWarning()) {
		return FALSE;
	}

	if (!Startup_BasePathWarning()) {
		return FALSE;
	}

	m_pMainWnd = new JmanWindow();
	m_pMainWnd->ShowWindow(SW_NORMAL);
	m_pMainWnd->UpdateWindow();

	((JmanWindow *)m_pMainWnd)->ShowTitles();

	return TRUE;
}

JManApp journeyman;
