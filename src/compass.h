#ifndef COMPASS_H
#define COMPASS_H

#include <afxwin.h>

#include "navdata.h"

// VTABLE: JMAN10 0x10089d22
class Compass : public CWnd {
public:
	Compass(CWnd *parent, int left, int top);
	~Compass();

	HWND hCompass_;
	int current_dir_;

	BOOL UpdateHeading(int dir);
	BOOL Oof();
	BOOL Spin(int which);

protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

#endif
