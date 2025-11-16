#ifndef DIALOG_H
#define DIALOG_H

#include <afxwin.h>

#define RES_BASEPATH 110
#define RES_BASEPATH_TEXT 111
#define RES_BASEPATH_INPUT 113
#define RES_HICOLORERROR 120
#define RES_HICOLORWARNING 140
#define RES_JUMPTOSCENE 150

class BasePathDialog : public CDialog {
public:
	char _header_text[256];

protected:
	virtual BOOL OnInitDialog();
};

#endif
