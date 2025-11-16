#ifndef NAVDATA_H
#define NAVDATA_H

#include <afxwin.h>

// enum Direction { kEast = 0, kNorth, kWest, kSouth };

typedef struct pos_s {
	int scene;
	int area;
	int dir;
} pos_t;

typedef struct visited_s {
	pos_t pos;
	BOOL visited;
} visited_t;

typedef struct scene_s {
	pos_t pos;
	int class_id;
	pos_t left;
	pos_t right;
	pos_t forward;
	pos_t backward;
	int vid_no;
	int frame_no;
	int other[3];
} scene_t;

class NavData : public CObject {
public:
	NavData();
	~NavData();

	scene_t *FUN_1008_5ffc(scene_t *ptr, pos_t pos);
	scene_t *FUN_1008_60c6(scene_t *ptr, int area, int scene, int dir);
	HGLOBAL GetScenesHandle();
	UINT GetScenesCount();

	// UINT or int?
	UINT _n_scenes;      // 0x04
	HGLOBAL _glScenes;   // 0x06
	HGLOBAL _glAvifiles; // 0x08
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
