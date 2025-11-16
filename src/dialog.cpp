#include "dialog.h"

#include <afxwin.h>

// FUNCTION: JMAN10 0x1008164a
BOOL BasePathDialog::OnInitDialog() {
	RECT rect;

	GetWindowRect(&rect);
	int ypos = (GetSystemMetrics(SM_CYSCREEN) - rect.top - rect.bottom) / 2;
	int xpos = (GetSystemMetrics(SM_CXSCREEN) - rect.left - rect.right) / 2;
	::SetWindowPos(m_hWnd, NULL, xpos, ypos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	SetDlgItemText(RES_BASEPATH_TEXT, _header_text);
	CEdit *edit = (CEdit *)GetDlgItem(RES_BASEPATH_INPUT);
	edit->LimitText(255);

	char szPathName[2 * _MAX_FNAME];
	::GetModuleFileName(AfxGetInstanceHandle(), szPathName, 2 * _MAX_FNAME);
	*_fstrrchr(szPathName, '\\') = '\0';

	SetDlgItemText(RES_BASEPATH_INPUT, szPathName);

	return TRUE;
}
