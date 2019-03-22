#include "stdafx.h"
#include "NormalBullet.h"
#include "CPlayer.h"
#include "Citizen.h"

using SuicideObj::CCollisionObj;

NormalBullet::NormalBullet(CPlayer* player,CVector3 position, CVector3 direction,
						   const wchar_t* modelPath, unsigned int damage, const wchar_t* efkPath)
	: Bullet(player, position, direction, modelPath, damage){
	if(efkPath){
		CQuaternion rot; 
		rot.SetRotation(CVector3::AxisY(), atan2f(-direction.x, -direction.z));
		float xz = sqrt(direction.x*direction.x + direction.z*direction.z);
		rot.Multiply( CQuaternion::GetRotation(CVector3::AxisX(), atan2f(direction.y, xz)) );
		efk = new SuicideObj::CEffekseer(efkPath, 1.0f, position, rot);
		efk->SetIsSuicide(false);
	}
}


NormalBullet::~NormalBullet() {
	if (efk) {
		new EfkDeleter(efk);
	}
}

void NormalBullet::BulletUpdate() {
	if (efk && efk->IsPlay()) {
		efk->SetPos(m_pos);
	} else {
		delete efk;
		efk = nullptr;
	}
}
