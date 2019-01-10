#include "stdafx.h"
#include "Citizen.h"
#include "ICitizenBrain.h"
#include "kansen.h"

Citizen::Citizen(std::unordered_map<int, CPlayer*>& pm, ICitizenBrain* moveType): playersMap(pm){
	m_animationClips[anim_walk].Load(L"Resource/animData/CitizenWalk.tka", true);
	m_animationClips[anim_idle].Load(L"Resource/animData/CitizenIdle.tka", true);
	m_model.Init(L"Resource/modelData/Citizen.cmo", m_animationClips, anim_num);
	m_model.SetPos({ 300,100,300 });

	charaCon.Init(15.0f, 80.0f, { 300,100,300 });

	m_collision.CreateCapsule(charaCon.GetPosition(), CQuaternion::Identity(), 30.0f, 80.0f);
	m_collision.SetName(L"Citizen");
	m_collision.SetClass(this);

	mover = moveType;
}

Citizen::~Citizen() {
	delete mover;
}

void Citizen::Update() {
	mover->Update(charaCon.IsOnGround());

	if (atkTime > 0) {
		atkTime -= GetDeltaTimeSec();
		return;
	}
	//UŒ‚
	if (mover->isAtk()) {
		atkTime = 0.5;
		using namespace SuicideObj;
		CVector3 pos = charaCon.GetPosition();
		pos.y += 50;
		CVector3 vec = CVector3::Front()*60;
		mover->getTurn().Multiply(vec);
		pos += vec;

		CCollisionObj* atkCol = new CCollisionObj();
		atkCol->CreateSphere(pos, CQuaternion::Identity(), 20.0f);
		atkCol->SetName(L"ZombieAtk");
		atkCol->SetTimer(2);
		atkCol->SetCallback([&, vec, atkCol](CCollisionObj::SCallbackParam& callback) {
			if (callback.EqualName(L"CPlayer")) {
				CPlayer* p = callback.GetClass<CPlayer>();
				if (ownerTeam->hasPlayer(p)) {
					CVector3 v = vec * 10;
					v.y += 200;
					p->Hit(v);
					atkCol->Delete();
				}
			}
		});
		return;
	}
	//ˆÚ“®
	CVector3 moveVec = mover->getMove();
	if (moveVec.x != 0 || moveVec.z != 0) {
		m_model.GetAnimCon().Play(anim_walk);
	} else {
		m_model.GetAnimCon().Play(anim_idle);
	}
	m_model.SetPos(charaCon.Execute(moveVec));
	m_collision.SetPosition(charaCon.GetPosition());

	//‰ñ“]
	m_model.SetRot(mover->getTurn());
}

bool Citizen::BatHit(CPlayer* player, CVector3 dir) {
	if (ownerTeam != player->team) {
		if (!isKenzoku) {
			Kansenzyoutai();
		}

		if (ownerTeam != nullptr) {
			ownerTeam->removeZombie();
		}
		ownerTeam = player->team;
		ownerTeam->addZombie();

		m_model.GetSkinModel().FindMaterial([&](ModelEffect* mat) {
			mat->SetAlbedoScale(ownerTeam->getColor());
		});
	}
	return true;
}

void Citizen::Kansenzyoutai()
{
	for (AnimationClip& clip : m_animationClips) {
		clip.~AnimationClip();
		new(&clip) AnimationClip();
	}
	m_animationClips[anim_walk].Load(L"Resource/animData/VanpWalk.tka", true);
	m_animationClips[anim_idle].Load(L"Resource/animData/VanpIdle.tka", true);

	m_model.~CSkinModelRender();
	new(&m_model) GameObj::CSkinModelRender();
	m_model.Init(L"Resource/modelData/Vanp.cmo", m_animationClips, anim_num);
	m_model.SetPos(charaCon.GetPosition());

	delete mover;
	mover = new kansen(playersMap,charaCon.GetPosition());

	isKenzoku = true;
}
