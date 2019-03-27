#include "stdafx.h"
#include "CPlayer.h"
#include "GameWaiter.h"
#include "Wing.h"
#include "HandGun.h"
#include "Rifle.h"
#include "Lazer.h"
#include "Bullet.h"
#include "CollisionMaskConst.h"

CPlayer::CPlayer(int pNum,Team* tem, const CVector3& position)
	: playerNum(pNum), team(tem), miniHpbar(maxHp){
	team->addPlayer(this);//チームに自分を設定
	mover.SetPosition(position);
#ifndef SpritScreen
	if (playerNum != GetPhoton()->GetLocalPlayerNumber()) {
		mover.SetIsLocalUser(false);
	}
#endif	
}

CPlayer::~CPlayer() {
	delete wing;
	for (Weapon* wp : weapon) {
		delete wp;
	}
}

bool CPlayer::Start() {
	m_animationClips[anim_run].Load(L"Resource/animData/PlayerRun.tka", true);
	m_animationClips[anim_walk].Load(L"Resource/animData/PlayerWalk.tka", true);
	m_animationClips[anim_fly].Load(L"Resource/animData/PlayerFly.tka", true);
	m_animationClips[anim_idle].Load(L"Resource/animData/PlayerIdle.tka", true);
	m_animationClips[anim_shot].Load(L"Resource/animData/PlayerShot.tka", true);
	m_animationClips[anim_reload].Load(L"Resource/animData/PlayerReload.tka", false);
	m_animationClips[anim_jump].Load(L"Resource/animData/PlayerJump.tka", true);
	m_animationClips[anim_fall].Load(L"Resource/animData/PlayerFall.tka", true);

	m_model.Init(L"Resource/modelData/PlayerVanp.cmo", m_animationClips, anim_num);

	m_model.GetSkinModel().FindMaterialSetting([&](MaterialSetting* mat) {
		mat->SetAlbedoScale(team->getColor());
	});

	//CharacterControllerの初期化
	mover.Init(30.0f, 40.0f, getPosition());

	//コライダーとキャラコンの基点がずれているのでその修正。
	//キャラコンは一番下が基点で、コライダーは中心が基点。
	//カプセルコライダーのHeightは円柱部分の高さで上下の半球部分は含んでいないため、
	//中心の高さはHeight÷2＋半径で求める
	CVector3 pos = getPosition();
	pos.y += mover.GetCollider()->GetHeight() / 2 + mover.GetCollider()->GetRadius();
	m_collision.CreateCapsule(pos, CQuaternion::Identity(), 30.0f, 40.0f);

	m_collision.SetName(L"CPlayer");
	m_collision.SetClass(this);//CCollisionには自クラスのポインタを設定できる

	//マスクとグループの設定
	m_collision.All_Off_Group();
	m_collision.On_OneGroup(CollisionMaskConst::encolKurai);
	m_collision.All_Off_Mask();
	m_collision.On_OneMask(CollisionMaskConst::encolAttack);
	//これは喰らい判定
	m_collision.SetIsHurtCollision(true);

	//武器配列に武器を格納する
	weapon[HUND_GUN] = new HandGun(this, &m_model, anim_shot, anim_reload);
	weapon[RIFLE] = new Rifle(this, &m_model, anim_shot, anim_reload);
	weapon[LAZER] = new Lazer(this, &m_model, anim_shot, anim_reload);

	//手に持っている武器にハンドガンを設定してアクティブにする。
	activeWeapon = HUND_GUN;
	weapon[HUND_GUN]->Activate();

	//初期化されているフラグ
	m_Init = true;
	return true;
};

void CPlayer::Update() {
	//ゲームがウェイト状態なら終わる
	if (GameWaiter::GetIsWait()) { return; }

	//HP0ならなにもできない
	if (m_hp != 0) {
		if (!weapon[activeWeapon]->isReloading()) {//リロード中は行動できない
			Move();
			if (!isFlying()) {//飛行中は銃を撃てない
				Shot();
			}
			Reload();
			changeWeapon(action.isWeaponLeft(), action.isWeaponRight());
		}
		//当たり判定の更新
		mover.Update();
		//モデルの更新
		m_model.SetPos(getPosition());
		m_model.SetRot(mover.getRotation());
		//ミニHPバーの位置の更新
		miniHpbar.setPos(getPosition());
		CVector3 pos = getPosition();

		//ダメージ用当たり判定の位置の更新。コライダーの中心を設定する。
		pos.y += mover.GetCollider()->GetHeight()/2 + mover.GetCollider()->GetRadius();
		m_collision.SetPosition(pos);

		//場外に落ちた場合、ダメージを与えてホームポジションにテレポートさせる
		if (getPosition().y < -100.0f) {
			damage({ 0,0,0 }, maxHp / 5);
			mover.SetPosition(team->getHome());
		}
	} else {
		//一定時間たったら蘇生する
		deathCool -= GetDeltaTimeSec();
		if (deathCool <= 0.0f) {
			Revive();
		}
	}
}

void CPlayer::sendAction(const ActionSender& actionPal) {
	action = actionPal;
}

bool CPlayer::damage(const CVector3 & dir, unsigned int damage, const Team* atkTeam, const CPlayer*) {
	//ダメージを与えられるのは生きている場合のみ
	if (m_hp != 0) {
		if (atkTeam == team) {//自分のチームの攻撃はダメージは受けないが衝撃は受ける
			damage = 0;
		}
		CVector3 pos = getPosition();
		pos.y += 60.0f;
		//音とエフェクト
		new GameObj::Suicider::CEffekseer(L"Resource/effect/damage.efk", 1.0f, pos);
		playSE(L"Resource/sound/SE_damage.wav", getPosition());
		//衝撃を与える
		mover.addVelocity(dir);

		//ダメージを与える。0以下になるなら死亡する。
		if (m_hp > damage) {
			m_hp -= damage;
		} else {
			m_hp = 0;
			Death();
		}
		//ダメージを受けたならミニHPバーを表示する。
		if (damage != 0) {
			miniHpbar.display(m_hp);
		}
		return true;
	}
	//死亡中だったなら当たった事実自体無くなる。(弾などは貫通する)
	return false;
}

