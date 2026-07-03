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
	m_glAvifiles = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE, n_avi_blocks * 128);
	int *nav_avi = (int *)GlobalLock(m_glAvifiles);
	res_navdata++;
	memcpy(nav_avi, res_navdata, n_avi_blocks * 128);
	UnlockResource(m_glAvifiles);

	// Seek data pointer ahead to scene section
	res_navdata += (n_avi_blocks * 64); // 64 words = 128 bytes.
	m_sceneCount = *res_navdata;
	res_navdata++;

	// Read scene data into memory
	m_glScenes = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE, m_sceneCount * 42);
	int *nav_scene = (int *)GlobalLock(m_glScenes);
	memcpy(nav_scene, res_navdata, m_sceneCount * 42);
	UnlockResource(m_glScenes);

	UnlockResource(gl);
	FreeResource(gl);
}

// FUNCTION: JMAN10 0x10085fa4
NavData::~NavData() {
	if (m_glScenes != 0) {
		GlobalFree(m_glScenes);
	}

	if (m_glAvifiles != 0) {
		GlobalFree(m_glAvifiles);
	}

	m_glAvifiles = 0;
	m_glScenes = 0;
}

// FUNCTION: JMAN10 0x10085ffc
scene_t NavData::FUN_1008_5ffc(pos_t pos) {
	scene_t scene;
	scene.m_data.pos.area = -1;
	scene.m_data.pos.scene = -1;
	scene.m_data.pos.dir = -1;
	BOOL found = FALSE;

	if (m_glScenes != 0) {
		scene_t *scenes = (scene_t *)GlobalLock(m_glScenes);
		// redundant, but intentional.
		int max = (int)m_sceneCount;
		for (int i = 0; i < max; i++) {
			if (found) {
				break;
			}

			if (scenes->m_data.pos.area == pos.area &&
				scenes->m_data.pos.scene == pos.scene &&
				scenes->m_data.pos.dir == pos.dir) {
				found = TRUE;
			} else {
				scenes++;
			}
		}

		if (found == TRUE) {
			memcpy(&scene, scenes, sizeof(scene_t));
		}

		GlobalUnlock(m_glScenes);
	}

	return scene;
}

// FUNCTION: JMAN10 0x100860c6
scene_t NavData::FUN_1008_60c6(int area, int scene, int dir) {
	pos_t pos = {area, scene, dir};
	return FUN_1008_5ffc(pos);
}

// FUNCTION: JMAN10 0x1008621e
HGLOBAL NavData::GetScenesHandle() {
	return m_glScenes;
}

// FUNCTION: JMAN10 0x1008622c
UINT NavData::GetScenesCount() {
	return m_sceneCount;
}

// STUB: JMAN10 0x1008623a
NavMap::NavMap() {
}

// STUB: JMAN10 0x10086264
NavMap::~NavMap() {
}

// TODO
// FUNCTION: JMAN10 0x100862bc
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
		pVisited[i].pos = pScene[i].m_data.pos;
	}

	GlobalUnlock(visits_);
	GlobalUnlock(scenes);

	return TRUE;
}
