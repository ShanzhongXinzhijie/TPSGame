#include "stdafx.h"
#include "CPlayer.h"
#include "GameWaiter.h"
#include "Wing.h"
#include "HandGun.h"
#include "Rifle.h"
#include "Bullet.h"

CPlayer::CPlayer(int pNum,Team* tem, const CVector3& position)
	: playerNum(pNum), team(tem), miniHpbar(maxHp){
	team->addPlayer(this);
	mover.SetPosition(position);
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

	mover.Init(30.0f, 40.0f, getPosition());
	CVector3 pos = getPosition();
	pos.y += mover.GetCollider()->GetHeight() / 2 + mover.GetCollider()->GetRadius();
	m_collision.CreateCapsule(pos, CQuaternion::Identity(), 30.0f, 40.0f);
	m_collision.SetName(L"CPlayer");
	m_collision.SetClass(this);

	weapon[HUND_GUN] = new HandGun(this, &m_model, anim_shot, anim_reload);
	weapon[RIFLE] = new Rifle(this, &m_model, anim_shot, anim_reload);
	activeWeapon = HUND_GUN;
	weapon[HUND_GUN]->Activate();

	m_Init = true;
	return true;
};

void CPlayer::Update() {
	//�Q�[�����E�F�C�g��ԂȂ�I���
	if (GameWaiter::GetIsWait()) { return; }

	if (m_hp != 0) {
		if (!weapon[activeWeapon]->isReloading()) {
			Move();
			Shot();
			Reload();
			changeWeapon(action.isWeaponLeft(), action.isWeaponRight());
		}
		mover.Update();
		m_model.SetPos(getPosition());
		m_model.SetRot(mover.getRotation());
		miniHpbar.setPos(getPosition());
		CVector3 pos = getPosition();
		pos.y += mover.GetCollider()->GetHeight()/2 + mover.GetCollider()->GetRadius();
		m_collision.SetPosition(pos);

		if (getPosition().y < -100.0f) {
			Hit({ 0,0,0 }, maxHp / 5);
			mover.SetPosition(team->getHome());
		}
	} else {
		deathCool -= GetDeltaTimeSec();
		if (deathCool <= 0.0f) {
			Revive();
		}
	}
}

void CPlayer::sendAction(const ActionSender& actionPal) {
	action = actionPal;
}

bool CPlayer::BatHit(Bullet* bullet) {
	CPlayer* shooter = bullet->getShooter();
	if (shooter == this) {
		return false;
	}
	unsigned int damage = 0;
	if (shooter->team != this->team) {
		damage = bullet->getDamage();
	}
	Hit(bullet->getHitVec(), damage);
	return true;
}

void CPlayer::Hit(const CVector3 & dir, unsigned int damage) {
	if (m_hp != 0) {
		CVector3&& pos = getPosition();
		pos.y += 60.0f;
		new GameObj::Suicider::CEffekseer(L"Resource/effect/damage.efk", 1.0f, pos);
		playSE(L"Resource/sound/SE_damage.wav");
		mover.addVelocity(dir);
		if (m_hp > damage) {
			m_hp -= damage;
		} else {
			m_hp = 0;
			Death();
		}
		miniHpbar.display(m_hp);
	}
}

//���S����
void CPlayer::Death() {
	m_hp = 0;
	deathCool = constDeathCool;
	m_model.SetIsDraw(false);
	weapon[activeWeapon]->Inactivate();
}
//�h������
void CPlayer::Revive() {
	m_hp = maxHp;
	m_model.SetIsDraw(true);
	weapon[activeWeapon]->Activate();
}

void CPlayer::Move() {

	const CVector2& movement = action.getMovement();

	//�ǃW�����v
	bool isWalljump = false;
	//�󒆂ɕǂɓ�����Ȃ���W�����v
	if (mover.IsContactWall() && !mover.IsOnGround() && action.isJump()) {
		playSE(L"Resource/sound/SE_jump.wav");
		mover.walljump(jumpPower, movement);
		isWalljump = true;
	}
	//��s�ŕǂɓ˂�����
	if (mover.isHitWall()) {
		mover.SetFlyTimer(max(0.0f, mover.getFlyTimer() - mover.getFlyTimerMax()*0.1f));//��s�\���Ԃ�����
		playSE(L"Resource/sound/SE_jump.wav");
		isWalljump = true;
	}

	//�W�����v�Ɣ�s
	if (action.isJump() && !isWalljump) {
		if (mover.IsOnGround()) {
			playSE(L"Resource/sound/SE_jump.wav");
			mover.jump(jumpPower);
		}
		else{
			if (mover.isFlying()) {
				mover.flyStop();
			} else {
				mover.fly(true, action.getLookVec(), {0, 0}, flyPower);
			}
		}
	}

	//��s
	if (mover.isFlying()) {
		if (wing == nullptr) {
			wing = new Wing(mover);
		}
		mover.fly(false, action.getLookVec(), movement);
		mover.turn(action.getLookVec().x, action.getLookVec().z);
		m_model.GetAnimCon().Play(anim_fly, animInterpolateSec);
		return;
	} else {
		if (wing != nullptr) {
			wing->closeDelete();
			wing = nullptr;
		}
	}

	//�ړ�
	float speed = moveSpeed;
	bool dash = false;

	//�ړ����x
	if(action.isDash() && !action.isShot()) {
		speed = moveSpeed * dashMul;
		dash = true;
	}

	mover.move(movement*speed);

	//�A�j���[�V����
	if (mover.IsOnGround()) {
		if (movement.x != 0 || movement.y != 0) {
			if (dash) {
				m_model.GetAnimCon().Play(anim_run, animInterpolateSec);
			} else {
				m_model.GetAnimCon().Play(anim_walk, animInterpolateSec);
			}
		} else {
			m_model.GetAnimCon().Play(anim_idle, animInterpolateSec);
		}
	} else {
		if (mover.getVelocity().y > 0) {
			m_model.GetAnimCon().Play(anim_jump, animInterpolateSec);
		} else {
			m_model.GetAnimCon().Play(anim_fall, animInterpolateSec);
		}
	}

	//��]
	if (action.isShot()) {
		mover.turn(action.getLookVec().x, action.getLookVec().z);
	} else {
		mover.turn(movement.x, movement.y);
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
	short nextWeapon = activeWeapon;
	if (left) { nextWeapon -= 1;}
	if (right) { nextWeapon += 1;}
	if (nextWeapon < 0) { nextWeapon = WEAPON_NUM - 1;}
	if (nextWeapon >= WEAPON_NUM) { nextWeapon = 0;}
	
	weapon[activeWeapon]->Inactivate();
	weapon[nextWeapon]->Activate();
	activeWeapon = nextWeapon;
}
void CPlayer::changeWeapon(unsigned char useWeapon) {
	short nextWeapon = useWeapon;
	if (nextWeapon < 0) { nextWeapon = WEAPON_NUM - 1; }
	if (nextWeapon >= WEAPON_NUM) { nextWeapon = 0; }

	weapon[activeWeapon]->Inactivate();
	weapon[nextWeapon]->Activate();
	activeWeapon = nextWeapon;
}


void CPlayer::playSE(const wchar_t* path) {
	SuicideObj::CSE* se = NewGO<SuicideObj::CSE>(path);
	se->SetPos(mover.GetPosition());//���̈ʒu
	se->SetDistance(500.0f);//������������͈�
	se->Play(true); //��������true
}
