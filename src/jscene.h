#ifndef JSCENE_H
#define JSCENE_H

#include <afxwin.h>

#include "navdata.h"

// VTABLE: JMAN10 0x10089294
class JScene : public CObject {
public:
	JScene();
	virtual BOOL OnEnter();
	virtual BOOL OnLeave(pos_t nextScene);
	virtual BOOL OnMouseUp(UINT, CPoint);
	virtual BOOL OnMouseDown(UINT, CPoint);
	virtual BOOL OnMouseMove(UINT, CPoint);
	virtual BOOL OnTimer();
	void Timer1();
	void Timer2();

	pos_t curpos; // 0x110?
};

#endif
