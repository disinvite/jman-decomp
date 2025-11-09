#ifndef TITLES_H
#define TITLES_H

#include <afxwin.h>

class Titles : public CWnd {
protected:
	DECLARE_DYNAMIC(Titles)

public:
	Titles(CFrameWnd *);
	void Logos();

protected:
	afx_msg BOOL OnEraseBkgnd(CDC *pDC);
	DECLARE_MESSAGE_MAP()
};

#endif
