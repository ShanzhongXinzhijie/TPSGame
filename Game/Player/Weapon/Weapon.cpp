#include "stdafx.h"
#include "Weapon.h"

Weapon::Weapon(CPlayer* player,
			   GameObj::CSkinModelRender* playerModel,
			   const wchar_t* weaponModelPath,
			   float zoomScale)
	: player(player), playerModel(playerModel), zoomScale(zoomScale){
	weaponModel.Init(weaponModelPath);
}

Weapon::~Weapon() {
}

void Weapon::Update() {
	Bone* arm = playerModel->FindBone(L"Bone022");
	CQuaternion rot = arm->GetRotation();
	rot.Multiply(CQuaternion::GetRotationDeg(CVector3::AxisX(), 90.0f));
	weaponModel.SetRot(rot);
	CVector3 armVec = { -25.0f, 0.0f, -5.0f };
	rot.Multiply(armVec);
	weaponModel.SetPos(arm->GetPosition() + armVec);
}