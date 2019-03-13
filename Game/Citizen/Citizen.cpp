#include "stdafx.h"
#include "Citizen.h"
#include "ICitizenBrain.h"
#include "kansen.h"
#include "Bullet.h"
#include "GameWaiter.h"
#include "DemolisherWeapon/Graphic/Effekseer/CEffekseer.h"
#include "CollisionMaskConst.h"

Citizen::Citizen(const std::unordered_map<int, CPlayer*>& pm, ICitizenBrain* moveType, unsigned int id): playersMap(pm), m_uniqueID(id){
	//インスタンシングモデル
	AnimationClip animationClips[anim_num];
	animationClips[anim_walk0].Load(L"Resource/animData/CitizenWalk.tka", true);
	animationClips[anim_walk1].Load(L"Resource/animData/CitizenWalk.tka", true, 0.3f);
	animationClips[anim_walk2].Load(L"Resource/animData/CitizenWalk.tka", true, 0.6f);
	animationClips[anim_idle].Load(L"Resource/animData/CitizenIdle.tka", true);
	m_model.Init(InstancingNum, L"Resource/modelData/Citizen.cmo", animationClips, anim_num);
	m_model.SetPos({ 300,100,300 });
	//インスタンシングモデルの色変えるクラス
	for (int i = 0; i < anim_num; i++) {
		wchar_t name[32];
		if (i == anim_walk0) { wcscpy_s(name, L"CitizenWalk0"); }
		if (i == anim_walk1) { wcscpy_s(name, L"CitizenWalk1"); }
		if (i == anim_walk2) { wcscpy_s(name, L"CitizenWalk2"); }
		if (i == anim_idle) { wcscpy_s(name, L"CitizenIdle"); }

		m_ptrCitizenColorManager[i] = FindGO<InstancingCitizenColorManager>(name);
		if (!m_ptrCitizenColorManager[i]) {
			//なければ作る
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

	//マスクとグループの設定
	m_collision.All_Off_Group();
	m_collision.On_OneGroup(CollisionMaskConst::encolKurai);
	m_collision.Off_OneMask(CollisionMaskConst::encolKurai);

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

	m_netCnt++;//通信用カウンタ進める

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
			m_model.ChangeAnim((int)anim_walk0 + (m_uniqueID % 3));
		} else {
			m_model.ChangeAnim(anim_idle);
		}
		m_model.SetPos(charaCon.Execute(moveVec, delta));
		m_modelAttack.SetPos(charaCon.GetPosition());
		CVector3 pos = charaCon.GetPosition();
		pos.y += charaCon.GetCollider()->GetHeight() / 2 + charaCon.GetCollider()->GetRadius();
		m_collision.SetPosition(pos);

		//回転
		m_model.SetRot(mover->getTurn());
		m_modelAttack.SetRot(mover->getTurn());
	}

	if (0 < nowFlame) {
		nowFlame--;
	}

#ifndef SpritScreen
	//距離がプレイヤー(自分)に近いと位置同期
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

	//自チームの攻撃はダメージを受けない
	if (ownerTeam != bullet->getShooter()->team) {

		if (m_hp > bullet->getDamage()) {
			m_hp -= bullet->getDamage();
		} else {
			m_hp = maxHp;

			bool canKenzokuing = false;
#ifndef SpritScreen
			//通信時は自分の眷属化だけ実行
			if (bullet->getShooter()->playerNum == GetPhoton()->GetLocalPlayerNumber()) {
				canKenzokuing = true;
			}
#else
			canKenzokuing = true;
#endif
			if (canKenzokuing) {
				//眷属化
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

		//自分の眷属になったことを送信
		if (player->playerNum == GetPhoton()->GetLocalPlayerNumber() && player->GetNetCaster()) {
			player->GetNetCaster()->SendNewKenzoku(this);
		}
	}
}

void Citizen::Kansenzyoutai()
{
	//インスタンシングモデル
	AnimationClip animationClips[anim_num];
	animationClips[anim_walk0].Load(L"Resource/animData/VanpWalk.tka", true);
	animationClips[anim_walk1].Load(L"Resource/animData/VanpWalk.tka", true, 0.3f);
	animationClips[anim_walk2].Load(L"Resource/animData/VanpWalk.tka", true, 0.6f);
	animationClips[anim_idle].Load(L"Resource/animData/VanpIdle.tka", true);
	m_model.Init(InstancingNum, L"Resource/modelData/Vanp.cmo", animationClips, anim_num);
	m_model.SetPos(charaCon.GetPosition());
	//インスタンシングモデルの色変えるクラス
	for (int i = anim_num; i < (int)anim_num * 2; i++) {
		wchar_t name[32];
		if (i == (int)anim_num + anim_walk0) { wcscpy_s(name, L"VanpWalk0"); }
		if (i == (int)anim_num + anim_walk1) { wcscpy_s(name, L"VanpWalk1"); }
		if (i == (int)anim_num + anim_walk2) { wcscpy_s(name, L"VanpWalk2"); }
		if (i == (int)anim_num + anim_idle) { wcscpy_s(name, L"VanpIdle"); }

		m_ptrCitizenColorManager[i] = FindGO<InstancingCitizenColorManager>(name);
		if (!m_ptrCitizenColorManager[i]) {
			//なければ作る
			m_ptrCitizenColorManager[i] = NewGO<InstancingCitizenColorManager>(m_model.GetInstancingModel(i - (int)anim_num));
			m_ptrCitizenColorManager[i]->SetName(name);
		}
	}

	//攻撃モデル
	m_attackAnimationClip.Load(L"Resource/animData/VanpAttack.tka", false);
	m_modelAttack.Init(L"Resource/modelData/Vanp.cmo", &m_attackAnimationClip, 1);
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
				p->Hit(v, attackPower);
				atkCol->Delete();
			}
		}
	});
}

void Citizen::PostLoopUpdate() {
	if (m_model.GetIsDraw()) {//インスタンシング描画するなら
		//インスタンシングモデルにカラーのセット
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
