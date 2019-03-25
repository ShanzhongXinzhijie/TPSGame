#include "stdafx.h"
#include "SE_Util.h"

using namespace DemolisherWeapon;
using CSE = GameObj::Suicider::CSE;

CSE* playSE(const wchar_t* path, CVector3 pos, float length, bool loop) {
	CSE* se = NewGO<CSE>(path);
	se->SetPos(pos);//���̈ʒu
	se->SetDistance(length);//������������͈�
	se->Play(true, loop); //��������true
	return se;
}

CSE* playSE(const wchar_t* path, bool loop) {
	using CSE = GameObj::Suicider::CSE;
	CSE* se = NewGO<CSE>(path);
	se->Play(false, loop); //��������true
	return se;
}