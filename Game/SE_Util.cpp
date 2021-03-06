#include "stdafx.h"
#include "SE_Util.h"

using namespace DemolisherWeapon;
using CSE = GameObj::Suicider::CSE;

CSE* playSE(const wchar_t* path, CVector3 pos, float length, bool loop) {
	CSE* se = NewGO<CSE>(path);
	se->SetPos(pos);//音の位置
	se->SetDistance(length);//音が聞こえる範囲
	se->Play(true, loop); //第一引数をtrue
	return se;
}

CSE* playSE(const wchar_t* path, bool loop) {
	using CSE = GameObj::Suicider::CSE;
	CSE* se = NewGO<CSE>(path);
	se->Play(false, loop); //第一引数をtrue
	return se;
}