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
	virtual BOOL OnEnter();
	virtual BOOL OnLeave(pos_t nextScene);
	virtual BOOL OnMouseUp(UINT, CPoint);
	virtual BOOL OnMouseDown(UINT, CPoint);
	virtual BOOL OnMouseMove(UINT, CPoint);
	virtual BOOL OnTimer();
	void Timer1();
	void Timer2();

	char *m_ambientSoundPath; // 0x008
	NavData *m_navData;       // 0x108
	scene_t m_scene;          // 0x110
};

//  implemented in each scene class as needed.
// 	short m_sceneState;       // 0x13c

#endif
