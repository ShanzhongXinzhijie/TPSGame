#include "stdafx.h"
#include "Citizen.h"
#include "ICitizenBrain.h"
#include "kansen.h"
#include "Bullet.h"
#include "GameWaiter.h"
#include "DemolisherWeapon/Graphic/Effekseer/CEffekseer.h"
#include "CollisionMaskConst.h"

Citizen::Citizen(const std::unordered_map<int, CPlayer*>& pm, ICitizenBrain* moveType, unsigned int id): playersMap(pm), m_uniqueID(id){
	//�C���X�^���V���O���f��
	AnimationClip animationClips[anim_num];
	animationClips[anim_walk0].Load(L"Resource/animData/CitizenWalk.tka", true);
	animationClips[anim_walk1].Load(L"Resource/animData/CitizenWalk.tka", true, 0.3f);
	animationClips[anim_walk2].Load(L"Resource/animData/CitizenWalk.tka", true, 0.6f);
	animationClips[anim_idle].Load(L"Resource/animData/CitizenIdle.tka", true);
	m_model.Init(InstancingNum, L"Resource/modelData/Citizen.cmo", animationClips, anim_num);
	m_model.SetPos({ 300,100,300 });
	//�C���X�^���V���O���f���̐F�ς���N���X
	for (int i = 0; i < anim_num; i++) {
		wchar_t name[32];
		if (i == anim_walk0) { wcscpy_s(name, L"CitizenWalk0"); }
		if (i == anim_walk1) { wcscpy_s(name, L"CitizenWalk1"); }
		if (i == anim_walk2) { wcscpy_s(name, L"CitizenWalk2"); }
		if (i == anim_idle) { wcscpy_s(name, L"CitizenIdle"); }

		m_ptrCitizenColorManager[i] = FindGO<InstancingCitizenColorManager>(name);
		if (!m_ptrCitizenColorManager[i]) {
			//�Ȃ���΍��
			m_ptrCitizenColorManager[i] = NewGO<InstancingCitizenColorManager>(m_model.GetInstancingModel(i));
			m_ptrCitizenColorManager[i]->SetName(name);
		}
	}

	charaCon.Init(30.0f, 40.0f, { 300,100,300 });
	CVector3 pos = charaCon.GetPosition();
	pos.y += charaCon.GetCollider()->GetHeight() / 2 + charaCon.GetCollider()->GetRadius();
	m_collision.CreateCapsule(pos, CQuaternion::Identity(), 30.0f, 40.0f);
	m_collision.SetName(L"Citizen");
	m_collision.SetClass(this);

	//�}�X�N�ƃO���[�v�̐ݒ�
	m_collision.All_Off_Group();
	m_collision.On_OneGroup(CollisionMaskConst::encolKurai);
	m_collision.Off_OneMask(CollisionMaskConst::encolKurai);

	//����͋�炢����
	m_collision.SetIsHurtCollision(true);

	mover = moveType;

	moveType->SetCitizen(this);
}

Citizen::~Citizen() {
	delete mover;
}

