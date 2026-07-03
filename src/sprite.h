#ifndef SPRITE_H
#define SPRITE_H

#include <afxwin.h>
#include <vfw.h>

// SIZE 0xa?
class SpriteMgr {
public:
	SpriteMgr() {};
	SpriteMgr(const char *);

	short FUN_1028_b324(const char *);
	short FUN_1028_b3b0();

	HDRAWDIB m_drawDib;   // 0x00
	IAVIStream *m_stream; // 0x02
	IGetFrame *m_frame;   // 0x06
};

#endif // SPRITE_H