#include "sprite.h"

// FUNCTION: JMAN10 0x1028b2a0
SpriteMgr::SpriteMgr(const char *path) {
	// For lack of a better definition.
	// This manage sprites embedded in video files (e.g. SHUTTLE.AVI, OBJECTS.AVI).
	// It also handles full screen graphics in the same fashion (i.e. with no transparency)
	// like DREAM.AVI and TUTOR.AVI.
	m_drawDib = 0;
	m_stream = NULL;
	m_frame = NULL;

	m_drawDib = DrawDibOpen();
	AVIFileInit();
	if (path) {
		FUN_1028_b324(path);
	}
}

// FUNCTION: JMAN10 0x1028b324
short SpriteMgr::FUN_1028_b324(const char *path) {
	int fh;
	OFSTRUCT of;

	fh = OpenFile(path, &of, OF_EXIST);

	if (fh == -1) {
		return 0;
	}

	FUN_1028_b3b0();
	AVIStreamOpenFromFile(&m_stream, path, streamtypeVIDEO, 0, 0, NULL);
	m_frame = AVIStreamGetFrameOpen(m_stream, NULL);

	return 1;
}

// FUNCTION: JMAN10 0x1028b3b0
short SpriteMgr::FUN_1028_b3b0() {
	if (m_frame) {
		AVIStreamGetFrameClose(m_frame);
	}

	m_frame = 0;

	if (m_stream) {
		AVIStreamRelease(m_stream);
	}

	m_stream = 0;

	return 1;
}