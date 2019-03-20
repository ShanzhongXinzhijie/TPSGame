#include "stdafx.h"
#include "Wosiris.h"
#include "CollisionMaskConst.h"
#include "Bullet.h"
#include "Cplayer.h"
#include "Game.h"
#include "Weapon.h"
#include "Syouraidan.h"

Wosiris::Wosiris(CPlayer* owner, float rot)
{	
	m_owner = owner;
	m_rot = rot;
}
Wosiris::~Wosiris()
{
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

	//�n�`����
	m_atari.Init(L"Resource\\modelData\\wosirisAtari.cmo", enFbxUpAxisY);
	m_phyStaticObject.CreateMesh(m_model.GetPos(), m_model.GetRot(), m_model.GetScale(), m_atari);

	//�R���W����
	m_collision.CreateMesh(m_model.GetPos(), m_model.GetRot(), m_model.GetScale(), m_atari);
	m_collision.SetName(L"Wosiris");
	m_collision.SetClass(this);
	//�}�X�N�ƃO���[�v�̐ݒ�
	m_collision.All_Off_Group();
	m_collision.On_OneGroup(CollisionMaskConst::encolKurai);
	m_collision.Off_OneMask(CollisionMaskConst::encolKurai);
	//����͋�炢����
	m_collision.SetIsHurtCollision(true);
	//�e���q�b�g�������̏���
	m_collision.SetCallback([&](SuicideObj::CCollisionObj::SCallbackParam& callback) {
		if (callback.EqualName(L"Bullet")) {
			Bullet* bullet = callback.GetClass<Bullet>();
			if (bullet->getDamage() >= m_point && m_owner != bullet->getShooter()) {
				//playSE(L"Resource/sound/SE_zombie.wav");

				m_owner = bullet->getShooter();					
				m_model.GetSkinModel().FindMaterialSetting([&](MaterialSetting* mat) {
					if (mat->EqualMaterialName(L"wing")) {
						mat->SetAlbedoScale(m_owner->team->getColor());
					}
				});

				//TUSINN
				//�������ő��ɂȂ������Ƃ𑗐M
				//if (player->playerNum == GetPhoton()->GetLocalPlayerNumber() && player->GetNetCaster()) {
				//	player->GetNetCaster()->SendNewKenzoku(this);
				//}
			}
		}
	});

	if (m_owner) {
		m_model.GetSkinModel().FindMaterialSetting([&](MaterialSetting* mat) {
			if (mat->EqualMaterialName(L"wing")) {
				mat->SetAlbedoScale(m_owner->team->getColor());
			}
		});
	}

	//2D
	m_window.Init(L"Resource/spriteData/osirisWindow.dds");
	m_font.UseSystemFont();

	//�v���C���[���X�g
	for(auto& P : m_game->getPlayers()){
		m_players.emplace_back(P.second, P.second->GetIsDead());
	}

	return true;
}

void Wosiris::Update(){
	//�t���b�V��
	if (m_flashTimer > 0.0f) {
		m_flashTimer -= GetDeltaTimeSec();
	}

	if (!m_owner) { return; }

	//�����e
	for(auto& P : m_players){
		if(m_owner != P.first && P.second && !P.first->GetIsDead()){//�h������
			CVector3 fromPos = {18.85f,166.42f,196.43f };
			fromPos *= m_model.GetScale();
			CQuaternion rot = m_model.GetRot();
			rot.Multiply(fromPos);
			fromPos += m_model.GetPos();
			new Syouraidan(fromPos, P.first);
		}
		P.second = P.first->GetIsDead();
	}

	//�|�C���g�v�Z
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
	swprintf_s(atk, L"���V���X�̓V��\n\n�U����\n%d", m_point);
	m_font.Draw(atk, { 0.9f,0.4f+0.025f }, { 1.0f,0.82f,0.26f,1.0f }, CVector2::One()*0.5f, { 0.5f,0.0f });
}
