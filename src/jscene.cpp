#include "jscene.h"

#include "navdata.h"

// FUNCTION: JMAN10 0x1008907e
JScene::JScene() {
}

// FUNCTION: JMAN10 0x1008916c
JScene::~JScene() {
	// DeleteObject(m_unk0x004)
}

BOOL JScene::OnTimer() {
	return TRUE;
}

// FUNCTION: JMAN10 0x100891c2
BOOL JScene::OnEnter() {
	return TRUE;
}

// FUNCTION: JMAN10 0x100891c8
BOOL JScene::OnLeave(pos_t nextScene) {
	return TRUE;
}

BOOL JScene::OnMouseUp(UINT nFlags, CPoint point) {
	return TRUE;
}

BOOL JScene::OnMouseDown(UINT nFlags, CPoint point) {
	return TRUE;
}

BOOL JScene::OnMouseMove(UINT nFlags, CPoint point) {
	return TRUE;
}

void JScene::Timer1() {
}

void JScene::Timer2() {
}
