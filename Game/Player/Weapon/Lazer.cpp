#include "stdafx.h"
#include "Lazer.h"
#include "NormalBullet.h"
#include "CPlayer.h"
#include "Game.h"

Lazer::Lazer(CPlayer* player, GameObj::CSkinModelRender* playerModel,
	unsigned int shotAnim, unsigned int reloadAnim)
	:Weapon(player, playerModel, getInfo(shotAnim, reloadAnim)) {

	m_game = FindGO<Game>(L"GameClass");
	m_sprite.Init(L"Resource/spriteData/lockon.dds");
}

Lazer::~Lazer() {
}

Bullet* Lazer::createBullet(CPlayer * player, CVector3 pos, CVector3 dir) {
	if (player->GetLockOnNum() < 0) { return nullptr; }//�m�[���b�N

	//�G�t�F�N�g
	CVector3 Dir, axis;
	CQuaternion bias, rot, rot2;
	bias.SetRotationDeg(CVector3::AxisX(), 90.0f); 

	Dir = m_lockPos - pos; axis = Dir; axis.y = 0.0f;
	if (axis.LengthSq() > FLT_EPSILON) {
		axis.Normalize();
		Dir.Normalize();
		float sign = 1.0f;
		if (CVector2(-1.0f, 0.0f).Cross({ 0.0f,Dir.y }) < 0.0f) { sign = -1.0f; }
		rot2.SetRotation(CVector3::AxisX(), sign*CVector3::AngleOf2NormalizeVector(axis, Dir));
	}
	Dir = m_lockPos - pos; Dir.y = 0.0f;
	if (Dir.LengthSq() > FLT_EPSILON) {
		Dir.Normalize();
		float sign = 1.0f;
		if (CVector2(0.0f, 1.0f).Cross({ Dir.x,Dir.z }) < 0.0f) { sign = -1.0f; }
		rot.SetRotation(CVector3::AxisZ(), sign*CVector3::AngleOf2NormalizeVector(CVector3::AxisZ(), Dir));
	}

	rot.Multiply(bias, rot);
	rot.Multiply(rot, rot2);

	new SuicideObj::CEffekseer(L"Resource/effect/lazerShot_lazer.efk", 1.0f, pos, rot, { 50.0f,(m_lockPos - pos).Length() / 10.0f,50.0f });
	new SuicideObj::CEffekseer(L"Resource/effect/lazerShot_end.efk"  , 50.0f, m_lockPos);
	new SuicideObj::CEffekseer(L"Resource/effect/lazerShot_start.efk", 1.0f, pos, CQuaternion::Identity(), {50.0f,450.0f,50.0f});

	//�����ȋʂ��o��
	NormalBullet* B = new NormalBullet(player, m_lockPos, CVector3::AxisY()*0.1f, nullptr, 100);
	B->SetLifeTime(0.0f);

	//�v���C���[����
	if (player->GetLockOnIsPly()) {
		m_pPly->SetFlyTimer(0.0f);
	}

	return B;
}

WeaponInfo Lazer::getInfo(unsigned int shotAnim, unsigned int reloadAnim) {
	WeaponInfo info;
	info.modelPath = L"Resource/modelData/Lazer.cmo";
	info.spritePath = L"Resource/spriteData/Lazer.dds";
	info.maxBullet = 1;
	info.shotCool = 1.5f;
	info.zoomScale = 5.0f;
	info.shotAnimNum = shotAnim;
	info.reloadAnimNum = reloadAnim;
	return info;
}

namespace{
	//�_P�Ɛ�(AB)�̋���
	float Distance_DotAndLine(const CVector3& P, const CVector3& A, const CVector3& B)
	{
		CVector3 AB, AP;

		AB = B - A;
		AP = P - A;

		//AB�AAP���O�ς��ċ��߂�ꂽ�x�N�g���̒������A���s�l�ӌ`D�̖ʐςɂȂ�
		AB.Cross(AP); float D = AB.Length();

		//AB�Ԃ̋���
		float L = (A - B).Length();	//AB�̒���

		return D / L;
	}
}

void Lazer::WeaponUpdate(){

	if (!isActive) {
		player->SetLockOn(player->GetLockOnIsPly(), -1);
		if (m_eff) { delete m_eff; m_eff = nullptr; }
		m_effCnt = 0.0f;
		m_charge = 0.0f;
		m_isOnEFF = false;
		return;
	}
		
	//���b�N�p������
	if (player->GetLockOnNum() >= 0) {
		CVector3 rayStart = player->getPosition(); if (!player->isFlying()) { rayStart.y += 60.0f; }
		CVector3 targetPos;
		if (player->GetLockOnIsPly()) {
			targetPos = m_pPly->getPosition(); if (!m_pPly->isFlying()) { targetPos.y += 60.0f; }
		}
		else {
			targetPos = m_pCiti->getPos(); targetPos.y += 60.0f;
		}
		//��Q������
		btCollisionWorld::AllHitsRayResultCallback callback(rayStart, targetPos);
		GetPhysicsWorld().RayTest(rayStart, targetPos, callback);
		if (callback.hasHit()) {
			int i;
			for (i = 0; i < callback.m_collisionObjects.size(); ++i) {
				const btCollisionObject* col = callback.m_collisionObjects[i];
				if (col->getUserIndex() != enCollisionAttr_Character
					&& col->getInternalType() != btCollisionObject::CO_GHOST_OBJECT
				) {
					player->SetLockOn(player->GetLockOnIsPly(), -1);
					break;
				}
			}
		}
		//���W�X�V
		m_lockPos = targetPos;
	}	
	
	//�`���[�W�G�t�F�N�g
	if (m_isOnEFF) {
		m_effCnt -= GetDeltaTimeSec();
		CVector3 pos = player->getPosition();
		//SuicideObj::CSE* se = NewGO<SuicideObj::CSE>(L"Resource/sound/SE_shot.wav");
		//se->SetPos(pos);//���̈ʒu
		//se->SetDistance(500.0f);//������������͈�
		//se->Play(true); //��������true

		//�e������(����)�Əꏊ���w�肵�Ĕ���
		if (player->isFlying()) {
			pos += player->GetActionSender().getLookVec() * (100.0f + 50.0f);
		}
		else {
			pos += player->GetActionSender().getLookVec() * (50.0f + 50.0f);
			pos.y += 60.0f;
		}
		if (m_effCnt <= 0.0f) {
			if (m_eff) { delete m_eff; m_eff = nullptr;}
			m_eff = new SuicideObj::CEffekseer(L"Resource/effect/lazerCharge.efk", 10.0f, pos);
			m_effCnt = 1.6f;
		}
		m_eff->SetPos(pos);
		m_eff->SetScale(CVector3::One()*(m_charge+1.0f));
	}
	else {
		player->SetLockOn(player->GetLockOnIsPly(), -1);
		if (m_eff) { delete m_eff; m_eff = nullptr; }
		m_effCnt = 0.0f;
		m_charge = 0.0f;
	}
	m_isOnEFF = false;
}