void Citizen::Update() {

	//�Q�[�����E�F�C�g��ԂȂ�I���
	if (GameWaiter::GetIsWait()) { return; }

	m_netCnt++;//�ʐM�p�J�E���^�i�߂�

	deltaTime += GetDeltaTimeSec();
	if (0 == nowFlame || nowFlame == updateFlame) {

		float delta = deltaTime;
		deltaTime = 0;

		mover->Update(charaCon.IsOnGround(), delta);

		//�U��
		if (isKenzoku) {
			if (attacking) {
				return;
			}
			if (mover->isAtk()) {
				playSE(L"Resource/sound/SE_zombieAtk.wav");
				m_modelAttack.GetAnimCon().Replay(0);
				attacking = true;

				//�\�����郂�f����؂�ւ�
				m_model.SetIsDraw(false);
				m_modelAttack.SetEnable(true);
				m_modelAttack.RefreshWorldMatrix();//���t���b�V��
				return;
			}
		}
		//�ړ�
		CVector3 moveVec = mover->getMove();
		if (moveVec.x != 0 || moveVec.z != 0) {
			m_model.ChangeAnim((int)anim_walk0 + (m_uniqueID % 3));
		} else {
			m_model.ChangeAnim(anim_idle);
		}
		m_model.SetPos(charaCon.Execute(moveVec, delta));
		m_modelAttack.SetPos(charaCon.GetPosition());
		CVector3 pos = charaCon.GetPosition();
		pos.y += charaCon.GetCollider()->GetHeight() / 2 + charaCon.GetCollider()->GetRadius();
		m_collision.SetPosition(pos);

		//��]
		m_model.SetRot(mover->getTurn());
		m_modelAttack.SetRot(mover->getTurn());
	}

	if (0 < nowFlame) {
		nowFlame--;
	}

#ifndef SpritScreen
	//�������v���C���[(����)�ɋ߂��ƈʒu����
	if ((playersMap.at(GetPhoton()->GetLocalPlayerNumber())->getPosition() - getPos()).LengthSq() < 100.0f*100.0f) {
		playersMap.at(GetPhoton()->GetLocalPlayerNumber())->GetNetCaster()->SendSyncCitizen(this);
	}
#endif
}

bool Citizen::BatHit(Bullet* bullet) {
	playSE(L"Resource/sound/SE_damage.wav");
	CVector3 pos = getPos();
	pos.y += 60.0f;
	new GameObj::Suicider::CEffekseer(L"Resource/effect/damage.efk", 1.0f, pos);

	//���`�[���̍U���̓_���[�W���󂯂Ȃ�
	if (ownerTeam != bullet->getShooter()->team) {

		if (m_hp > bullet->getDamage()) {
			m_hp -= bullet->getDamage();
		} else {
			m_hp = maxHp;

			bool canKenzokuing = false;
#ifndef SpritScreen
			//�ʐM���͎������ő����������s
			if (bullet->getShooter()->playerNum == GetPhoton()->GetLocalPlayerNumber()) {
				canKenzokuing = true;
			}
#else
			canKenzokuing = true;
#endif
			if (canKenzokuing) {
				//�ő���
				ChangeToKenzoku(bullet->getShooter());
			}
		}
	}
	return true;
}

void Citizen::ChangeToKenzoku(CPlayer* player) {
	if (ownerTeam != player->team) {
		if (!isKenzoku) {
			Kansenzyoutai();
		}

		playSE(L"Resource/sound/SE_zombie.wav");

		if (ownerTeam != nullptr) {
			ownerTeam->removeZombie();
		}
		ownerTeam = player->team;
		ownerTeam->addZombie();

		m_modelAttack.GetSkinModel().FindMaterialSetting([&](MaterialSetting* mat) {
			mat->SetAlbedoScale(ownerTeam->getColor());
		});

		//�������ő��ɂȂ������Ƃ𑗐M
		if (player->playerNum == GetPhoton()->GetLocalPlayerNumber() && player->GetNetCaster()) {
			player->GetNetCaster()->SendNewKenzoku(this);
		}
	}
}

