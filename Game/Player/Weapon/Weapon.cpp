#include "stdafx.h"
#include "Weapon.h"
#include "CPlayer.h"
#include "Bullet.h"

Weapon::Weapon(CPlayer* player,GameObj::CSkinModelRender* playerModel,const WeaponInfo& info)
	: player(player), playerModel(playerModel),
	shotAnimNum(info.shotAnimNum), reloadAnimNum(info.reloadAnimNum),
	c_ShotCool(info.shotCool), maxBullet(info.maxBullet), zoomScale(info.zoomScale){

	weaponModel.Init(info.modelPath);
	weaponSprite.Init(info.spritePath);
	Inactivate();

	playerModel->GetAnimCon().GetAnimation(0).AddAnimationEventListener(
		[&](const wchar_t* clipName, const wchar_t* eventName) {
		if (isActive && std::wcscmp(eventName, L"Reload") == 0) {
			reloading = false;
			bulletCount = maxBullet;
		}
	});
}

Weapon::~Weapon() {
}

void Weapon::shot() {
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
	SuicideObj::CSE* se = NewGO<SuicideObj::CSE>(L"Resource/sound/SE_shot.wav");
	se->SetPos(pos);//âπÇÃà íu
	se->SetDistance(500.0f);//âπÇ™ï∑Ç±Ç¶ÇÈîÕàÕ
	se->Play(true); //ëÊàÍà¯êîÇtrue

	//íeÇå¸Ç´(ï˚å¸)Ç∆èÍèäÇéwíËÇµÇƒî≠éÀ
	CVector3 vec = action.getLookVec();
	if (player->isFlying()) {
		pos += action.getLookVec() * 100;
	} else {
		pos += action.getLookVec() * 50;
		pos.y += 60;
	}

	Bullet* bullet = createBullet(player, pos, vec);

	if (player->isFlying()) {
		bullet->addVelocity(player->getVelocity());
	}

	//íeÇÃå¸Ç´(âÒì])Çê›íË
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
		SuicideObj::CSE* se = NewGO<SuicideObj::CSE>(L"Resource/sound/SE_reload.wav");
		se->SetPos(player->getPosition());//âπÇÃà íu
		se->SetDistance(500.0f);//âπÇ™ï∑Ç±Ç¶ÇÈîÕàÕ
		se->Play(true); //ëÊàÍà¯êîÇtrue
		playerModel->GetAnimCon().Play(reloadAnimNum, CPlayer::animInterpolateSec);
	}
}

void Weapon::PreUpdate() {
	if (0 < shotCool) {
		shotCool -= GetDeltaTimeSec();
	}
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

void Weapon::drawBulletCount(CFont & font) {
	wchar_t countbullet[20];
	swprintf_s(countbullet, L"Bullet: %d / %d", bulletCount, maxBullet);
	font.Draw(countbullet, { 0.723f, 0.883f }, { 0,0,0,1 }, { 1.5f, 1.5f });
	font.Draw(countbullet, { 0.72f, 0.88f }, { 1,1,1,1 }, {1.5f, 1.5f});
}