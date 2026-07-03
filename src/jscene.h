#ifndef JSCENE_H
#define JSCENE_H

#include <afxwin.h>

#include "navdata.h"

// VTABLE: JMAN10 0x10089294
// SIZE 0x13c
class JScene : public CObject {
public:
	JScene();
	virtual ~JScene();
	virtual BOOL OnEnter(UINT, UINT, UINT, UINT, UINT);
	virtual BOOL OnLeave(CWnd *, pos_t nextScene);
	virtual BOOL OnMouseUp(UINT, CPoint);
	virtual BOOL OnMouseDown(UINT, CPoint);
	virtual BOOL OnMouseMove(UINT, CPoint);
	virtual BOOL OnTimer();
	void Timer1();
	void Timer2();

	BYTE m_unk0x04[4];              // 0x004
	char *m_ambientSoundPath;       // 0x008
	BYTE m_unk0x0c[0x108 - 0x00c];  // 0x00c
	NavData *m_navData;             // 0x108
	BYTE m_unk0x10a[0x110 - 0x10c]; // 0x10c
	scene_t m_scene;                // 0x110
};

//  implemented in each scene class as needed.
// 	short m_sceneState;       // 0x13c

#endif