void Citizen::Kansenzyoutai()
{
	//�C���X�^���V���O���f��
	AnimationClip animationClips[anim_num];
	animationClips[anim_walk0].Load(L"Resource/animData/VanpWalk.tka", true);
	animationClips[anim_walk1].Load(L"Resource/animData/VanpWalk.tka", true, 0.3f);
	animationClips[anim_walk2].Load(L"Resource/animData/VanpWalk.tka", true, 0.6f);
	animationClips[anim_idle].Load(L"Resource/animData/VanpIdle.tka", true);
	m_model.Init(InstancingNum, L"Resource/modelData/Vanp.cmo", animationClips, anim_num);
	m_model.SetPos(charaCon.GetPosition());
	//�C���X�^���V���O���f���̐F�ς���N���X
	for (int i = anim_num; i < (int)anim_num * 2; i++) {
		wchar_t name[32];
		if (i == (int)anim_num + anim_walk0) { wcscpy_s(name, L"VanpWalk0"); }
		if (i == (int)anim_num + anim_walk1) { wcscpy_s(name, L"VanpWalk1"); }
		if (i == (int)anim_num + anim_walk2) { wcscpy_s(name, L"VanpWalk2"); }
		if (i == (int)anim_num + anim_idle) { wcscpy_s(name, L"VanpIdle"); }

		m_ptrCitizenColorManager[i] = FindGO<InstancingCitizenColorManager>(name);
		if (!m_ptrCitizenColorManager[i]) {
			//�Ȃ���΍��
			m_ptrCitizenColorManager[i] = NewGO<InstancingCitizenColorManager>(m_model.GetInstancingModel(i - (int)anim_num));
			m_ptrCitizenColorManager[i]->SetName(name);
		}
	}

	//�U�����f��
	m_attackAnimationClip.Load(L"Resource/animData/VanpAttack.tka", false);
	m_modelAttack.Init(L"Resource/modelData/Vanp.cmo", &m_attackAnimationClip, 1);
	m_modelAttack.SetPos(charaCon.GetPosition());
	m_modelAttack.GetAnimCon().AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* evName) {
		if (std::wcscmp(evName, L"attack") == 0) {
			Attack();
		} else if (std::wcscmp(evName, L"attackEnd") == 0) {
			//�\�����郂�f����؂�ւ�
			m_model.SetIsDraw(true);
			m_modelAttack.SetEnable(false);

			attacking = false;
		}
	});
	m_modelAttack.SetEnable(false);//������

	charaCon.RemoveRigidBoby();
	charaCon.Init(30.0f, 70.0f, charaCon.GetPosition());
	CVector3 pos = charaCon.GetPosition();
	pos.y += charaCon.GetCollider()->GetHeight() / 2 + charaCon.GetCollider()->GetRadius();
	m_collision.CreateCapsule(pos, CQuaternion::Identity(), 30.0f, 70.0f);

	delete mover;
	mover = new kansen(playersMap,charaCon.GetPosition(), ownerTeam);
	mover->SetCitizen(this);

	isKenzoku = true;
}

void Citizen::playSE(const wchar_t * path) {
	SuicideObj::CSE* se = NewGO<SuicideObj::CSE>(path);
	se->SetPos(charaCon.GetPosition());//���̈ʒu
	se->SetDistance(500.0f);//������������͈�
	se->Play(true); //��������true
}

void Citizen::Attack() {
	using namespace SuicideObj;
	CVector3 pos = charaCon.GetPosition();
	pos.y += 50;
	CVector3 vec = CVector3::Front() * 40;
	mover->getTurn().Multiply(vec);
	pos += vec;
	CCollisionObj* atkCol = new CCollisionObj();
	atkCol->CreateSphere(pos, CQuaternion::Identity(), 40.0f);
	atkCol->SetName(L"ZombieAtk");
	atkCol->SetTimer(2);
	atkCol->SetCallback([&, vec, atkCol](CCollisionObj::SCallbackParam& callback) {
		if (callback.EqualName(L"CPlayer")) {
			CPlayer* p = callback.GetClass<CPlayer>();

			if (!ownerTeam->hasPlayer(p)) {
				CVector3 v = vec * 20;
				v.y += 200;
				p->Hit(v, attackPower);
				atkCol->Delete();
			}
		}
	});
}

void Citizen::PostLoopUpdate() {
	if (m_model.GetIsDraw()) {//�C���X�^���V���O�`�悷��Ȃ�
		//�C���X�^���V���O���f���ɃJ���[�̃Z�b�g
		int index = (isKenzoku ? (int)anim_num : 0) + m_model.GetPlayAnimNum();
		if (ownerTeam) {
			m_ptrCitizenColorManager[index]->AddColor(ownerTeam->getColor());
		}
		else {
			m_ptrCitizenColorManager[index]->AddColor(CVector4::One());
		}
	}
}

const CVector3& Citizen::GetMoverNetVec()const {
	return mover->GetNetVec();
}
void Citizen::SetMoverNetVec(const CVector3& v) {
	mover->SetNetVec(v);
}
