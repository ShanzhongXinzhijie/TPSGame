#include "stdafx.h"
#include "Wosiris.h"
#include "CollisionMaskConst.h"
#include "Bullet.h"
#include "Cplayer.h"
#include "Game.h"
#include "Weapon.h"
#include "Syouraidan.h"
#include "GingerGene.h"

Wosiris::Wosiris(CPlayer* owner, float rot, GingerGene* gg)
{	
	m_owner = owner;
	m_rot = rot;

	m_gingergene = gg;
	m_gingergene->SetWosiris(this);
	m_gingergene->Register(this);
}
Wosiris::~Wosiris()
{
	m_gingergene->SetWosiris(nullptr);
	m_gingergene->Unregister(this);
}

bool Wosiris::Start(){
	m_game = FindGO<Game>(L"GameClass"); if (!m_game) { return false; }

	m_model.Init(L"Resource\\modelData\\wosiris.cmo", enFbxUpAxisY);
	m_model.SetPos(CVector3::AxisY()*4000.0f);
	CQuaternion rot[2]; 
	rot[0].SetRotationDeg(CVector3::AxisX(), 20.0f);
	rot[1].SetRotation(CVector3::AxisY(), m_rot);
	rot[1].Multiply(rot[0]);
	m_model.SetRot(rot[1]);
	m_model.SetScale(CVector3::One()*4.0f);

	//地形判定
	m_atari.Init(L"Resource\\modelData\\wosirisAtari.cmo", enFbxUpAxisY);
	m_phyStaticObject.CreateMesh(m_model.GetPos(), m_model.GetRot(), m_model.GetScale(), m_atari);

	//コリジョン
	m_collision.CreateMesh(m_model.GetPos(), m_model.GetRot(), m_model.GetScale(), m_atari);
	m_collision.SetName(L"Wosiris");
	m_collision.SetClass(this);
	//マスクとグループの設定
	m_collision.All_Off_Group();
	m_collision.On_OneGroup(CollisionMaskConst::encolKurai);
	m_collision.Off_OneMask(CollisionMaskConst::encolKurai);
	//これは喰らい判定
	m_collision.SetIsHurtCollision(true);
	//弾がヒットした時の処理
	m_collision.SetCallback([&](SuicideObj::CCollisionObj::SCallbackParam& callback) {
		if (callback.EqualName(L"Bullet")) {
			Bullet* bullet = callback.GetClass<Bullet>();
			if (bullet->getDamage() >= m_point && m_owner != bullet->getShooter()) {				
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
					ChangeControl(bullet->getShooter());
				}
			}
		}
	});

	if (m_owner) {
		m_model.GetSkinModel().FindMaterialSetting([&](MaterialSetting* mat) {
			if (mat->EqualMaterialName(L"wing")) {
				mat->SetAlbedoScale(m_owner->getTeam()->getColor());
			}
		});
	}

	//2D
	m_window.Init(L"Resource/spriteData/osirisWindow.dds");
	m_font.UseSystemFont();

	//プレイヤーリスト
	for(auto& P : m_game->getPlayers()){
		m_players.emplace_back(P.second, P.second->GetIsDead());
	}

	//SE
	SuicideObj::CSE* se = NewGO<SuicideObj::CSE>(L"Resource/sound/SE_wosiris.wav");
	se->Play();

	return true;
}

void Wosiris::Update(){
	//フラッシュ
	if (m_flashTimer > 0.0f) {
		m_flashTimer -= GetDeltaTimeSec();
	}

	if (!m_owner) { return; }

	//召雷弾
	for(auto& P : m_players){
		//蘇生した&インビジブル状態でない
		if(m_owner != P.first && P.second && !P.first->GetIsDead() && !P.first->GetInvisible()){
			CVector3 fromPos = {18.85f,166.42f,196.43f };
			fromPos *= m_model.GetScale();
			CQuaternion rot = m_model.GetRot();
			rot.Multiply(fromPos);
			fromPos += m_model.GetPos();
			new Syouraidan(fromPos, P.first, &m_game->GetGingerGene());
			//SE
			SuicideObj::CSE* se = NewGO<SuicideObj::CSE>(L"Resource/sound/SE_syouraidan.wav");
			se->SetPos(fromPos);//音の位置
			se->SetDistance(50000.0f);//音が聞こえる範囲
			se->Play(true);
		}
		P.second = P.first->GetIsDead();
	}

	//ポイント計算
	int cnt = 0;
	if (!m_owner->GetIsDead()) {
		Weapon** Weapons = m_owner->GetWeapons();
		for (int i = 0; i < m_owner->GetWeaponNum(); i++) {
			if (Weapons[i]->getBulletCount() > 0) {
				cnt++;
			}
		}
	}
	m_point = 1000 * cnt;	
}

void Wosiris::PostRender() {

	if(m_flashTimer > 0.0f){
		m_window.Draw({ 0.5f,0.5f }, {GetGraphicsEngine().GetFrameBuffer_W() / 240.0f,GetGraphicsEngine().GetFrameBuffer_H() / 169.0f }, { 0.5f,0.5f }, 0.0f, { 1000.0f,1000.0f ,1000.0f ,1.0f });
		return;
	}

	m_window.Draw({ 0.9f,0.4f }, CVector2::One()*0.85f, {0.5f,0.0f});
	
	wchar_t atk[64];
	swprintf_s(atk, L"ヲシリスの天空龍\n\n攻撃力\n%d", m_point);
	m_font.Draw(atk, { 0.9f,0.4f+0.025f }, { 1.0f,0.82f,0.26f,1.0f }, CVector2::One()*0.5f, { 0.5f,0.0f });
}

void Wosiris::ChangeControl(CPlayer* P) {
	//SE
	SuicideObj::CSE* se = NewGO<SuicideObj::CSE>(L"Resource/sound/SE_wosiris.wav");
	se->Play();

	m_owner = P;
	m_model.GetSkinModel().FindMaterialSetting([&](MaterialSetting* mat) {
		if (mat->EqualMaterialName(L"wing")) {
			mat->SetAlbedoScale(m_owner->getTeam()->getColor());
		}
	});

	//自分の眷属になったことを送信
	if (P->playerNum == GetPhoton()->GetLocalPlayerNumber() && P->GetNetCaster()) {
		P->GetNetCaster()->SendControlWosiris();
	}
}
