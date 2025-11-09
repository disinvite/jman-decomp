#ifndef MSGWND_H
#define MSGWND_H

#include <afxwin.h>

class MsgWnd : public CWnd {
public:
	MsgWnd(CWnd *parent, int left, int top);
	~MsgWnd();

	HWND hMessage_;   // 0x1c
	BOOL windowOpen_; // 0x1e
	CString str1_;    // 0x20
	CFont font_;      // 0x28
	// prop 0x2c
	HBITMAP canvas_; // 0x2e
	BOOL isPrompt_;  // 0x30
	// prop 0x32
	// prop 0x34
	CString str2_; // 0x36
	CString str3_; // 0x46
	// prop 0x58

	void NewMessage(char *str);
	BOOL TypeText();
	BOOL CloseWindow();

protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#endif
