#include "stdafx.h"
#include "HandGun.h"
#include "CPlayer.h"
#include "BatBullet.h"

HandGun::HandGun(CPlayer* player, GameObj::CSkinModelRender* playerModel, unsigned int shotAnim,
				 unsigned int reloadAnim)
	:Weapon(player, playerModel, L"Resource/modelData/HundGun.cmo"),
	shotAnimNum(shotAnim),
	reloadAnimNum(reloadAnim){

	playerModel->GetAnimCon().GetAnimation(0).AddAnimationEventListener(
		[&](const wchar_t* clipName, const wchar_t* eventName) {
		if (std::wcscmp(eventName, L"Reload") == 0) {
			reloading = false;
			bulletCount = maxBulletCount;
		}
	});
}


HandGun::~HandGun() {
}

void HandGun::PreUpdate() {
	if (0 < shotCool) {
		shotCool -= GetDeltaTimeSec();
	}
}

void HandGun::shot() {
	playerModel->GetAnimCon().Play(shotAnimNum, CPlayer::animInterpolateSec);

	if (shotCool <= 0) {
		shotCool = constShotCool;
	} else {
		return;
	}

	if (bulletCount == 0) {
		return;
	}

	const ActionSender& action = player->GetActionSender();
	CVector3 pos = player->getPosition();
	SuicideObj::CSE* se = NewGO<SuicideObj::CSE>(L"Resource/sound/SE_shot.wav");
	se->SetPos(pos);//音の位置
	se->SetDistance(500.0f);//音が聞こえる範囲
	se->Play(true); //第一引数をtrue

	//弾を向き(方向)と場所を指定して発射
	CVector3 vec = action.getLookVec() * 5000;
	if (player->isFlying()) {
		vec += player->getVelocity();
		pos += action.getLookVec() * 100;
	} else {
		pos.y += 60;
	}
	new BatBullet(player, pos, vec);

	vec = action.getLookVec();

	//弾の向き(回転)を設定
	CQuaternion rot;
	rot.SetRotation(CVector3::AxisY(), atan2f(vec.x, vec.z));
	float xz = sqrt(vec.x*vec.x + vec.z*vec.z);
	rot.Multiply(CQuaternion::GetRotation(CVector3::AxisX(), atan2f(-vec.y, xz)));

	pos += vec * 50;
	new GameObj::Suicider::CEffekseer(L"Resource/effect/shot.efk", 1.0f, pos, rot);
	bulletCount--;
}

void HandGun::reload() {
	if (bulletCount < maxBulletCount) {
		reloading = true;
		new GameObj::Suicider::CEffekseer(L"Resource/effect/reload.efk", 1.0f, player->getPosition());
		SuicideObj::CSE* se = NewGO<SuicideObj::CSE>(L"Resource/sound/SE_reload.wav");
		se->SetPos(player->getPosition());//音の位置
		se->SetDistance(500.0f);//音が聞こえる範囲
		se->Play(true); //第一引数をtrue
		playerModel->GetAnimCon().Play(reloadAnimNum, CPlayer::animInterpolateSec);
	}
}

void HandGun::drawBulletCount(CFont & font) {
	wchar_t countbullet[10];
	swprintf_s(countbullet, L"残弾%d/%d発", bulletCount, maxBulletCount);
	font.Draw(countbullet, { 0.7f, 0.9f });
}