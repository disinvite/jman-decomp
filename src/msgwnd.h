#ifndef MSGWND_H
#define MSGWND_H

#include <afxwin.h>

// SIZE 0x5a
class MsgWnd : public CWnd {
public:
	MsgWnd(CWnd *parent, int left, int top);
	~MsgWnd();

	HWND hMessage_;     // 0x1c
	BOOL windowOpen_;   // 0x1e
	CString str1_;      // 0x20
	CFont font_;        // 0x28
	HBITMAP canvas_;    // 0x2e
	BOOL isPrompt_;     // 0x30
	DWORD m_unk0x32;    // 0x32
	CString str2_;      // 0x36
	char m_unk0x3a[8];  // 0x3e
	CString str3_;      // 0x46
	char m_unk0x4e[10]; // 0x4e
	WORD m_unk0x58;     // 0x58

	void NewMessage(char *str);
	BOOL TypeText();
	BOOL CloseWindow();

protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#endif
