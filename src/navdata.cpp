#include "navdata.h"

#include <afxwin.h>

// NAVDATA comes in two sections:
// 128 byte block for each "_NAV" AVI file used
// 42 byte struct for each scene in the game
// Each section begins with one word that gives the length of the section.

// FUNCTION: JMAN10 0x10085e6a
NavData::NavData() {
	HGLOBAL gl = LoadResource(afxCurrentResourceHandle,
							  FindResource(afxCurrentResourceHandle, "BDATA", "NAVDATA"));

	int *res_navdata = (int *)LockResource(gl);

	// Read first word to get the length of the upcoming section
	int n_avi_blocks = *res_navdata;

	// Read AVI data into memory
	_glAvifiles = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE, n_avi_blocks * 128);
	int *nav_avi = (int *)LockResource(_glAvifiles);
	res_navdata++;
	memcpy(nav_avi, res_navdata, n_avi_blocks * 128);
	UnlockResource(_glAvifiles);

	// Seek data pointer ahead to scene section
	res_navdata += (n_avi_blocks * 64); // 64 words = 128 bytes.
	_n_scenes = *res_navdata;
	res_navdata++;

	// Read scene data into memory
	_glScenes = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE, _n_scenes * 42);
	int *nav_scene = (int *)LockResource(_glScenes);
	memcpy(nav_scene, res_navdata, _n_scenes * 42);
	UnlockResource(_glScenes);

	UnlockResource(gl);
	FreeResource(gl);
}

// FUNCTION: JMAN10 0x10085fa4
NavData::~NavData() {
	if (_glScenes != 0) {
		GlobalFree(_glScenes);
	}

	if (_glAvifiles != 0) {
		GlobalFree(_glAvifiles);
	}

	_glScenes = 0;
	_glAvifiles = 0;
}

// FUNCTION: JMAN10 0x10085ffc
scene_t *NavData::FUN_1008_5ffc(scene_t *ptr, pos_t pos) {
	scene_t scene;
	scene.pos.scene = -1;
	scene.pos.area = -1;
	scene.pos.dir = -1;
	BOOL found = FALSE;

	if (_glScenes != 0) {
		scene_t *scenes = (scene_t *)GlobalLock(_glScenes);
		// redundant, but intentional.
		int max = (int)_n_scenes;
		for (int i = 0; i < max; i++) {
			if (found) {
				break;
			}

			if (scenes->pos.scene == pos.scene &&
				scenes->pos.area == pos.area &&
				scenes->pos.dir == pos.dir) {
				found = TRUE;
			} else {
				scenes++;
			}
		}

		if (found == TRUE) {
			memcpy(&scene, scenes, sizeof(scene_t));
		}

		GlobalUnlock(_glScenes);
	}
	memcpy(ptr, &scene, sizeof(scene_t));

	return ptr;
}

// FUNCTION: JMAN10 0x100860c6
scene_t *NavData::FUN_1008_60c6(scene_t *ptr, int area, int scene, int dir) {
	pos_t pos = {area, scene, dir};
	FUN_1008_5ffc(ptr, pos);
	return ptr;
}

HGLOBAL NavData::GetScenesHandle() {
	return _glScenes;
}

UINT NavData::GetScenesCount() {
	return _n_scenes;
}

// STUB: JMAN10 0x1008623a
NavMap::NavMap() {
}

// STUB: JMAN10 0x10086264
NavMap::~NavMap() {
}

// TODO
BOOL NavMap::Setup(HGLOBAL scenes, UINT n_scenes) {
	if (visits_)
		GlobalFree(visits_);

	if (h8_)
		GlobalFree(h8_);

	visits_ = 0;
	h8_ = 0;

	h8_ = GlobalAlloc(GHND, 68);

	count_ = n_scenes;
	UINT count = n_scenes;
	visits_ = GlobalAlloc(GHND, n_scenes * sizeof(visited_t));

	visited_t *pVisited = (visited_t *)GlobalLock(visits_);
	scene_t *pScene = (scene_t *)GlobalLock(scenes);

	// TODO: mempcy section is wrong
	for (UINT i = 0; i < n_scenes; i++) {
		pVisited[i].pos = pScene[i].pos;
	}

	GlobalUnlock(visits_);
	GlobalUnlock(scenes);

	return TRUE;
}
