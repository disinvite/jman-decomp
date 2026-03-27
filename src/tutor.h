#ifndef TUTOR_H
#define TUTOR_H

#include <afxwin.h>

#include "sprite.h"

// VTABLE: JMAN10 0x103010cc
// SIZE 0x88
class InterfaceOverview : public CWnd {
public:
	InterfaceOverview(CWnd *);

	// TODO: replace with individual members when they can be identified.
	short m_unk0x1c;       // 0x1c
	SpriteMgr m_spriteMgr; // 0x1e
	RECT m_hotspots[12];   // 0x28
};

#endif // TUTOR_H