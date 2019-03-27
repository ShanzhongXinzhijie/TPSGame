#include "stdafx.h"
#include "Weapon.h"
#include "CPlayer.h"
#include "Bullet.h"
#include "SE_Util.h"

Weapon::Weapon(CPlayer* player,GameObj::CSkinModelRender* playerModel,const WeaponInfo& info)
	: player(player), playerModel(playerModel), shotSE(info.shotSEPath),
	shotAnimNum(info.shotAnimNum), reloadAnimNum(info.reloadAnimNum),
	c_ShotCool(info.shotCool), maxBullet(info.maxBullet){

	weaponModel.Init(info.modelPath);
	weaponSprite.Init(info.spritePath);
	Inactivate();

	playerModel->GetAnimCon().GetAnimation(0).AddAnimationEventListener(
		[&](const wchar_t* clipName, const wchar_t* eventName) {
		if (isActive && std::wcscmp(eventName, L"Reload") == 0) {
			reloading = false;
			bulletCount = maxBullet;
			playSE(L"Resource/sound/SE_reload2.wav", this->player->getPosition());
		}
	});
}

Weapon::~Weapon() {
}

void Weapon::shot() {
	PreShot();

	playerModel->GetAnimCon().Play(shotAnimNum, CPlayer::animInterpolateSec);

	if (shotCool <= 0) {
		shotCool = c_ShotCool;
	} else {
		return;
	}

	if (bulletCount == 0) {
		return;
	}

	const ActionSender& action = player->GetActionSender();
	CVector3 pos = player->getPosition();
	playSE(shotSE, player->getPosition());

	//弾を向き(方向)と場所を指定して発射
	CVector3 vec = action.getLookVec();
	if (player->isFlying()) {
		pos += action.getLookVec() * 100;
	} else {
		pos += action.getLookVec() * 50;
		pos.y += 60;
	}

	createBullet(player, pos, vec);

	//エフェクトの向き(回転)を設定
	CQuaternion rot;
	rot.SetRotation(CVector3::AxisY(), atan2f(vec.x, vec.z));
	float xz = sqrt(vec.x*vec.x + vec.z*vec.z);
	rot.Multiply(CQuaternion::GetRotation(CVector3::AxisX(), atan2f(-vec.y, xz)));

	new GameObj::Suicider::CEffekseer(L"Resource/effect/shot.efk", 1.0f, pos, rot);
	bulletCount--;
}

void Weapon::reload() {
	if (bulletCount < maxBullet) {
		reloading = true;
		new GameObj::Suicider::CEffekseer(L"Resource/effect/reload.efk", 1.0f, player->getPosition());
		playSE(L"Resource/sound/SE_reload.wav", player->getPosition());
		playerModel->GetAnimCon().Play(reloadAnimNum, CPlayer::animInterpolateSec);
	}
}

void Weapon::PreUpdate() {
	if (0 < shotCool) {
		shotCool -= GetDeltaTimeSec();
	}
}

void Weapon::Update() {
	WeaponUpdate();

	Bone* arm = playerModel->FindBone(L"Bone022");
	CQuaternion rot = arm->GetRotation();
	rot.Multiply(CQuaternion::GetRotationDeg(CVector3::AxisX(), 90.0f));
	weaponModel.SetRot(rot);
	CVector3 armVec = { -25.0f, 0.0f, -5.0f };
	rot.Multiply(armVec);
	weaponModel.SetPos(arm->GetPosition() + armVec);
}

void Weapon::drawBulletCount(CFont & font) {
	wchar_t countbullet[20];
	swprintf_s(countbullet, L"Bullet: %3d / %3d", bulletCount, maxBullet);
	font.Draw(countbullet, { 0.693f, 0.893f }, { 0,0,0,1 }, { 1.5f, 1.5f });
	font.Draw(countbullet, { 0.69f, 0.89f }, { 1,1,1,1 }, {1.5f, 1.5f});
}