#ifndef JMANWIN_H
#define JMANWIN_H

#include <afxwin.h>

// VTABLE: JMAN 10081082
class JmanWindow : public CFrameWnd {
public:
    JmanWindow();
    ~JmanWindow();
    BOOL ShowMenu();
    BOOL ShowTitles();
    BOOL ShowSepia();
    BOOL ShowDream();
    BOOL NewGame();

    BOOL _game_should_exit;
    CWnd* _curwindow;

protected:
    afx_msg void OnKeyDown(UINT, UINT, UINT);
    afx_msg void OnKeyUp(UINT, UINT, UINT);
    DECLARE_MESSAGE_MAP()
};

// GLOBAL: JMAN 10000000
// JmanWindow::_messageEntries

#endif