void Lazer::PreShot() {
	if (bulletCount == 0) { return; }

	if (player->GetLockOnNum() < 0 && m_charge < FLT_EPSILON) {
		LockOn();
	}
	//if (player->GetLockOnNum() >= 0) {
		m_isOnEFF = true;
		m_charge += GetDeltaTimeSec();
		if (m_charge < 1.0f) {
			shotCool = 1.5f;
		}
		else {
			shotCool = 0.0f;
		}
	//}
}

void Lazer::LockOn() {
#ifndef SpritScreen
	if (player->playerNum != GetPhoton()->GetLocalPlayerNumber()) {
		return;
	}
#endif

	bool isFirst = true;
	float distance = -1.0f;
	const float CAM_FAR = 4000.0f;
	CVector3 rayStart = player->getPosition(); if (!player->isFlying()) { rayStart.y += 60.0f; }

	//�v���C���[���b�N�I��
	for (auto& C : m_game->getPlayers()) {
		if (player->playerNum == C.second->playerNum) { continue; }

		CVector3 dist = C.second->getPosition() - player->getPosition();
		if (player->GetActionSender().getLookVec().Dot(dist) > 0.0f && dist.LengthSq() > 230.0f*230.0f) {
			
			float d = Distance_DotAndLine(C.second->getPosition(), rayStart, rayStart + player->GetActionSender().getLookVec()*CAM_FAR);
			if (isFirst || d < distance) {

				CVector3 targetPos = C.second->getPosition(); if (!C.second->isFlying()) { targetPos.y += 60.0f; }
				//��Q������
				btCollisionWorld::AllHitsRayResultCallback callback(rayStart, targetPos);
				GetPhysicsWorld().RayTest(rayStart, targetPos, callback);
				if (callback.hasHit()) {
					int i;
					for (i = 0; i < callback.m_collisionObjects.size(); ++i) {
						const btCollisionObject* col = callback.m_collisionObjects[i];
						if (col->getUserIndex() != enCollisionAttr_Character
							&& col->getInternalType() != btCollisionObject::CO_GHOST_OBJECT
							) {
							break;
						}
					}
					if (i != callback.m_collisionObjects.size()) { continue; }
				}

				isFirst = false;
				distance = d;
				m_lockPos = targetPos;
				player->SetLockOn(true, C.second->playerNum);
				m_pPly = C.second;
			}
		}
	}
	//�s�����b�N�I��
	for (int i = 0; i < m_game->GetCitizenGene().GetCitizenNum(); i++) {
		const Citizen* C = m_game->GetCitizenGene().GetCitizen(i);

		CVector3 dist = C->getPos() - player->getPosition();
		if (player->GetActionSender().getLookVec().Dot(dist) > 0.0f && dist.LengthSq() > 230.0f*230.0f) {
			
			float d = Distance_DotAndLine(C->getPos(), rayStart, rayStart + player->GetActionSender().getLookVec()*CAM_FAR);
			if (isFirst || d < distance) {

				CVector3 targetPos = C->getPos(); targetPos.y += 60.0f;
				//��Q������
				btCollisionWorld::AllHitsRayResultCallback callback(rayStart, targetPos);
				GetPhysicsWorld().RayTest(rayStart, targetPos, callback);
				if (callback.hasHit()) {
					int i;
					for (i = 0; i < callback.m_collisionObjects.size(); ++i) {
						const btCollisionObject* col = callback.m_collisionObjects[i];
						if (col->getUserIndex() != enCollisionAttr_Character
							&& col->getInternalType() != btCollisionObject::CO_GHOST_OBJECT
							) {
							break;
						}
					}
					if (i != callback.m_collisionObjects.size()) { continue; }
				}

				isFirst = false;
				distance = d;
				m_lockPos = targetPos;
				player->SetLockOn(false, i);
				m_pCiti = C;
			}
		}
	}
}

void Lazer::PostRender(){
#ifndef SpritScreen
	if (player->playerNum != GetPhoton()->GetLocalPlayerNumber()) {
		return;
	}
#endif

	if (player->GetLockOnNum() < 0) { return; }//�m�[���b�N

	CVector3 vec = GetMainCamera()->CalcScreenPosFromWorldPos(m_lockPos);
	if (vec.z > 0.0f && vec.z < 1.0f) {
		m_sprite.Draw(vec, { 0.75f,0.75f }, { 0.5f,0.5f }, 0.0f, {0.0f,1.0f,0.0f,1.0f});
	}
}