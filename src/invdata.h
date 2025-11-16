#ifndef INVDATA_H
#define INVDATA_H

#include <afxwin.h>

// VTABLE: JMAN10 0x102845d8
class InvData : public CObject {
public:
	InvData();
	~InvData();

	HGLOBAL _glInventory;

	int Count();
};

typedef struct player_item_s {
	int id;
	int item2;
	int item3;
} player_item_t;

// VTABLE: JMAN10 0x102845b0
class PlayerInventory : public CObject {
public:
	PlayerInventory();
	~PlayerInventory();

	UINT _total_items;   // 0x04
	int _player_n_items; // 0x06
	HGLOBAL _glItems;    // 0x08

	BOOL Reset(UINT);
	BOOL HasItem(int);
};

// VTABLE: JMAN10 0x102845c4
class Inventory : public CObject {
public:
	Inventory();

	InvData _invdata;              // 0x04
	PlayerInventory _player_items; // 0x0a

	BOOL PlayerHasItem(int);
};

#endif
