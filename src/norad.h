#ifndef NORAD_H
#define NORAD_H

#include <afxwin.h>

#include "jscene.h"

// #define MSG_SPOILFUN "Hey, I hate to spoil your fun, but guess what, you died from contact with a poisoned bottle."
#define MSG_SPOILFUN "Test"

class NoradGasBottle : public JScene {
public:
	NoradGasBottle();
	BOOL NoradGasBottle::OnClick(CPoint, CWnd *);
	BOOL NoradGasBottle::OnTimer();
};

#endif
