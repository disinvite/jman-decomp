#include "norad.h"

#include <afxwin.h>

NoradGasBottle::NoradGasBottle() {
}

// STUB: JMAN10 0x1020968e
BOOL NoradGasBottle::OnTimer() {
	// GetPathToSupportFile("SUPPORT\\NORAD\\SUCKING.WAV")
	// GetPathToSupportFile("SUPPORT\\NORAD\\OMASK.WAV")
	// GetPathToSupportFile("SUPPORT\\NORAD\\N_21.WAV")
	return TRUE;
}

BOOL NoradGasBottle::OnClick(CPoint point, CWnd *win) {
	RECT rect;
	::SetRect(&rect, 0x164, 0x68, 0x19e, 0xb6);

	if (PtInRect(&rect, point) == 1) {
		win->MessageBox(MSG_SPOILFUN, NULL, MB_OK);
		afxCurrentWinApp->m_pMainWnd->DestroyWindow();
	}

	return TRUE;
}
