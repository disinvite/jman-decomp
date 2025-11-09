#include "invdata.h"

#include <afxwin.h>

// FUNCTION: JMAN10 0x102835ec
InvData::InvData() {
	_glInventory = 0;

	HGLOBAL gl = LoadResource(afxCurrentResourceHandle, FindResource(afxCurrentResourceHandle, "BDATA", "INVDATA"));
	int *res_inv = (int *)LockResource(gl);

	UINT n_items = *res_inv;
	res_inv++;

	_glInventory = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE, 618 * n_items);
	int *invdata = (int *)GlobalLock(_glInventory);
	memcpy(invdata, res_inv, 618 * n_items);
	GlobalUnlock(_glInventory);

	GlobalUnlock(gl);
	FreeResource(gl);
}

// FUNCTION: JMAN10 0x102836b4
InvData::~InvData() {
	if (_glInventory != 0) {
		GlobalFree(_glInventory);
	}
	_glInventory = 0;
}

// FUNCTION: JMAN10 0x10283840
int InvData::Count() {
	if (_glInventory == 0) {
		return -1;
	}

	return GlobalSize(_glInventory) / 618;
}

// STUB: JMAN10 0x10283906
PlayerInventory::PlayerInventory() {
}

// STUB: JMAN10 0x1028392e
PlayerInventory::~PlayerInventory() {
}

// FUNCTION: JMAN10 0x10283976
BOOL PlayerInventory::Reset(UINT n_items) {
	_player_n_items = 0;
	_total_items = n_items;

	if (_glItems != 0) {
		GlobalFree(_glItems);
	}
	_glItems = 0;
	_glItems = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE, n_items * sizeof(player_item_t));

	return TRUE;
}

// FUNCTION: JMAN10 0x10283a9c
BOOL PlayerInventory::HasItem(int id) {
	if (_glItems == 0) {
		return FALSE;
	}

	if (_player_n_items <= 0) {
		return FALSE;
	}

	player_item_t *items = (player_item_t *)GlobalLock(_glItems);
	if (!items) {
		return FALSE;
	}

	BOOL found = FALSE;
	for (int i = 0; i < _player_n_items; i++) {
		if (found) {
			break;
		}

		if (items->id == id) {
			found = TRUE;
		} else {
			items++;
		}
	}

	GlobalUnlock(_glItems);
	return found;
}

// STUB: JMAN10 0x10283f98
Inventory::Inventory() {
	_player_items.Reset(_invdata.Count());
}

// FUNCTION: JMAN10 0x10284122
BOOL Inventory::PlayerHasItem(int id) {
	return _player_items.HasItem(id);
}
