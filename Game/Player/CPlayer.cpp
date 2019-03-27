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
	team->addPlayer(this);//�`�[���Ɏ�����ݒ�
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

	//CharacterController�̏�����
	mover.Init(30.0f, 40.0f, getPosition());

	//�R���C�_�[�ƃL�����R���̊�_������Ă���̂ł��̏C���B
	//�L�����R���͈�ԉ�����_�ŁA�R���C�_�[�͒��S����_�B
	//�J�v�Z���R���C�_�[��Height�͉~�������̍����ŏ㉺�̔��������͊܂�ł��Ȃ����߁A
	//���S�̍�����Height��2�{���a�ŋ��߂�
	CVector3 pos = getPosition();
	pos.y += mover.GetCollider()->GetHeight() / 2 + mover.GetCollider()->GetRadius();
	m_collision.CreateCapsule(pos, CQuaternion::Identity(), 30.0f, 40.0f);

	m_collision.SetName(L"CPlayer");
	m_collision.SetClass(this);//CCollision�ɂ͎��N���X�̃|�C���^��ݒ�ł���

	//�}�X�N�ƃO���[�v�̐ݒ�
	m_collision.All_Off_Group();
	m_collision.On_OneGroup(CollisionMaskConst::encolKurai);
	m_collision.All_Off_Mask();
	m_collision.On_OneMask(CollisionMaskConst::encolAttack);
	//����͋�炢����
	m_collision.SetIsHurtCollision(true);

	//����z��ɕ�����i�[����
	weapon[HUND_GUN] = new HandGun(this, &m_model, anim_shot, anim_reload);
	weapon[RIFLE] = new Rifle(this, &m_model, anim_shot, anim_reload);
	weapon[LAZER] = new Lazer(this, &m_model, anim_shot, anim_reload);

	//��Ɏ����Ă��镐��Ƀn���h�K����ݒ肵�ăA�N�e�B�u�ɂ���B
	activeWeapon = HUND_GUN;
	weapon[HUND_GUN]->Activate();

	//����������Ă���t���O
	m_Init = true;
	return true;
};

