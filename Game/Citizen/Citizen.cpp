#include "stdafx.h"
#include "Citizen.h"
#include "ICitizenBrain.h"
#include "kansen.h"
#include "GameWaiter.h"
#include "DemolisherWeapon/Graphic/Effekseer/CEffekseer.h"
#include "CollisionMaskConst.h"

Citizen::Citizen(const std::unordered_map<int, CPlayer*>& pm, ICitizenBrain* moveType): playersMap(pm){
	m_animationClips[anim_walk].Load(L"Resource/animData/CitizenWalk.tka", true);
	m_animationClips[anim_idle].Load(L"Resource/animData/CitizenIdle.tka", true);
	m_model.Init(InstancingNum, L"Resource/modelData/Citizen.cmo", m_animationClips, 2);
	m_model.SetPos({ 300,100,300 });

	for (int i = 0; i < 2; i++) {
		wchar_t name[32];
		if (i == 0) { wcscpy_s(name, L"CitizenWalk"); }
		if (i == 1) { wcscpy_s(name, L"CitizenIdle"); }

		m_ptrCitizenColorManager[i] = FindGO<InstancingCitizenColorManager>(name);
		if (!m_ptrCitizenColorManager[i]) {
			m_ptrCitizenColorManager[i] = NewGO<InstancingCitizenColorManager>(m_model.GetInstancingModel(i));
			m_ptrCitizenColorManager[i]->SetName(name);
		}
	}

	charaCon.Init(15.0f, 80.0f, { 300,100,300 });

	m_collision.CreateCapsule(charaCon.GetPosition(), CQuaternion::Identity(), 30.0f, 80.0f);
	m_collision.SetName(L"Citizen");
	m_collision.SetClass(this);

	//マスクとグループの設定
	m_collision.All_Off_Group();
	m_collision.On_OneGroup(CollisionMaskConst::Citizen);
	m_collision.Off_OneMask(CollisionMaskConst::Citizen);

	//これは喰らい判定
	m_collision.SetIsHurtCollision(true);

	mover = moveType;

	moveType->SetCitizen(this);
}

Citizen::~Citizen() {
	delete mover;
}

void Citizen::Update() {

	//ゲームがウェイト状態なら終わる
	if (GameWaiter::GetIsWait()) { return; }

	deltaTime += GetDeltaTimeSec();
	if (0 == nowFlame || nowFlame == updateFlame) {

		float delta = deltaTime;
		deltaTime = 0;

		mover->Update(charaCon.IsOnGround(), delta);

		//攻撃
		if (isKenzoku) {
			if (attacking) {
				return;
			}
			if (mover->isAtk()) {
				playSE(L"Resource/sound/SE_zombieAtk.wav");
				m_modelAttack.GetAnimCon().Replay(0);
				attacking = true;
				//表示するモデルを切り替え
				m_model.SetIsDraw(false);
				m_modelAttack.SetEnable(true);
				m_modelAttack.RefreshWorldMatrix();//リフレッシュ
				return;
			}
		}
		//移動
		CVector3 moveVec = mover->getMove();
		if (moveVec.x != 0 || moveVec.z != 0) {
			m_model.ChangeAnim(anim_walk);
		} else {
			m_model.ChangeAnim(anim_idle);
		}
		m_model.SetPos(charaCon.Execute(moveVec, delta));
		m_modelAttack.SetPos(charaCon.GetPosition());
		m_collision.SetPosition(charaCon.GetPosition());

		//回転
		m_model.SetRot(mover->getTurn());
		m_modelAttack.SetRot(mover->getTurn());
	}

	if (0 < nowFlame) {
		nowFlame--;
	}
}

bool Citizen::BatHit(CPlayer* player, CVector3 dir) {
	playSE(L"Resource/sound/SE_damage.wav");
	CVector3&& pos = getPos();
	pos.y += 60.0f;
	new GameObj::Suicider::CEffekseer(L"Resource/effect/damage.efk", 1.0f, pos);

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
	m_animationClips[anim_attack].Load(L"Resource/animData/VanpAttack.tka", false);

	m_model.Init(InstancingNum, L"Resource/modelData/Vanp.cmo", m_animationClips, 2);
	m_model.SetPos(charaCon.GetPosition());

	for (int i = 2; i < 4; i++) {
		wchar_t name[32];
		if (i == 2) { wcscpy_s(name, L"VanpWalk"); }
		if (i == 3) { wcscpy_s(name, L"VanpIdle"); }

		m_ptrCitizenColorManager[i] = FindGO<InstancingCitizenColorManager>(name);
		if (!m_ptrCitizenColorManager[i]) {
			m_ptrCitizenColorManager[i] = NewGO<InstancingCitizenColorManager>(m_model.GetInstancingModel(i-2));
			m_ptrCitizenColorManager[i]->SetName(name);
		}
	}

	m_modelAttack.Init(L"Resource/modelData/Vanp.cmo", &m_animationClips[anim_attack], 1);
	m_modelAttack.SetPos(charaCon.GetPosition());
	m_modelAttack.GetAnimCon().AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* evName) {
		if (std::wcscmp(evName, L"attack") == 0) {
			Attack();
		} else if (std::wcscmp(evName, L"attackEnd") == 0) {
			//表示するモデルを切り替え
			m_model.SetIsDraw(true);
			m_modelAttack.SetEnable(false);
			attacking = false;
		}
	});
	m_modelAttack.SetEnable(false);//無効化

	delete mover;
	mover = new kansen(playersMap,charaCon.GetPosition(), ownerTeam);

	isKenzoku = true;
}

void Citizen::playSE(const wchar_t * path) {
	SuicideObj::CSE* se = NewGO<SuicideObj::CSE>(path);
	se->SetPos(charaCon.GetPosition());//音の位置
	se->SetDistance(500.0f);//音が聞こえる範囲
	se->Play(true); //第一引数をtrue
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
				p->Hit(v);
				atkCol->Delete();
			}
		}
	});
}

void Citizen::PostLoopUpdate() {
	if (m_model.GetIsDraw()) {//インスタンシング描画するなら
		//インスタンシングモデルにカラーのセット
		int index = (isKenzoku ? 2 : 0) + m_model.GetPlayAnimNum();
		if (ownerTeam) {
			m_ptrCitizenColorManager[index]->AddColor(ownerTeam->getColor());
		}
		else {
			m_ptrCitizenColorManager[index]->AddColor(CVector4::One());
		}
	}
}