//死亡処理
void CPlayer::Death() {
	m_hp = 0;
	deathCool = constDeathCool;//蘇生時間の設定
	m_model.SetIsDraw(false);
	weapon[activeWeapon]->Inactivate();
	//あたり判定を消す
	m_collision.SetEnable(false);
	mover.RemoveRigidBoby();
}
//蘇生処理
void CPlayer::Revive() {
	m_hp = maxHp;
	m_model.SetIsDraw(true);
	weapon[activeWeapon]->Activate();
	//あたり判定の再生
	m_collision.SetEnable(true);
	mover.AddRigidBoby();
}

void CPlayer::Move() {

	const CVector2& movement = action.getMovement();

	//壁ジャンプ
	bool isWalljump = false;
	//空中に壁に当たりながらジャンプ
	if (mover.IsContactWall() && !mover.IsOnGround() && action.isJump()) {
		playSE(L"Resource/sound/SE_jump.wav", getPosition()); m_isSendJumpSE = true;
		mover.walljump(jumpPower, movement);
		isWalljump = true;
	}
	//飛行で壁に突っ込む
	if (mover.isHitWall()) {
		playSE(L"Resource/sound/SE_jump.wav", getPosition());
		isWalljump = true;
	}

	//ジャンプボタンが押された場合
	if (action.isJump() && !isWalljump) {
		//接地しているなら普通にジャンプ
		if (mover.IsOnGround()) {
			playSE(L"Resource/sound/SE_jump.wav", getPosition()); m_isSendJumpSE = true;
			mover.jump(jumpPower);
		}
		else{//空中にいるなら飛行状態を切り替える
			if (mover.isFlying()) {
				mover.flyStop();
			} else {
				mover.fly(true, action.getLookVec(), {0, 0}, flyPower);
			}
		}
	}

	//飛行
	if (mover.isFlying()) {
		if (wing == nullptr) {//翼をはやす
			wing = new Wing(mover);
		}
		mover.fly(false, action.getLookVec(), movement);
		mover.turn(action.getLookVec().x, action.getLookVec().z);
		m_model.GetAnimCon().Play(anim_fly, animInterpolateSec);//飛行アニメーション
		//飛行中は通常の移動はしないのでここで終了する。
		return;
	} else {
		//飛行中でないのに翼が生えているなら、消す
		if (wing != nullptr) {
			wing->closeDelete();
			wing = nullptr;
		}
	}

	float speed = moveSpeed;
	bool dash = false;

	//ダッシュ中は移動速度上昇、ただし射撃中は無効
	if(action.isDash() && !action.isShot()) {
		speed = moveSpeed * dashMul;
		dash = true;
	}

	//徒歩移動
	mover.move(movement*speed);

	//接地中アニメーション
	if (mover.IsOnGround()) {
		//徒歩移動アニメーション
		if (movement.x != 0 || movement.y != 0) {
			if (dash) {
				m_model.GetAnimCon().Play(anim_run, animInterpolateSec);//ダッシュ
			} else {
				m_model.GetAnimCon().Play(anim_walk, animInterpolateSec);//歩き
			}
		} else {
			//何もしてないときのアニメーション
			m_model.GetAnimCon().Play(anim_idle, animInterpolateSec);
		}
	} else {
		//空中アニメーション
		if (mover.getVelocity().y > 0) {
			m_model.GetAnimCon().Play(anim_jump, animInterpolateSec);//上昇中はジャンプアニメーション
		} else {
			m_model.GetAnimCon().Play(anim_fall, animInterpolateSec);//下降アニメーション
		}
	}

	//回転
	if (action.isShot()) {
		mover.turn(action.getLookVec().x, action.getLookVec().z);//射撃中は撃つ方向を向く
	} else {
		mover.turn(movement.x, movement.y);//そうでなければ移動方向を向く
	}
}

void CPlayer::Shot() {
	if (action.isShot()) {
		weapon[activeWeapon]->shot();
	}
}

void CPlayer::Reload() {
	if (action.isReload()) {
		weapon[activeWeapon]->reload();
	}
}

void CPlayer::changeWeapon(bool left, bool right) {
	if (left == right) { return; }

	int nextWeapon = (int)activeWeapon;
	if (left) { nextWeapon -= 1;}
	if (right) { nextWeapon += 1;}
	if (nextWeapon < 0) { nextWeapon = WEAPON_NUM - 1;}//0未満は末尾に移動
	if (nextWeapon >= WEAPON_NUM) { nextWeapon = 0;}//要素数を超えたら0に移動
	
	weapon[activeWeapon]->Inactivate();//前の武器を非アクティブ化
	weapon[nextWeapon]->Activate();//変更された武器をアクティブ化
	activeWeapon = (unsigned int)nextWeapon;//手に持っている武器を変更
}
void CPlayer::changeWeapon(unsigned char useWeapon) {
	if (GetIsDead()) { return; }

	short nextWeapon = useWeapon;
	if (nextWeapon < 0) { nextWeapon = WEAPON_NUM - 1; }
	if (nextWeapon >= WEAPON_NUM) { nextWeapon = 0; }

	weapon[activeWeapon]->Inactivate();
	weapon[nextWeapon]->Activate();
	activeWeapon = nextWeapon;
}

void CPlayer::PlayJumpSE() {
	playSE(L"Resource/sound/SE_jump.wav", getPosition());
}