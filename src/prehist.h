#ifndef PREHIST_H
#define PREHIST_H

#include <afxwin.h>

#include "compass.h"
#include "jscene.h"
#include "navdata.h"

// VTABLE: JMAN10 0x10206324
class PrehistBaseScene : public JScene {
};

// VTABLE: JMAN10 0x10206370
class PrehistKeyScene : public PrehistBaseScene {
};

// VTABLE: JMAN10 0x1020628c
class PrehistTRexScene : public PrehistBaseScene {
};

extern JScene *PrehistRouter(nav_combined_t, CWnd *, NavData *, Compass *);

#endif
