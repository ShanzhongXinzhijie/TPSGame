#include "stdafx.h"
#include "HandGun.h"
#include "CPlayer.h"
#include "BatBullet.h"

HandGun::HandGun(CPlayer* player, GameObj::CSkinModelRender* playerModel, unsigned int shotAnim,
				 unsigned int reloadAnim)
	:Weapon(player, playerModel), shotAnimationNum(shotAnim), reloadAnimationNum(reloadAnim){

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
	playerModel->GetAnimCon().Play(shotAnimationNum, CPlayer::animInterpolateSec);

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
	se->SetPos(pos);//âπÇÃà íu
	se->SetDistance(500.0f);//âπÇ™ï∑Ç±Ç¶ÇÈîÕàÕ
	se->Play(true); //ëÊàÍà¯êîÇtrue

	//íeÇå¸Ç´(ï˚å¸)Ç∆èÍèäÇéwíËÇµÇƒî≠éÀ
	CVector3 vec = action.getLookVec() * 5000;
	if (player->isFlying()) {
		vec += player->getVelocity();
		pos += action.getLookVec() * 100;
	} else {
		pos.y += 60;
	}
	new BatBullet(player, pos, vec);

	vec = action.getLookVec();

	//íeÇÃå¸Ç´(âÒì])Çê›íË
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
		se->SetPos(player->getPosition());//âπÇÃà íu
		se->SetDistance(500.0f);//âπÇ™ï∑Ç±Ç¶ÇÈîÕàÕ
		se->Play(true); //ëÊàÍà¯êîÇtrue
		playerModel->GetAnimCon().Play(reloadAnimationNum, CPlayer::animInterpolateSec);
	}
}

void HandGun::drawBulletCount(CFont & font) {
	wchar_t countbullet[10];
	swprintf_s(countbullet, L"écíe%d/%dî≠", bulletCount, maxBulletCount);
	font.Draw(countbullet, { 0.7f, 0.9f });
}