void CPlayer::Update() {
	//�Q�[�����E�F�C�g��ԂȂ�I���
	if (GameWaiter::GetIsWait()) { return; }

	//HP0�Ȃ�Ȃɂ��ł��Ȃ�
	if (m_hp != 0) {
		if (!weapon[activeWeapon]->isReloading()) {//�����[�h���͍s���ł��Ȃ�
			Move();
			if (!isFlying()) {//��s���͏e�����ĂȂ�
				Shot();
			}
			Reload();
			changeWeapon(action.isWeaponLeft(), action.isWeaponRight());
		}
		//�����蔻��̍X�V
		mover.Update();
		//���f���̍X�V
		m_model.SetPos(getPosition());
		m_model.SetRot(mover.getRotation());
		//�~�jHP�o�[�̈ʒu�̍X�V
		miniHpbar.setPos(getPosition());
		CVector3 pos = getPosition();

		//�_���[�W�p�����蔻��̈ʒu�̍X�V�B�R���C�_�[�̒��S��ݒ肷��B
		pos.y += mover.GetCollider()->GetHeight()/2 + mover.GetCollider()->GetRadius();
		m_collision.SetPosition(pos);

		//��O�ɗ������ꍇ�A�_���[�W��^���ăz�[���|�W�V�����Ƀe���|�[�g������
		if (getPosition().y < -100.0f) {
			damage({ 0,0,0 }, maxHp / 5);
			mover.SetPosition(team->getHome());
		}
	} else {
		//��莞�Ԃ�������h������
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
	//�_���[�W��^������̂͐����Ă���ꍇ�̂�
	if (m_hp != 0) {
		if (atkTeam == team) {//�����̃`�[���̍U���̓_���[�W�͎󂯂Ȃ����Ռ��͎󂯂�
			damage = 0;
		}
		CVector3 pos = getPosition();
		pos.y += 60.0f;
		//���ƃG�t�F�N�g
		new GameObj::Suicider::CEffekseer(L"Resource/effect/damage.efk", 1.0f, pos);
		playSE(L"Resource/sound/SE_damage.wav", getPosition());
		//�Ռ���^����
		mover.addVelocity(dir);

		//�_���[�W��^����B0�ȉ��ɂȂ�Ȃ玀�S����B
		if (m_hp > damage) {
			m_hp -= damage;
		} else {
			m_hp = 0;
			Death();
		}
		//�_���[�W���󂯂��Ȃ�~�jHP�o�[��\������B
		if (damage != 0) {
			miniHpbar.display(m_hp);
		}
		return true;
	}
	//���S���������Ȃ瓖�������������̖����Ȃ�B(�e�Ȃǂ͊ђʂ���)
	return false;
}

//���S����
void CPlayer::Death() {
	m_hp = 0;
	deathCool = constDeathCool;//�h�����Ԃ̐ݒ�
	m_model.SetIsDraw(false);
	weapon[activeWeapon]->Inactivate();
	//�����蔻�������
	m_collision.SetEnable(false);
	mover.RemoveRigidBoby();
}
//�h������
void CPlayer::Revive() {
	m_hp = maxHp;
	m_model.SetIsDraw(true);
	weapon[activeWeapon]->Activate();
	//�����蔻��̍Đ�
	m_collision.SetEnable(true);
	mover.AddRigidBoby();
}

void CPlayer::Move() {

	const CVector2& movement = action.getMovement();

	//�ǃW�����v
	bool isWalljump = false;
	//�󒆂ɕǂɓ�����Ȃ���W�����v
	if (mover.IsContactWall() && !mover.IsOnGround() && action.isJump()) {
		playSE(L"Resource/sound/SE_jump.wav", getPosition()); m_isSendJumpSE = true;
		mover.walljump(jumpPower, movement);
		isWalljump = true;
	}
	//��s�ŕǂɓ˂�����
	if (mover.isHitWall()) {
		playSE(L"Resource/sound/SE_jump.wav", getPosition());
		isWalljump = true;
	}

	//�W�����v�{�^���������ꂽ�ꍇ
	if (action.isJump() && !isWalljump) {
		//�ڒn���Ă���Ȃ畁�ʂɃW�����v
		if (mover.IsOnGround()) {
			playSE(L"Resource/sound/SE_jump.wav", getPosition()); m_isSendJumpSE = true;
			mover.jump(jumpPower);
		}
		else{//�󒆂ɂ���Ȃ��s��Ԃ�؂�ւ���
			if (mover.isFlying()) {
				mover.flyStop();
			} else {
				mover.fly(true, action.getLookVec(), {0, 0}, flyPower);
			}
		}
	}

	//��s
	if (mover.isFlying()) {
		if (wing == nullptr) {//�����͂₷
			wing = new Wing(mover);
		}
		mover.fly(false, action.getLookVec(), movement);
		mover.turn(action.getLookVec().x, action.getLookVec().z);
		m_model.GetAnimCon().Play(anim_fly, animInterpolateSec);//��s�A�j���[�V����
		//��s���͒ʏ�̈ړ��͂��Ȃ��̂ł����ŏI������B
		return;
	} else {
		//��s���łȂ��̂ɗ��������Ă���Ȃ�A����
		if (wing != nullptr) {
			wing->closeDelete();
			wing = nullptr;
		}
	}

	float speed = moveSpeed;
	bool dash = false;

	//�_�b�V�����͈ړ����x�㏸�A�������ˌ����͖���
	if(action.isDash() && !action.isShot()) {
		speed = moveSpeed * dashMul;
		dash = true;
	}

	//�k���ړ�
	mover.move(movement*speed);

	//�ڒn���A�j���[�V����
	if (mover.IsOnGround()) {
		//�k���ړ��A�j���[�V����
		if (movement.x != 0 || movement.y != 0) {
			if (dash) {
				m_model.GetAnimCon().Play(anim_run, animInterpolateSec);//�_�b�V��
			} else {
				m_model.GetAnimCon().Play(anim_walk, animInterpolateSec);//����
			}
		} else {
			//�������ĂȂ��Ƃ��̃A�j���[�V����
			m_model.GetAnimCon().Play(anim_idle, animInterpolateSec);
		}
	} else {
		//�󒆃A�j���[�V����
		if (mover.getVelocity().y > 0) {
			m_model.GetAnimCon().Play(anim_jump, animInterpolateSec);//�㏸���̓W�����v�A�j���[�V����
		} else {
			m_model.GetAnimCon().Play(anim_fall, animInterpolateSec);//���~�A�j���[�V����
		}
	}

	//��]
	if (action.isShot()) {
		mover.turn(action.getLookVec().x, action.getLookVec().z);//�ˌ����͌�����������
	} else {
		mover.turn(movement.x, movement.y);//�����łȂ���Έړ�����������
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
	if (nextWeapon < 0) { nextWeapon = WEAPON_NUM - 1;}//0�����͖����Ɉړ�
	if (nextWeapon >= WEAPON_NUM) { nextWeapon = 0;}//�v�f���𒴂�����0�Ɉړ�
	
	weapon[activeWeapon]->Inactivate();//�O�̕�����A�N�e�B�u��
	weapon[nextWeapon]->Activate();//�ύX���ꂽ������A�N�e�B�u��
	activeWeapon = (unsigned int)nextWeapon;//��Ɏ����Ă��镐���ύX
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