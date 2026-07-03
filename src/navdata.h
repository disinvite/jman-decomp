#ifndef NAVDATA_H
#define NAVDATA_H

#include <afxwin.h>

// enum Direction { kEast = 0, kNorth, kWest, kSouth };

// SIZE 0x06
typedef struct pos_s {
	int area;  // 0x00
	int scene; // 0x02
	int dir;   // 0x04
} pos_t;

typedef struct visited_s {
	pos_t pos;
	BOOL visited;
} visited_t;

// SIZE 0x08
typedef struct nav_combined_s {
	pos_t pos;    // 0x00
	int class_id; // 0x06
} nav_combined_t;

// SIZE 0x2a
typedef struct scene_s {
	nav_combined_t m_data; // 0x00
	pos_t left;            // 0x08
	pos_t right;           // 0x0e
	pos_t forward;         // 0x14
	pos_t backward;        // 0x1a
	int vid_no;            // 0x20
	int frame_no;          // 0x22
	int unk0x24;           // 0x24
	int unk0x26;           // 0x26
	int unk0x28;           // 0x28

	// 0x26 appears to be the total number of frames "connected" to this scene.
} scene_t;

// VTABLE: JMAN10 0x10086a24
class NavData : public CObject {
public:
	NavData();
	~NavData();

	scene_t FUN_1008_5ffc(pos_t pos);
	scene_t FUN_1008_60c6(int area, int scene, int dir);
	HGLOBAL GetScenesHandle();
	UINT GetScenesCount();

	// UINT or int?
	UINT m_sceneCount;    // 0x04
	HGLOBAL m_glScenes;   // 0x06
	HGLOBAL m_glAvifiles; // 0x08
};

class NavMap : public CObject {
public:
	NavMap();
	~NavMap();

	UINT count_;
	HGLOBAL visits_;
	HGLOBAL h8_;

	BOOL NavMap::Setup(HGLOBAL scenes, UINT n_scenes);
};

#endif
