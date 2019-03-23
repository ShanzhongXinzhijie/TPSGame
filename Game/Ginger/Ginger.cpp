#include "stdafx.h"
#include "Ginger.h"
#include "CollisionMaskConst.h"
#include "Bullet.h"
#include "CPlayer.h"
#include "../Network/NetPlayerCaster.h"
#include "../Network/NetPlayerReceiver.h"
#include "GameWaiter.h"

Ginger::Ginger(int index, NetPlayerReceiver* receiver, int timeSec, GodPowerType powerType) : m_index(index),m_receiver(receiver)
{
	m_kensetuLeftTime = timeSec * GetStandardFrameRate();
	m_powerType = powerType;

	m_pos = { 2863.38f - 2863.38f*2.0f*(rand() % 101 * 0.01f), -100.0f, 2863.38f - 2863.38f*2.0f*(rand() % 101 * 0.01f) };
	if (rand() % 100 <= 12) {
		m_pos.y = 3000.0f;
	}

	m_rot.SetRotationDeg(CVector3::AxisY(), 360.0f*(rand() % 101 * 0.01f));
}
Ginger::~Ginger()
{
}

bool Ginger::Start() {
	m_model.Init(L"Resource\\modelData\\Ginger.cmo", enFbxUpAxisY);	

	if (m_pos.y < 0.0f) {
		//���C�Ŕ���
		btVector3 rayStart = btVector3(m_pos.x, 3000.0f, m_pos.z);
		btVector3 rayEnd = btVector3(m_pos.x, -50.0f, m_pos.z);
		btCollisionWorld::AllHitsRayResultCallback gnd_ray(rayStart, rayEnd);
		GetEngine().GetPhysicsWorld().RayTest(rayStart, rayEnd, gnd_ray);
		if (gnd_ray.hasHit()) {
			for (int i = 0; i < gnd_ray.m_collisionObjects.size(); ++i) {
				const btCollisionObject* col = gnd_ray.m_collisionObjects[i];
				if (col->getUserIndex() == enCollisionAttr_Character
					|| col->getInternalType() == btCollisionObject::CO_GHOST_OBJECT
					) {
					continue;
				}
				if (m_pos.y < gnd_ray.m_hitPointWorld[i].y()) {
					m_pos.y = gnd_ray.m_hitPointWorld[i].y();
				}
			}
		}
	}

	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.SetScale({ 1.0f,0.0f,1.0f });
	
	//�e�N�X�`�������ւ�
	ID3D11ShaderResourceView* srv = nullptr;
	DirectX::CreateDDSTextureFromFile(GetEngine().GetGraphicsEngine().GetD3DDevice(), L"Resource\\modelData\\mud.dds", nullptr, &srv);
	m_model.GetSkinModel().FindMaterialSetting([&](MaterialSetting* mat) {
		mat->SetAlbedoTexture(srv);
	});
	srv->Release();

	m_kensetuLeftTimeMax = m_kensetuLeftTime;

	return true;
}

void Ginger::Konryu(){

	//�e�N�X�`�������ւ�
	m_model.GetSkinModel().FindMaterialSetting([&](MaterialSetting* mat) {
		mat->SetDefaultAlbedoTexture();
		mat->SetUVOffset({0.0f,0.0f});
	});

	//�n�`����
	m_phyStaticObject.CreateMesh(m_model);

	//�Â�����	
	for (int i = 0; i < 2; i++) {
		if (i == 0) {
			CVector3 offset = CVector3(0.0f, 376.115f, -815.025f)*0.21f; m_rot.Multiply(offset);
			m_collision[i].CreateBox(m_pos + offset, m_rot, CVector3(376.115f*1.2f + 10.0f, 376.115f + 10.0f, 376.115f*1.2f)*0.21f*2.0f);
		}
		else {
			CVector3 offset = CVector3(0.0f, 671.355f, -0.0f)*0.21f; m_rot.Multiply(offset);
			m_collision[i].CreateBox(m_pos + offset, m_rot, CVector3(479.25f*1.2f + 10.0f, 671.355f + 10.0f, 73.04f)*0.21f*2.0f);
		}
		m_collision[i].SetName(L"Ginger");
		m_collision[i].SetClass(this);
		//�}�X�N�ƃO���[�v�̐ݒ�
		m_collision[i].All_Off_Group();
		m_collision[i].On_OneGroup(CollisionMaskConst::encolKurai);
		m_collision[i].Off_OneMask(CollisionMaskConst::encolKurai);
		//����͋�炢����
		m_collision[i].SetIsHurtCollision(true);
		//�e���q�b�g�������̏���
		m_collision[i].SetCallback([&](SuicideObj::CCollisionObj::SCallbackParam& callback) {
			if (callback.EqualName(L"Bullet")) {
				Bullet* bullet = callback.GetClass<Bullet>();
				m_hp -= bullet->getDamage();
				if (bullet->getDamage() > 0) {
					//�G�t�F�N�g
					CVector3 pos = CVector3::AxisZ()*(-400.0f*0.21f); m_rot.Multiply(pos);
					SuicideObj::CEffekseer* effe = new SuicideObj::CEffekseer(L"Resource/effect/hakai.efk", 1.0f, m_pos + pos);
					effe->SetScale({ 60.0f, 60.0f ,60.0f });
					//SE
					SuicideObj::CSE* se = NewGO<SuicideObj::CSE>(L"Resource/sound/SE_gingerDamege.wav");
					se->SetPos(m_pos);//���̈ʒu
					se->SetDistance(500.0f);//������������͈�
					se->Play(true);
				}
				if(m_hp <= 0 && !m_isWillDestroy){//�j��
					m_isWillDestroy = true;
#ifdef SpritScreen
					//�_�̃p���[�l��
					bullet->getShooter()->SetGodPower((GodPowerType)m_powerType);
					//�j��
					Destory();
#else
					//�}�X�^�[���グ�����߂�
					if (bullet->getShooter()->playerNum == GetPhoton()->GetLocalPlayerNumber()) {
						if (GetPhoton()->GetIsMasterClient()) {
							m_receiver->AddGodPowerLottery(m_index, bullet->getShooter()->GetNetCaster()->GetTime(), bullet->getShooter()->playerNum);
						}
						else {
							bullet->getShooter()->GetNetCaster()->SendDestroyGinger(m_index);
						}
					}
#endif
				}
			}
		});
	}

	//�G�t�F�N�g
	CVector3 pos = CVector3::AxisZ()*(-400.0f*0.21f); m_rot.Multiply(pos);
	SuicideObj::CEffekseer* effe = new SuicideObj::CEffekseer(L"Resource/effect/aura.efk", 1.0f, m_pos + pos);
	effe->SetScale({ 100.0f, 100.0f ,100.0f });

	//SE
	SuicideObj::CSE* se = NewGO<SuicideObj::CSE>(L"Resource/sound/SE_ginger.wav");
	se->SetPos(m_pos);//���̈ʒu
	se->SetDistance(500.0f);//������������͈�
	se->Play(true);

	m_isKensetued = true;
}

void Ginger::Update() {
	//�Q�[�����E�F�C�g��ԂȂ�I���
	if (GameWaiter::GetIsWait()) { return; }

	//����
	m_kensetuLeftTime--;
	if(!m_isKensetued && m_kensetuLeftTime <= 0){
		Konryu();
	}
	
	if (m_isKensetued) {
		m_model.SetScale({ 1.0f,1.0f,1.0f });
	}
	else {
		m_model.SetScale({ 1.0f,1.0f - ((float)m_kensetuLeftTime / m_kensetuLeftTimeMax),1.0f });
		//�e�N�X�`���X�N���[��
		m_uvScroll += 0.03f/60.0f; if (m_uvScroll > 1.0f) { m_uvScroll -= 1.0f; }
		m_model.GetSkinModel().FindMaterialSetting([&](MaterialSetting* mat) {
			mat->SetUVOffset({ m_uvScroll,m_uvScroll });
		});
	}
}

void Ginger::Destory() {
	SetEnable(false);
	m_model.SetEnable(false);
	m_phyStaticObject.Release();
	m_collision[0].SetEnable(false);
	m_collision[1].SetEnable(false);
}